
#ifndef CWHTTP_STRINGRESPONSEBODY_H
#define CWHTTP_STRINGRESPONSEBODY_H

#include "ResponseBody.h"

namespace cwhttp {

    class StringResponseBody : public ResponseBody {
    private:
        std::string m_Body;

    public:
        StringResponseBody();
        virtual ~StringResponseBody() override;

        virtual bool onWriteStart(std::size_t length) override;
        virtual std::size_t onWrite(const char* pData, std::size_t length) override;
        virtual void onWriteEnded() override;
        virtual void onReset() override;

        const std::string& getAsString() const;

    };

}

#endif //CWHTTP_STRINGRESPONSEBODY_H
