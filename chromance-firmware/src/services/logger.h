#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "../globals.h"
#include "timeService.h"
#include "config.h"

namespace Chromance
{
    enum class LogLevel : uint8_t
    {
        Trace = 0,
        Debug = 1,
        Information = 2,
        Warning = 3,
        Error = 4,
        Critical = 5,
        None = 6
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