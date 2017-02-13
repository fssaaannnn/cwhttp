
#include <curl/curl.h>
#include "HttpClient.h"
#include "ScopedCurl.h"
#include "Method.h"
#include "Request.h"
#include "Response.h"
#include "ScopedSList.h"
#include "StringUtils.h"
#include "Header.h"
#include "Logger.h"
#include "Cookie.h"
#include "CookieJar.h"
#include "HeaderContainer.h"
#include "ResponseBody.h"

namespace cwhttp {

    class CurlInitializer {
    public:
        CurlInitializer() {
            curl_global_init(CURL_GLOBAL_DEFAULT);
        }

        ~CurlInitializer() {
            curl_global_cleanup();
        }
    };

    static const CurlInitializer CURL_INITIALIZER;
    constexpr const char* UA_CURL = "libcurl/";

    HttpClient::HttpClient()
            : m_IsDebug(false)
            , m_UserAgent(getDefaultUserAgent())
            , m_CaBundle()
            , m_ProxyHost()
            , m_ProxyPort(0)
            , m_ConnectionTimeout(0)
            , m_pCookieJar(nullptr) {

    }

    HttpClient::WriteControl::WriteControl(Response* pR, bool follow, CookieList* pC)
            : pResponse(pR)
            , lastCode(0)
            , followRedirect(follow)
            , isInited(false)
            , pReceivedCookies(pC) {

    }

    bool HttpClient::WriteControl::isRedirect() const {
        return lastCode != 0 && (300 <= lastCode && lastCode < 400);
    }

    bool HttpClient::WriteControl::shallWrite() const {
        return followRedirect ? !isRedirect() : true;
    }

    static int getStatusCode(const std::string& line) {
        std::istringstream s(line);
        std::string protocol;
        s >> protocol;

        if (!s.good() || !StringUtils::hasPrefix(protocol, "HTTP/")) return 0;

        int code;
        s >> code;
        return s.good() ? code : 0;
    }

    void HttpClient::constructResponse(Response* pResponse, const std::string& name, const std::string& value) {
        if (name.compare(HEADER_CONTENT_LENGTH) == 0) {
            std::istringstream is(value);
            std::size_t cl;
            is >> cl;
            pResponse->setContentLength(cl);
        } else if (name.compare(HEADER_CONTENT_TYPE) == 0) {
            pResponse->setContentType(value);
        }
    }

    std::size_t HttpClient::onCurlWriteHeader(const char* pData, std::size_t size, std::size_t n, void* pUserData) {
        HttpClient::WriteControl* pControl = static_cast<HttpClient::WriteControl*>(pUserData);
        HeaderContainer* pHeaders = pControl->pResponse->getModifiableHeaders();
        const std::size_t length = size * n;
        std::string line(pData, length);

        // ignore CR LF
        if (line.compare("\x0D\x0A") == 0) return length;

        const int code = getStatusCode(line);
        if (code != 0) {
            pControl->lastCode = code;
        }

        std::string name;
        std::string value;
        if (HeaderContainer::parseLine(line, name, value)) {
            if (pControl->shallWrite()) {
                pHeaders->addTrimmed(name, value);
                constructResponse(pControl->pResponse, name, value);
            }
            if (name.compare(HEADER_SET_COOKIE) == 0) {
                const Cookie c(value);
                if (c.isValid()) {
                    pControl->pReceivedCookies->emplace_back(c);
                }
            }
        }
        return length;
    }

    std::size_t HttpClient::onCurlWriteBody(const char* pData, std::size_t size, std::size_t n, void* pUserData) {
        HttpClient::WriteControl* pControl = static_cast<HttpClient::WriteControl*>(pUserData);
        const std::size_t length = size * n;
        if (pControl->shallWrite()) {
            if (!pControl->isInited) {
                pControl->isInited = true;
                if (!pControl->pResponse->startWrite()) {
                    return 0;
                }
            }
            return pControl->pResponse->executeWrite(pData, length);
        } else {
            return length;
        }
    }

    std::string HttpClient::buildCookieHeader(const CookieList& cookies) {
        std::string header;
        for (auto it = cookies.begin(); it != cookies.end();) {
            header += it->getName();
            header += "=";
            header += it->getValue();
            ++it;
            if (it != cookies.end()) {
                header += "; ";
            }
        }
        return header;
    }

    int HttpClient::onCurlSeekBody(void* pUserData, int64_t offset, int origin) {
        Request* pRequest = static_cast<Request*>(pUserData);
        int ret;
        switch (origin) {
        case SEEK_SET:
            ret = pRequest->executeSeek(offset) ? CURL_SEEKFUNC_OK : CURL_SEEKFUNC_FAIL;
            break;
        case SEEK_CUR:
        case SEEK_END:
        default:
            ret = CURL_SEEKFUNC_FAIL;
            break;
        }
        return ret;
    }

