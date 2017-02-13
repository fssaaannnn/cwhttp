
#include "HeaderContainer.h"
#include "StringUtils.h"

namespace cwhttp {

    HeaderContainer::HeaderContainer() {

    }

    bool HeaderContainer::parseLine(const std::string& headerLine, std::string& name, std::string& value) {
        if (headerLine.length() == 0) return false;

        const std::size_t i = headerLine.find(':');
        if (i == std::string::npos) {
            return false;
        }
        std::string trimmedName = StringUtils::trimConst(std::string(headerLine, 0, i));
        std::string trimmedValue = std::string(headerLine, i);
        trimmedValue[0] = ' ';
        StringUtils::trim(trimmedValue);
        if (trimmedName.length() != 0 && trimmedValue.length() != 0) {
            name = trimmedName;
            value = trimmedValue;
            return true;
        }
        return false;
    }

    std::string HeaderContainer::buildLine(const char* pName, const char* pValue) {
        return std::string(pName).append(": ").append(pValue);
    }

    std::size_t HeaderContainer::getSize() const {
        return m_Headers.size();
    }

    const char* HeaderContainer::get(const std::string& name) const {
        if (name.length() == 0) return nullptr;

        const auto& it = m_Headers.find(name);
        return it != m_Headers.end() ? it->second.c_str() : nullptr;
    }

    bool HeaderContainer::has(const std::string& name) const {
        return get(name) != nullptr;
    }

    void HeaderContainer::addLine(const std::string& headerLine) {
        std::string name;
        std::string value;
        if (!parseLine(headerLine, name, value)) {
            return;
        }

        addTrimmed(std::move(name), std::move(value));
    }

    void HeaderContainer::addTrimmed(const std::string& name, const std::string& value) {
        m_Headers.emplace(name, value);
    }

    void HeaderContainer::addTrimmed(std::string&& name, std::string&& value) {
        m_Headers.emplace(name, value);
    }

    void HeaderContainer::addRaw(const std::string& name, const std::string& value) {
        std::string trimmedName(StringUtils::trimConst(std::string(name)));
        std::string trimmedValue(StringUtils::trimConst(std::string(value)));
        if (name.length() == 0 || value.length() == 0) return;

        addTrimmed(std::move(trimmedName), std::move(trimmedValue));
    }

    void HeaderContainer::setRaw(const std::string& name, const std::string& value) {
        remove(name);
        addRaw(name, value);
    }

    void HeaderContainer::remove(const std::string& name) {
        if (name.length() == 0) return;

        m_Headers.erase(name);
    }

    void HeaderContainer::iterateAll(const NameValuePredicate& predicate) const {
        for (const auto& header:m_Headers) {
            predicate(header.first, header.second);
        }
    }

    void HeaderContainer::iterateAllLines(const LinePredicate& predicate) const {
        iterateAll([&](const std::string& name, const std::string& value) {
            predicate(buildLine(name.c_str(), value.c_str()));
        });
    }

    void HeaderContainer::reset() {
        m_Headers.clear();
    }

}