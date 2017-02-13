
#ifndef CWHTTP_REQUESTBODYBUILDER_H
#define CWHTTP_REQUESTBODYBUILDER_H

#include <map>
#include <string>
#include <memory>

namespace cwhttp {

    class RequestBody;

    class FormBodyBuilder {
    private:
        using FormEntries = std::map<std::string, std::string>;
        FormEntries m_Entries;

    public:
        FormBodyBuilder();

        FormBodyBuilder& add(const std::string& name, const std::string& value);
        FormBodyBuilder& addEncoded(const std::string& name, const std::string& value);
        FormBodyBuilder& addEncoded(std::string&& name, std::string&& value);
        std::unique_ptr<RequestBody> build() const;
        std::string buildString() const;
        void reset();

    };

}

#endif //CWHTTP_REQUESTBODYBUILDER_H