    std::size_t HttpClient::onCurlReadBody(char* pBuffer, std::size_t size, std::size_t n, void* pUserData) {
        Request* pRequest = static_cast<Request*>(pUserData);
        const std::size_t atMost = size * n;
        return pRequest->executeRead(pBuffer, atMost);
    }

    struct DebugConfig {
        bool traceSslData = false;
    };

    constexpr int MAX_LOG_LENGTH = 512;

    static int onDebugDataReceived(CURL* pCurl, curl_infotype type, const char* pData, const std::size_t size,
            void* pUserData) {

        const bool shallTruncate = size > MAX_LOG_LENGTH;
        std::size_t logSize = shallTruncate ? MAX_LOG_LENGTH : size;

        std::string data(pData, logSize);
        if (StringUtils::hasSuffix(data, "\x0D\x0A\x0D\x0A")) {
            data.resize(data.size() - 4);
        } else if (StringUtils::hasSuffix(data, "\x0D\x0A")) {
            data.resize(data.size() - 2);
        } else if (*data.rbegin() == '\x0D' || *data.rbegin() == '\x0A') {
            data.pop_back();
        }
        std::transform(data.begin(), data.end(), data.begin(), [](char c) {
            if (std::isprint(c) || c == '\x0D' || c == '\x0A' || c == '\x09') {
                return c;
            } else {
                return '.';
            }
        });

        if (shallTruncate) {
            data += "   ...and more";
        }

        DebugConfig* pConfig = static_cast<DebugConfig*>(pUserData);
        const char* pText;

        switch (type) {
        case CURLINFO_TEXT:
            LOGD("== Info: %s", data.c_str());
        default: /* in case a new one is introduced to shock us */
            return 0;
        case CURLINFO_HEADER_OUT:
            pText = "=> Send header";
            break;
        case CURLINFO_DATA_OUT:
            pText = "=> Send data";
            break;
        case CURLINFO_SSL_DATA_OUT:
            if (!pConfig->traceSslData) return 0;
            pText = "=> Send SSL data";
            break;
        case CURLINFO_HEADER_IN:
            pText = "<= Recv header";
            break;
        case CURLINFO_DATA_IN:
            pText = "<= Recv data";
            break;
        case CURLINFO_SSL_DATA_IN:
            if (!pConfig->traceSslData) return 0;
            pText = "<= Recv SSL data";
            break;
        }

        LOGD("%s, %10.10ld bytes (0x%8.8lX)", pText, (long) size, (long) size);
        LOGD("%s", data.c_str());
        return 0;
    }

