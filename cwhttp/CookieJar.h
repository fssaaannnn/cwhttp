
#ifndef CWHTTP_COOKIEJAR_H
#define CWHTTP_COOKIEJAR_H

#include <string>
#include <vector>

namespace cwhttp {

    class Cookie;

    using CookieList = std::vector<Cookie>;

    class CookieJar {
    public:
        CookieJar();
        virtual ~CookieJar();

        virtual void saveFromResponse(const std::string& url, const CookieList& cookies) = 0;
        virtual void loadForRequest(const std::string& url, CookieList& cookies) const = 0;

    };

}

#endif //CWHTTP_COOKIEJAR_H
