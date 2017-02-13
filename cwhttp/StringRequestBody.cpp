
#include "StringRequestBody.h"

namespace cwhttp {

    StringRequestBody::StringRequestBody(const std::string& contentType, const std::string& body)
            : m_ContentType(contentType)
            , m_Body(body) {

    }

    StringRequestBody::StringRequestBody(std::string&& contentType, std::string&& body)
            : m_ContentType(contentType)
            , m_Body(body) {

    }

    StringRequestBody::~StringRequestBody() {

    }

    const std::string& StringRequestBody::getContentType() const {
        return m_ContentType;
    }

    std::size_t StringRequestBody::getContentLength() const {
        return m_Body.str().length();
    }

    bool StringRequestBody::onSeek(std::streamoff position) {
        m_Body.clear();
        m_Body.seekg(position, std::ios::beg);
        bool isOk = !m_Body.fail();
        return isOk;
    }

    std::size_t StringRequestBody::onRead(char* pBuffer, std::size_t atMost) {
        if (m_Body.eof()) {
            return 0;
        }
        const auto readLength = m_Body.read(pBuffer, atMost).gcount();
        return static_cast<std::size_t>(readLength);
    }

}