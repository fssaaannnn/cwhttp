
#include "Request.h"
#include "Method.h"
#include "HeaderContainer.h"
#include "RequestBuilder.h"
#include "RequestBody.h"

namespace cwhttp {

    Request::Request(const RequestBuilder& builder)
            : m_Method(builder.m_Method)
            , m_Url(builder.m_Url)
            , m_Headers(new HeaderContainer(*builder.m_Headers))
            , m_pRequestBody(builder.m_pBody)
            , m_Chunked(builder.m_Chunked)
            , m_FollowRedirect(builder.m_FollowRedirect) {
    }

    Request::~Request() {
    }

    Method Request::getMethod() const {
        return m_Method;
    }

    const std::string& Request::getUrl() const {
        return m_Url;
    }

    const HeaderContainer* Request::getHeaders() const {
        return m_Headers.get();
    }

    bool Request::hasBody() const {
        return m_pRequestBody != nullptr;
    }

    std::size_t Request::getContentLength() const {
        return m_pRequestBody ? m_pRequestBody->getContentLength() : 0;
    }

    const char* Request::getContentType() const {
        return m_pRequestBody ? m_pRequestBody->getContentType().c_str() : nullptr;
    }

    bool Request::isChunked() const {
        return m_Chunked;
    }

    bool Request::isFollowRedirect() const {
        return m_FollowRedirect;
    }

    bool Request::executeSeek(std::streamoff position) const {
        return m_pRequestBody ? m_pRequestBody->onSeek(position) : true;
    }

    std::size_t Request::executeRead(char* pBuffer, std::size_t atMost) const {
        return m_pRequestBody ? m_pRequestBody->onRead(pBuffer, atMost) : 0;
    }

}