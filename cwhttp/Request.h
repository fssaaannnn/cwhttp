
#ifndef CWHTTP_REQUEST_H
#define CWHTTP_REQUEST_H

#include <string>
#include <memory>

namespace cwhttp {

    enum class Method : int32_t;

    class HeaderContainer;
    class RequestBuilder;
    class RequestBody;

    class Request {
    private:
        friend class HttpClient;

        Method m_Method;
        std::string m_Url;
        std::unique_ptr<HeaderContainer> m_Headers;
        RequestBody* m_pRequestBody;
        bool m_Chunked;
        bool m_FollowRedirect;

    public:
        Request(const RequestBuilder& builder);
        Request(const Request& other) = delete;
        ~Request();

        Method getMethod() const;
        const std::string& getUrl() const;
        const HeaderContainer* getHeaders() const;
        bool hasBody() const;
        std::size_t getContentLength() const;
        const char* getContentType() const;
        bool isChunked() const;
        bool isFollowRedirect() const;

    private:
        bool executeSeek(std::streamoff position) const;
        std::size_t executeRead(char* pBuffer, std::size_t atMost) const;

    };

}

#endif //CWHTTP_REQUEST_H
