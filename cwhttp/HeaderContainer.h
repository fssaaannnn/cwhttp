
#ifndef CWHTTP_HEADERCONTAINER_H
#define CWHTTP_HEADERCONTAINER_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <functional>

namespace cwhttp {

    class HeaderContainer {
    public:
        using NameValuePredicate = std::function<void(const std::string&, const std::string&)>;
        using LinePredicate = std::function<void(const std::string&)>;

    private:
        using HeaderMap = std::multimap<std::string, std::string>;
        HeaderMap m_Headers;

    public:
        HeaderContainer();

        static bool parseLine(const std::string& headerLine, std::string& name, std::string& value);
        static std::string buildLine(const char* pName, const char* pValue);
        std::size_t getSize() const;
        const char* get(const std::string& name) const;
        bool has(const std::string& name) const;
        void addLine(const std::string& headerLine);
        void addTrimmed(const std::string& name, const std::string& value);
        void addTrimmed(std::string&& name, std::string&& value);
        void addRaw(const std::string& name, const std::string& value);
        template<typename T>
        void addArbitraryRaw(const std::string& name, const T& value) {
            std::ostringstream ss;
            ss << value;
            addRaw(name, ss.str());
        }
        void setRaw(const std::string& name, const std::string& value);
        template<typename T>
        void setArbitraryRaw(const std::string& name, const T& value) {
            std::ostringstream ss;
            ss << value;
            remove(name);
            addRaw(name, ss.str());
        }
        void remove(const std::string& name);
        void iterateAll(const NameValuePredicate& predicate) const;
        void iterateAllLines(const LinePredicate& predicate) const;
        void reset();

    };

}

#endif //CWHTTP_HEADERCONTAINER_H
