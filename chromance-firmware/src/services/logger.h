#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "../globals.h"
#include "timeService.h"
#include "config.h"

namespace Chromance
{
    enum LogLevel
    {
        LOG_LEVEL_TRACE = 0,
        LOG_LEVEL_DEBUG = 1,
        LOG_LEVEL_INFORMATION = 2,
        LOG_LEVEL_WARNING = 3,
        LOG_LEVEL_ERROR = 4,
        LOG_LEVEL_CRITICAL = 5,
        LOG_LEVEL_NONE = 6
    };

    class Logger
    {
        public:

            Logger(TimeService* timeService, Config* config);
            ~Logger();

            void Setup();
            void Trace(String message);
            void Debug(String message);
            void Info(String message);
            void Warn(String message);
            void Error(String message);
            void Ciritical(String message);

        private:

            void Log(LogLevel logLevel, String message);
            const char* LogLevelToString(LogLevel logLevel);

            TimeService* timeService;
            Config* config;
            SemaphoreHandle_t semaphore;
    };
}

#endif