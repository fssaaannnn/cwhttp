
#ifndef CWHTTP_RESPONSE_H
#define CWHTTP_RESPONSE_H

#include <string>
#include <memory>

namespace cwhttp {

    class HttpClient;
    class HeaderContainer;
    class ResponseBody;

    class Response {
    private:
        friend class HttpClient;

        int32_t m_Code;
        std::unique_ptr<HeaderContainer> m_Headers;
        ResponseBody* m_pBody;
        std::string m_EmptyContentType;

    public:
        Response();
        Response(const Response& other) = delete;
        ~Response();

        int32_t getCode() const;
        bool isSuccessful() const;
        const HeaderContainer* getHeaders() const;
        std::size_t getContentLength() const;
        const std::string& getContentType() const;
        void setBody(ResponseBody* pResponseBody);
        void reset();

    private:
        void setCode(int32_t code);
        HeaderContainer* getModifiableHeaders();
        void setContentLength(std::size_t value);
        void setContentType(const std::string& value);
        bool startWrite();
        std::size_t executeWrite(const char* pData, std::size_t length);
        void endWrite();

    };

}

#endif //CWHTTP_RESPONSE_H
