
#include "ResponseBody.h"

namespace cwhttp {

    ResponseBody::ResponseBody()
            : m_ContentType()
            , m_ContentLength(0) {

    }

    ResponseBody::~ResponseBody() {

    }

    const std::string& ResponseBody::getContentType() const {
        return m_ContentType;
    }

    void ResponseBody::setContentType(const std::string& value) {
        m_ContentType = value;
    }

    std::size_t ResponseBody::getContentLength() const {
        return m_ContentLength;
    }

    void ResponseBody::setContentLength(std::size_t value) {
        m_ContentLength = value;
    }

    void ResponseBody::reset() {
        m_ContentType = "";
        m_ContentLength = 0;
        onReset();
    }

}