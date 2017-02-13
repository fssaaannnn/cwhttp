
#ifndef CWHTTP_SCOPEDSLIST_H
#define CWHTTP_SCOPEDSLIST_H

#include <vector>
#include <string>
#include <functional>

struct curl_slist;

namespace cwhttp {

    class ScopedSList {
    private:
        curl_slist* m_pList;

    public:
        ScopedSList();
        ~ScopedSList();

        bool isValid() const;
        const curl_slist* c_ptr() const;
        curl_slist* operator()() const;
        bool append(const char* pItem);

        ScopedSList(const ScopedSList&) = delete;
        ScopedSList& operator=(const ScopedSList&) = delete;
    };

}

#endif //CWHTTP_SCOPEDSLIST_H
