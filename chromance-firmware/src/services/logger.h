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
            void Trace(const String& message);
            void Debug(const String& message);
            void Info(const String& message);
            void Warn(const String& message);
            void Error(const String& message);
            void Critical(const String& message);

        private:

            void Log(LogLevel logLevel, const String& message);
            const char* LogLevelToString(LogLevel logLevel);

            TimeService* timeService;
            Config* config;
            SemaphoreHandle_t semaphore;
    };
}

#endif