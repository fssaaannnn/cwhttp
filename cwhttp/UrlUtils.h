
#ifndef CWHTTP_URLUTILS_H
#define CWHTTP_URLUTILS_H

#include <string>

namespace cwhttp {

    class UrlUtils {
    private:
        UrlUtils();

    public:
        static std::string escape(const std::string& s);
        static std::string unescape(const std::string& s);

    };

}

#endif //CWHTTP_URLUTILS_H
