
#include "RequestBuilder.h"
#include "Method.h"
#include "HeaderContainer.h"
#include "Request.h"
#include "RequestBody.h"

namespace cwhttp {
    RequestBuilder::RequestBuilder()
            : m_Method(Method::GET)
            , m_Url()
            , m_Headers(new HeaderContainer())
            , m_pBody(nullptr)
            , m_Chunked(false)
            , m_FollowRedirect(false) {

    }

    RequestBuilder::~RequestBuilder() {
    }

    RequestBuilder& RequestBuilder::setMethod(Method method) {
        m_Method = method;
        return *this;
    }

    RequestBuilder& RequestBuilder::setUrl(const std::string& url) {
        m_Url = url;
        return *this;
    }

    RequestBuilder& RequestBuilder::addHeader(const std::string& name, const std::string& value) {
        m_Headers->addRaw(name, value);
        return *this;
    }

    RequestBuilder& RequestBuilder::setHeader(const std::string& name, const std::string& value) {
        m_Headers->setRaw(name, value);
        return *this;
    }

    RequestBuilder& RequestBuilder::removeHeader(const std::string& name) {
        m_Headers->remove(name);
        return *this;
    }

    RequestBuilder& RequestBuilder::setBody(RequestBody* pBody) {
        m_pBody = pBody;
        return *this;
    }

    RequestBuilder& RequestBuilder::setChunked(bool chunked) {
        m_Chunked = chunked;
        return *this;
    }

    RequestBuilder& RequestBuilder::setFollowRedirect(bool followRedirect) {
        m_FollowRedirect = followRedirect;
        return *this;
    }

    std::unique_ptr<Request> RequestBuilder::build() const {
        return std::unique_ptr<Request>(new Request(*this));
    }


}