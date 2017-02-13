
#ifndef CWHTTP_STRINGREQUESTBODY_H
#define CWHTTP_STRINGREQUESTBODY_H

#include <sstream>
#include <string>
#include "RequestBody.h"

namespace cwhttp {

    class StringRequestBody : public RequestBody {
    private:
        std::string m_ContentType;
        std::istringstream m_Body;

    public:
        StringRequestBody(const std::string& contentType, const std::string& body);
        StringRequestBody(std::string&& contentType, std::string&& body);
        virtual ~StringRequestBody() override;

        virtual const std::string& getContentType() const override;
        virtual std::size_t getContentLength() const override;
        virtual bool onSeek(std::streamoff position) override;
        virtual std::size_t onRead(char* pBuffer, std::size_t atMost) override;

    };

}

#endif //CWHTTP_STRINGREQUESTBODY_H
