
#ifndef CWHTTP_METHOD_H
#define CWHTTP_METHOD_H

#include <cstdint>

namespace cwhttp {

    constexpr const char* const METHOD_GET = "GET";
    constexpr const char* const METHOD_POST = "POST";
    constexpr const char* const METHOD_PUT = "PUT";
    constexpr const char* const METHOD_PATCH = "PATCH";
    constexpr const char* const METHOD_DELETE = "DELETE";
    constexpr const char* const METHOD_HEAD = "HEAD";

    enum class Method : int {
        GET = 0,
        POST,
        PUT,
        PATCH,
        DEL,
        HEAD
    };

}

#endif //CWHTTP_METHOD_H
