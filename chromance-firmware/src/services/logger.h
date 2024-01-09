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

            /// @brief Initialize the logging service
            void Setup();
            /// @brief Write a new trace log message to the registered logging providers
            /// @param message The trace message to log
            void Trace(String message);
            /// @brief Write a new debug log message to the registered logging providers
            /// @param message The debug message to log
            void Debug(String message);
            /// @brief Write a new information log message to the registered logging providers
            /// @param message The information message to log
            void Info(String message);
            /// @brief Write a new warning log message to the registered logging providers
            /// @param message The warning message to log
            void Warn(String message);
            /// @brief Write a new error log message to the registered logging providers
            /// @param message The error message to log
            void Error(String message);
            /// @brief Write a new critical log message to the registered logging providers
            /// @param message The critical message to log
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