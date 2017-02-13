
#ifndef CWHTTP_SCOPEDCURL_H
#define CWHTTP_SCOPEDCURL_H

namespace cwhttp {

    class ScopedCurl {
    private:
        void* m_pCurl;

    public:
        ScopedCurl();
        ~ScopedCurl();

        bool isValid() const;
        void* c_ptr() const;
        void* operator()() const;
    };

}

#endif //CWHTTP_SCOPEDCURL_H
