
#include <cstdio>
#include <cstdarg>
#include <mutex>
#include "Logger.h"

#ifdef __ANDROID__
#include <android/log.h>
#define LOG_TAG "libcwlog"
#endif

namespace cwlog {

    static std::mutex s_printMutex;

    void printLog(Level level, const char* pFormat, ...) {

#ifdef __ANDROID__
        android_LogPriority priority;
        switch (level){
        case Level::L_VERBOSE:
            priority = ANDROID_LOG_VERBOSE;
            break;
        case Level::L_DEBUG:
            priority = ANDROID_LOG_DEBUG;
            break;
        case Level::L_INFO:
            priority = ANDROID_LOG_INFO;
            break;
        case Level::L_WARNING:
            priority = ANDROID_LOG_WARN;
            break;
        case Level::L_ERROR:
            priority = ANDROID_LOG_ERROR;
            break;
        default:
            priority = ANDROID_LOG_VERBOSE;
            break;
        }

        va_list argList;
        va_start(argList, pFormat);
        __android_log_vprint(priority, LOG_TAG, pFormat, argList);
        va_end(argList);
#else
        std::lock_guard<std::mutex> lg(s_printMutex);
        va_list argList;
        va_start(argList, pFormat);
        vprintf(pFormat, argList);
        va_end(argList);
        printf("\n");
#endif

    }


}