
#ifndef CWLOG_LOGGER_H
#define CWLOG_LOGGER_H

#define LOGV(...) (cwlog::printLog(cwlog::Level::L_VERBOSE, __VA_ARGS__))
#define LOGD(...) (cwlog::printLog(cwlog::Level::L_DEBUG, __VA_ARGS__))
#define LOGI(...) (cwlog::printLog(cwlog::Level::L_INFO, __VA_ARGS__))
#define LOGW(...) (cwlog::printLog(cwlog::Level::L_WARNING, __VA_ARGS__))
#define LOGE(...) (cwlog::printLog(cwlog::Level::L_ERROR, __VA_ARGS__))

namespace cwlog {

    enum class Level : int {
        L_VERBOSE = 0,
        L_DEBUG,
        L_INFO,
        L_WARNING,
        L_ERROR
    };

    void printLog(Level level, const char* pFormat, ...);

}

#endif //CWLOG_LOGGER_H
