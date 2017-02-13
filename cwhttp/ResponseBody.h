
#ifndef CWHTTP_RESPONSEBODY_H
#define CWHTTP_RESPONSEBODY_H

#include <string>

namespace cwhttp {

    class Response;

    class ResponseBody {
    private:
        friend class Response;

        std::string m_ContentType;
        std::size_t m_ContentLength;

    public:
        ResponseBody();
        virtual ~ResponseBody();

        const std::string& getContentType() const;
        std::size_t getContentLength() const;

    protected:
        virtual bool onWriteStart(std::size_t length) = 0;
        virtual std::size_t onWrite(const char* pData, std::size_t length) = 0;
        virtual void onWriteEnded() = 0;
        virtual void onReset() = 0;

    private:
        void setContentType(const std::string& value);
        void setContentLength(std::size_t value);
        void reset();

    };

}

#endif //CWHTTP_RESPONSEBODY_H
