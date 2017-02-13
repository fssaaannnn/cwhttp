
#include <curl/curl.h>
#include "ScopedCurl.h"

namespace cwhttp {

    ScopedCurl::ScopedCurl() {
        m_pCurl = curl_easy_init();
    }

    ScopedCurl::~ScopedCurl() {
        if (isValid()) {
            curl_easy_cleanup(m_pCurl);
        }
    }

    bool ScopedCurl::isValid() const {
        return m_pCurl != nullptr;
    }

    void* ScopedCurl::c_ptr() const {
        return m_pCurl;
    }

    void* ScopedCurl::operator()() const {
        return m_pCurl;
    }
}