    bool HttpClient::execute(const Request& request, Response& response) const {
        const ScopedCurl curl;
        if (!curl.isValid()) {
            return false;
        }

        DebugConfig debugConfig;
        if (m_IsDebug) {
            curl_easy_setopt(curl(), CURLOPT_DEBUGFUNCTION, onDebugDataReceived);
            curl_easy_setopt(curl(), CURLOPT_DEBUGDATA, &debugConfig);
            curl_easy_setopt(curl(), CURLOPT_VERBOSE, 1L);
        }
        if (m_CaBundle.length() != 0) {
            curl_easy_setopt(curl(), CURLOPT_CAINFO, m_CaBundle.c_str());
        }
        if (m_ProxyHost.length() != 0) {
            curl_easy_setopt(curl(), CURLOPT_PROXY, m_ProxyHost.c_str());
            if (m_ProxyPort > 0) {
                curl_easy_setopt(curl(), CURLOPT_PROXYPORT, static_cast<long>(m_ProxyPort));
            }
        }
        if (m_ConnectionTimeout > 0) {
            curl_easy_setopt(curl(), CURLOPT_CONNECTTIMEOUT, static_cast<long>(m_ConnectionTimeout));
        }
        curl_easy_setopt(curl(), CURLOPT_USERAGENT, m_UserAgent.c_str());
        curl_easy_setopt(curl(), CURLOPT_FOLLOWLOCATION, request.isFollowRedirect() ? 1L : 0L);

        curl_easy_setopt(curl(), CURLOPT_URL, request.getUrl().c_str());

        CookieList receivedCookies;
        WriteControl control(&response, request.isFollowRedirect(), &receivedCookies);
        curl_easy_setopt(curl(), CURLOPT_HEADERFUNCTION, onCurlWriteHeader);
        curl_easy_setopt(curl(), CURLOPT_HEADERDATA, &control);
        curl_easy_setopt(curl(), CURLOPT_WRITEFUNCTION, onCurlWriteBody);
        curl_easy_setopt(curl(), CURLOPT_WRITEDATA, &control);

        ScopedSList headerLines;
        request.getHeaders()->iterateAllLines([&headerLines](const std::string& headerLine) {
            headerLines.append(headerLine.c_str());
        });
        if (request.hasBody()) {
            headerLines.append(HeaderContainer::buildLine(HEADER_CONTENT_TYPE, request.getContentType()).c_str());
            if (request.isChunked()) {
                headerLines.append(HeaderContainer::buildLine(HEADER_TRANSFER_ENCODING, "chunked").c_str());
            }
        }
        curl_easy_setopt(curl(), CURLOPT_HTTPHEADER, headerLines());

        if (m_pCookieJar != nullptr) {
            CookieList cookiesToSend;
            m_pCookieJar->loadForRequest(request.getUrl(), cookiesToSend);
            if (cookiesToSend.size() != 0) {
                std::string cookieString = buildCookieHeader(cookiesToSend);
                curl_easy_setopt(curl(), CURLOPT_COOKIE, cookieString.c_str());
            }
        }

        switch (request.getMethod()) {
        case Method::GET:
            curl_easy_setopt(curl(), CURLOPT_HTTPGET, 1L);
            break;
        case Method::POST:
            curl_easy_setopt(curl(), CURLOPT_POST, 1L);
            curl_easy_setopt(curl(), CURLOPT_SEEKFUNCTION, onCurlSeekBody);
            curl_easy_setopt(curl(), CURLOPT_SEEKDATA, &request);
            curl_easy_setopt(curl(), CURLOPT_READFUNCTION, onCurlReadBody);
            curl_easy_setopt(curl(), CURLOPT_READDATA, &request);
            if (!request.isChunked()) {
                const curl_off_t contentLength = request.getContentLength();
                curl_easy_setopt(curl(), CURLOPT_POSTFIELDSIZE_LARGE, contentLength);
            }
            break;
        case Method::PUT:
            curl_easy_setopt(curl(), CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl(), CURLOPT_SEEKFUNCTION, onCurlSeekBody);
            curl_easy_setopt(curl(), CURLOPT_SEEKDATA, &request);
            curl_easy_setopt(curl(), CURLOPT_READFUNCTION, onCurlReadBody);
            curl_easy_setopt(curl(), CURLOPT_READDATA, &request);
            if (!request.isChunked()) {
                const curl_off_t contentLength = request.getContentLength();
                curl_easy_setopt(curl(), CURLOPT_INFILESIZE_LARGE, contentLength);
            }
            break;
        case Method::PATCH:
            curl_easy_setopt(curl(), CURLOPT_POST, 1L);
            curl_easy_setopt(curl(), CURLOPT_SEEKFUNCTION, onCurlSeekBody);
            curl_easy_setopt(curl(), CURLOPT_SEEKDATA, &request);
            curl_easy_setopt(curl(), CURLOPT_READFUNCTION, onCurlReadBody);
            curl_easy_setopt(curl(), CURLOPT_READDATA, &request);
            if (!request.isChunked()) {
                const curl_off_t contentLength = request.getContentLength();
                curl_easy_setopt(curl(), CURLOPT_POSTFIELDSIZE_LARGE, contentLength);
            }
            curl_easy_setopt(curl(), CURLOPT_CUSTOMREQUEST, METHOD_PATCH);
            break;
        case Method::DEL:
            curl_easy_setopt(curl(), CURLOPT_CUSTOMREQUEST, METHOD_DELETE);
            break;
        case Method::HEAD:
            curl_easy_setopt(curl(), CURLOPT_NOBODY, 1L);
            break;
        }

        CURLcode res = curl_easy_perform(curl());
        if (res != CURLE_OK) {
            LOGW("curl_easy_perform() failed %d: %s\n", res, curl_easy_strerror(res));
            return false;
        }
        if (m_pCookieJar != nullptr && receivedCookies.size() != 0) {
            m_pCookieJar->saveFromResponse(request.getUrl(), receivedCookies);
        }

        long statusCode;
        curl_easy_getinfo(curl(), CURLINFO_RESPONSE_CODE, &statusCode);
        if (control.isInited) {
            response.endWrite();
        }

        response.setCode(static_cast<int32_t>(statusCode));

        return true;
    }

    bool HttpClient::isDebug() const {
        return m_IsDebug;
    }

    void HttpClient::setDebug(bool value) {
        m_IsDebug = value;
    }

    std::string HttpClient::getDefaultUserAgent() {
        const curl_version_info_data* pData = curl_version_info(CURLVERSION_NOW);
        return std::string(UA_CURL).append(pData->version);
    }

    const std::string& HttpClient::getUserAgent() const {
        return m_UserAgent;
    }

    void HttpClient::setUserAgent(const std::string& userAgent) {
        if (m_UserAgent.length() == 0) return;

        m_UserAgent = userAgent;
    }

    void HttpClient::setCaBundle(const std::string& caBundle) {
        m_CaBundle = caBundle;
    }

    void HttpClient::setProxy(const std::string& host, std::int32_t port) {
        m_ProxyHost = host;
        m_ProxyPort = port;
    }

    void HttpClient::setConnectionTimeout(std::int32_t connectionTimeout) {
        m_ConnectionTimeout = connectionTimeout;
    }

    void HttpClient::setCookieJar(CookieJar* pCookieJar) {
        m_pCookieJar = pCookieJar;
    }

}