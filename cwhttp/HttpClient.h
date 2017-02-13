
#ifndef CWHTTP_HTTPCLIENT_H
#define CWHTTP_HTTPCLIENT_H

#include <string>
#include <vector>

namespace cwhttp {

    class Request;
    class Response;
    class Cookie;
    class CookieJar;

    using CookieList = std::vector<Cookie>;

    class HttpClient {
    private:
        bool m_IsDebug;
        std::string m_UserAgent;
        std::string m_CaBundle;
        std::string m_ProxyHost;
        std::int32_t m_ProxyPort;
        std::int32_t m_ConnectionTimeout;
        CookieJar* m_pCookieJar;

    public:
        HttpClient();

        bool isDebug() const;
        void setDebug(bool value);
        static std::string getDefaultUserAgent();
        const std::string& getUserAgent() const;
        void setUserAgent(const std::string& userAgent);
        void setCaBundle(const std::string& caBundle);
        void setProxy(const std::string& host, std::int32_t port);
        void setConnectionTimeout(std::int32_t connectionTimeout);
        void setCookieJar(CookieJar* pCookieJar);
        bool execute(const Request& request, Response& response) const;

        struct WriteControl {
            Response* pResponse = nullptr;
            int lastCode = 0;
            bool followRedirect = false;
            bool isInited = false;
            CookieList* pReceivedCookies = nullptr;

            WriteControl(Response* pR, bool follow, CookieList* pC);

            bool isRedirect() const;
            bool shallWrite() const;
        };

    private:
        static int onCurlSeekBody(void* pUserData, int64_t offset, int origin);
        static std::size_t onCurlReadBody(char* pBuffer, std::size_t size, std::size_t n, void* pUserData);

        static void constructResponse(Response* pResponse, const std::string& name, const std::string& value);
        static std::size_t onCurlWriteHeader(const char* pData, std::size_t size, std::size_t n, void* pUserData);
        static std::size_t onCurlWriteBody(const char* pData, std::size_t size, std::size_t n, void* pUserData);
        static std::string buildCookieHeader(const CookieList& cookies);
    };

}

#endif //CWHTTP_HTTPCLIENT_H
