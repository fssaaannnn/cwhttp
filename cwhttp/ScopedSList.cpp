
#include <curl/curl.h>
#include "ScopedSList.h"

namespace cwhttp {

    ScopedSList::ScopedSList()
            : m_pList(nullptr) {
    }

    ScopedSList::~ScopedSList() {
        if (isValid()) {
            curl_slist_free_all(m_pList);
        }
    }

    bool ScopedSList::isValid() const {
        return m_pList != nullptr;
    }

    const curl_slist* ScopedSList::c_ptr() const {
        return m_pList;
    }

    curl_slist* ScopedSList::operator()() const {
        return m_pList;
    }

    bool ScopedSList::append(const char* pItem) {
        curl_slist* pNewList = curl_slist_append(m_pList, pItem);
        if (pNewList != nullptr) {
            m_pList = pNewList;
            return true;
        } else {
            return false;
        }
    }

}