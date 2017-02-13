
#ifndef CWHTTP_REQUESTBUILDER_H
#define CWHTTP_REQUESTBUILDER_H

#include <string>
#include <memory>

namespace cwhttp {

    enum class Method : int;

    class HeaderContainer;
    class RequestBody;
    class Request;

    class RequestBuilder {
    private:
        friend class Request;

        Method m_Method;
        std::string m_Url;
        std::unique_ptr<HeaderContainer> m_Headers;
        RequestBody* m_pBody;
        bool m_Chunked;
        bool m_FollowRedirect;

    public:
        RequestBuilder();
        RequestBuilder(const RequestBuilder& other) = delete;
        ~RequestBuilder();

        RequestBuilder& setMethod(Method method);
        RequestBuilder& setUrl(const std::string& url);
        RequestBuilder& addHeader(const std::string& name, const std::string& value);
        RequestBuilder& setHeader(const std::string& name, const std::string& value);
        RequestBuilder& removeHeader(const std::string& name);
        RequestBuilder& setBody(RequestBody* pBody);
        RequestBuilder& setChunked(bool chunked);
        RequestBuilder& setFollowRedirect(bool followRedirect);
        std::unique_ptr<Request> build() const;


    };

}

#endif //CWHTTP_REQUESTBUILDER_H
