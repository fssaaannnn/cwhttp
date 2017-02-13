
#include <sstream>
#include "FormBodyBuilder.h"
#include "UrlUtils.h"
#include "StringRequestBody.h"

namespace cwhttp {

    constexpr const char* DEFAULT_FORM_CONTENT_TYPE = "application/x-www-form-urlencoded";

    FormBodyBuilder::FormBodyBuilder() {

    }

    FormBodyBuilder& FormBodyBuilder::add(const std::string& name, const std::string& value) {
        std::string encodedName = UrlUtils::escape(name);
        if (encodedName.length() == 0) {
            return *this;
        }
        std::string encodedValue = UrlUtils::escape(value);
        if (value.length() != 0 && encodedValue.length() == 0) {
            return *this;
        }
        addEncoded(std::move(encodedName), std::move(encodedValue));
        return *this;
    }

    FormBodyBuilder& FormBodyBuilder::addEncoded(const std::string& name, const std::string& value) {
        m_Entries.emplace(name, value);
        return *this;
    }

    FormBodyBuilder& FormBodyBuilder::addEncoded(std::string&& name, std::string&& value) {
        m_Entries.emplace(name, value);
        return *this;
    }

    std::unique_ptr<RequestBody> FormBodyBuilder::build() const {
        return std::unique_ptr<RequestBody>(
                new StringRequestBody(std::string(DEFAULT_FORM_CONTENT_TYPE), buildString()));
    }

    std::string FormBodyBuilder::buildString() const {
        std::string bodyString;
        for (const auto& entry:m_Entries) {
            bodyString.append(entry.first).append("=").append(entry.second).append("&");
        }
        if (bodyString.length() > 0 && *bodyString.rbegin() == '&') {
            bodyString.pop_back();
        }
        return bodyString;
    }

    void FormBodyBuilder::reset() {
        m_Entries.clear();
    }

}