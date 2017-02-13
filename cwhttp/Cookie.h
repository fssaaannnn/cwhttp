
#ifndef CWHTTP_COOKIE_H
#define CWHTTP_COOKIE_H

#include <string>

namespace cwhttp {

    class Cookie {
    private:
        std::string m_Name;
        std::string m_Value;
        std::string m_Expires;
        std::string m_Domain;
        std::string m_Path;
        bool m_IsSecure;
        bool m_IsHttpOnly;

    public:
        Cookie();
        Cookie(const std::string& name, const std::string& value);
        Cookie(const std::string& setCookieString);
        Cookie(const Cookie& other) = default;
        Cookie(Cookie&&) = default;

        bool isValid() const;
        const std::string& getName() const;
        void setName(const std::string& name);
        const std::string& getValue() const;
        void setValue(const std::string& value);
        const std::string& getExpires() const;
        void setExpires(const std::string& expires);
        const std::string& getDomain() const;
        void setDomain(const std::string& domain);
        const std::string& getPath() const;
        void setPath(const std::string& path);
        bool isSecure() const;
        void setSecure(bool isSecure);
        bool isHttpOnly() const;
        void setHttpOnly(bool isHttpOnly);

    };

}

#endif //CWHTTP_COOKIE_H
