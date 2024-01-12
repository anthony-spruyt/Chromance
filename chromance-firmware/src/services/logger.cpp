#include "logger.h"

using namespace Chromance;

Logger::Logger(TimeService* timeService, Config* config)
{
    this->timeService = timeService;
    this->config = config;
    this->semaphore = xSemaphoreCreateMutex();
}

Logger::~Logger()
{
    vSemaphoreDelete(this->semaphore);
}

void Logger::Setup()
{
}

void Logger::Trace(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_TRACE)
    {
        return;
    }

    this->Log(LOG_LEVEL_TRACE, message);
    
}

void Logger::Debug(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_DEBUG)
    {
        return;
    }

    this->Log(LOG_LEVEL_DEBUG, message);
}

void Logger::Info(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_INFORMATION)
    {
        return;
    }

    this->Log(LOG_LEVEL_INFORMATION, message);
}

void Logger::Warn(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_WARNING)
    {
        return;
    }

    this->Log(LOG_LEVEL_WARNING, message);
}

void Logger::Error(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_ERROR)
    {
        return;
    }

    this->Log(LOG_LEVEL_ERROR, message);
}

void Logger::Ciritical(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LOG_LEVEL_CRITICAL)
    {
        return;
    }

    this->Log(LOG_LEVEL_CRITICAL, message);
}

void Logger::Log(LogLevel logLevel, String message)
{
#ifdef SERIAL_ENABLED
    String timestamp = this->timeService->Now();
    const char* logLevelString = this->LogLevelToString(logLevel);

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        Serial.printf("(%s)[%s] %s\n", timestamp.c_str(), logLevelString, message.c_str());
        
        xSemaphoreGive(this->semaphore);
    }
#endif
}

const char* Logger::LogLevelToString(LogLevel logLevel)
{
    switch (logLevel)
    {
        case LOG_LEVEL_TRACE:
            return "Trace";
        case LOG_LEVEL_DEBUG:
            return "Debug";
        case LOG_LEVEL_INFORMATION:
            return "Information";
        case LOG_LEVEL_WARNING:
            return "Warning";
        case LOG_LEVEL_ERROR:
            return "Error";
        case LOG_LEVEL_CRITICAL:
            return "Critical";
        default:
            return "Unknown";
    }
}