
#ifndef CWHTTP_REQUESTBODY_H
#define CWHTTP_REQUESTBODY_H

#include <ios>

namespace cwhttp {

    class RequestBody {
    public:
        RequestBody();
        virtual ~RequestBody();

        virtual const std::string& getContentType() const = 0;
        virtual std::size_t getContentLength() const = 0;
        virtual bool onSeek(std::streamoff position) = 0;
        virtual std::size_t onRead(char* pBuffer, std::size_t atMost) = 0;
    };

}

#endif //CWHTTP_REQUESTBODY_H
