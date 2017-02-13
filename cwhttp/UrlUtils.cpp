
#include <curl/curl.h>
#include "UrlUtils.h"
#include "ScopedCurl.h"

namespace cwhttp {

    UrlUtils::UrlUtils() {

    }

    std::string UrlUtils::escape(const std::string& s) {
        std::string escaped;
        ScopedCurl curl;
        if (!curl.isValid()) return escaped;

        char* pEscaped = curl_easy_escape(curl(), s.c_str(), static_cast<int>(s.length()));
        if (pEscaped) {
            escaped = pEscaped;
            curl_free(pEscaped);
        }
        return escaped;
    }

    std::string UrlUtils::unescape(const std::string& s) {
        std::string unescaped;
        ScopedCurl curl;
        if (!curl.isValid()) return unescaped;

        int outLength = 0;
        char* pUnescaped = curl_easy_unescape(curl(), s.c_str(), static_cast<int>(s.length()), &outLength);
        if (pUnescaped) {
            unescaped = pUnescaped;
            curl_free(pUnescaped);
        }
        return unescaped;
    }


}