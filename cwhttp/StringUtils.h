
#ifndef CWHTTP_STRINGUTILS_H
#define CWHTTP_STRINGUTILS_H

#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <vector>

namespace cwhttp {

    class StringUtils {
    private:
        StringUtils();

    public:

        using TrimPredicate = std::function<bool(int)>;

        static bool isSpaceOrCtl(int c) {
            return std::isspace(c) || std::iscntrl(c);
        }

        static std::string& trimStart(std::string& s, const TrimPredicate& predicate = isSpaceOrCtl) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(predicate)));
            return s;
        }

        static std::string& trimEnd(std::string& s, const TrimPredicate& predicate = isSpaceOrCtl) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(predicate)).base(), s.end());
            return s;
        }

        static std::string& trim(std::string& s, const TrimPredicate& predicate = isSpaceOrCtl) {
            return trimStart(trimEnd(s, predicate), predicate);
        }

        static std::string trimConst(const std::string& s, const TrimPredicate& predicate = isSpaceOrCtl) {
            std::string out(s);
            return trimStart(trimEnd(out, predicate), predicate);
        }

        static bool hasPrefix(const std::string& s, const std::string& prefix) {
            return s.size() >= prefix.size() &&
                    std::equal(prefix.begin(), prefix.end(), s.begin());
        }

        static bool hasSuffix(const std::string& s, const std::string& suffix) {
            return s.size() >= suffix.size() &&
                    s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
        }

        using SplitPredicate = std::function<bool(std::string&)>;

        static bool shallInsert(std::string& i) {
            return true;
        }

        static std::vector<std::string>& split(const std::string& s, char dlm, std::vector<std::string>& outs,
                const SplitPredicate& predicate = shallInsert) {
            std::istringstream ss(s);
            std::string item;
            while (std::getline(ss, item, dlm)) {
                if (predicate(item)) {
                    outs.emplace_back(item);
                }
            }
            return outs;
        }

        static bool equalCharIgnoreCase(char l, char r) {
            return toupper(l) == toupper(r);
        }

        static bool equalIgnoreCase(const std::string& l, const std::string& r) {
            return l.size() == r.size() && std::equal(l.begin(), l.end(), r.begin(), equalCharIgnoreCase);
        }

        static bool equal(const std::string& l, const std::string& r) {
            return l.compare(r) == 0;
        }

    };

}

#endif //CWHTTP_STRINGUTILS_H
