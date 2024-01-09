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
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Trace)
    {
        return;
    }

    this->Log(LogLevel::Trace, message);
    
}

void Logger::Debug(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Debug)
    {
        return;
    }

    this->Log(LogLevel::Debug, message);
}

void Logger::Info(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Information)
    {
        return;
    }

    this->Log(LogLevel::Information, message);
}

void Logger::Warn(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Warning)
    {
        return;
    }

    this->Log(LogLevel::Warning, message);
}

void Logger::Error(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Error)
    {
        return;
    }

    this->Log(LogLevel::Error, message);
}

void Logger::Ciritical(String message)
{
    if (this->config->GetLogLevel() > (uint8_t)LogLevel::Critical)
    {
        return;
    }

    this->Log(LogLevel::Critical, message);
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
        case LogLevel::Trace:
            return "Trace";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Information:
            return "Information";
        case LogLevel::Warning:
            return "Warning";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Critical:
            return "Critical";
        default:
            return "Unknown";
    }
}