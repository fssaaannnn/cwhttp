
#include "Response.h"
#include "HeaderContainer.h"
#include "ResponseBody.h"

namespace cwhttp {

    Response::Response()
            : m_Code(0)
            , m_Headers(new HeaderContainer())
            , m_pBody(nullptr) {

    }

    Response::~Response() {

    }

    int32_t Response::getCode() const {
        return m_Code;
    }

    void Response::setCode(int32_t code) {
        m_Code = code;
    }

    bool Response::isSuccessful() const {
        return m_Code >= 200 && m_Code < 300;
    }

    HeaderContainer* Response::getModifiableHeaders() {
        return m_Headers.get();
    }

    const HeaderContainer* Response::getHeaders() const {
        return m_Headers.get();
    }

    std::size_t Response::getContentLength() const {
        if (m_pBody) {
            return m_pBody->getContentLength();
        } else {
            return 0;
        }
    }

    const std::string& Response::getContentType() const {
        if (m_pBody) {
            return m_pBody->getContentType();
        } else {
            return m_EmptyContentType;
        }
    }

    void Response::setBody(ResponseBody* pResponseBody) {
        m_pBody = pResponseBody;
    }

    void Response::reset() {
        m_Code = 0;
        m_Headers->reset();
        if (m_pBody) {
            m_pBody->reset();
        }
    }

    void Response::setContentType(const std::string& value) {
        if (m_pBody) {
            m_pBody->setContentType(value);
        }
    }
    void Response::setContentLength(std::size_t value) {
        if (m_pBody) {
            m_pBody->setContentLength(value);
        }
    }

    bool Response::startWrite() {
        if (m_pBody) {
            return m_pBody->onWriteStart(m_pBody->getContentLength());
        } else {
            return false;
        }
    }

    std::size_t Response::executeWrite(const char* pData, std::size_t length) {
        if (m_pBody) {
            return m_pBody->onWrite(pData, length);
        } else {
            return 0;
        }
    }

    void Response::endWrite() {
        if (m_pBody) {
            m_pBody->onWriteEnded();
        }
    }

}