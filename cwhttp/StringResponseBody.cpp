
#include "StringResponseBody.h"

namespace cwhttp {

    StringResponseBody::StringResponseBody() {

    }

    StringResponseBody::~StringResponseBody() {

    }

    bool StringResponseBody::onWriteStart(std::size_t length) {
        return true;
    }

    std::size_t StringResponseBody::onWrite(const char* pData, std::size_t length) {
        m_Body.append(pData, length);
        return length;
    }

    void StringResponseBody::onWriteEnded() {

    }

    void StringResponseBody::onReset() {
        m_Body.clear();
    }

    const std::string& StringResponseBody::getAsString() const {
        return m_Body;
    }

}