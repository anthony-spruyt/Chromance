#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "../globals.h"
#include "timeService.h"
#include "config.h"

namespace Chromance
{
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