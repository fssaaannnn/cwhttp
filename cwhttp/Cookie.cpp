
#include "Cookie.h"
#include "StringUtils.h"

namespace cwhttp {

    Cookie::Cookie()
            : m_IsSecure(false)
            , m_IsHttpOnly(false) {

    }

    Cookie::Cookie(const std::string& name, const std::string& value)
            : m_Name(name)
            , m_Value(value)
            , m_IsSecure(false)
            , m_IsHttpOnly(false) {

    }

    Cookie::Cookie(const std::string& setCookieString)
            : m_IsSecure(false)
            , m_IsHttpOnly(false) {

        std::vector<std::string> elements;
        StringUtils::split(setCookieString, ';', elements, [](std::string& i) {
            StringUtils::trim(i);
            return i.length() != 0;
        });

        if (elements.size() == 0) return;

        StringUtils::SplitPredicate trimQuote = [](std::string& i) {
            StringUtils::trim(i, [](int c) { return StringUtils::isSpaceOrCtl(c) || c == '"'; });
            return i.length() != 0;
        };
        std::vector<std::string> pairs;
        bool nameValueParsed = false;
        bool expiresParsed = false;
        bool domainParsed = false;
        bool pathParsed = false;
        bool isSecureParsed = false;
        bool isHttpOnlyParsed = false;
        for (const auto& item : elements) {
            pairs.clear();
            StringUtils::split(item, '=', pairs, trimQuote);
            if (pairs.size() == 0) continue;

            if (nameValueParsed) {
                if (pairs.size() == 1) {
                    if (!isSecureParsed && StringUtils::equal("Secure", pairs[0])) {
                        isSecureParsed = true;
                        m_IsSecure = true;
                    } else if (!isHttpOnlyParsed && StringUtils::equal("HttpOnly", pairs[0])) {
                        isHttpOnlyParsed = true;
                        m_IsHttpOnly = true;
                    }
                } else if (pairs.size() == 2) {
                    if (!expiresParsed && StringUtils::equal("Expires", pairs[0])) {
                        expiresParsed = true;
                        m_Expires = pairs[1];
                    } else if (!domainParsed && StringUtils::equal("Domain", pairs[0])) {
                        domainParsed = true;
                        m_Domain = pairs[1];
                    } else if (!pathParsed && StringUtils::equal("Path", pairs[0])) {
                        pathParsed = true;
                        m_Path = pairs[1];
                    }
                }
            } else {
                nameValueParsed = true;
                if (pairs.size() != 2) {
                    return;
                } else {
                    m_Name = pairs[0];
                    m_Value = pairs[1];
                }
            }
        }
    }

    bool Cookie::isValid() const {
        return m_Name.length() != 0 && m_Value.length() != 0;
    }

    const std::string& Cookie::getName() const {
        return m_Name;
    }

    void Cookie::setName(const std::string& name) {
        m_Name = name;
    }

    const std::string& Cookie::getValue() const {
        return m_Value;
    }

    void Cookie::setValue(const std::string& value) {
        m_Value = value;
    }

    const std::string& Cookie::getExpires() const {
        return m_Expires;
    }

    void Cookie::setExpires(const std::string& expires) {
        m_Expires = expires;
    }

    const std::string& Cookie::getDomain() const {
        return m_Domain;
    }

    void Cookie::setDomain(const std::string& domain) {
        m_Domain = domain;
    }

    const std::string& Cookie::getPath() const {
        return m_Path;
    }

    void Cookie::setPath(const std::string& path) {
        m_Path = path;
    }

    bool Cookie::isSecure() const {
        return m_IsSecure;
    }

    void Cookie::setSecure(bool isSecure) {
        m_IsSecure = isSecure;
    }

    bool Cookie::isHttpOnly() const {
        return m_IsHttpOnly;
    }

    void Cookie::setHttpOnly(bool isHttpOnly) {
        m_IsHttpOnly = isHttpOnly;
    }


}