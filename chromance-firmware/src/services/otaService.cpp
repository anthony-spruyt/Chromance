#include "otaService.h"

using namespace Chromance;

OTAService::OTAService(Logger* logger) :
    isUpdating(false)
{
    this->logger = logger;
}

void OTAService::Setup()
{
    this->logger->Info("Start OTA server");

    ArduinoOTA.setHostname(ChromanceNameLowercase.c_str());
    ArduinoOTA.setPassword(OTAPassword);

    ArduinoOTA.onStart
    (
        [this]()
        {
            this->isUpdating = true;
            this->logger->Info("Start OTA update");
        }
    );

    ArduinoOTA.onEnd
    (
        [this]()
        {
            this->logger->Info("OTA update completed");
            this->isUpdating = false;
        }
    );

    ArduinoOTA.onProgress
    (
        [this](uint32_t progress, uint32_t total)
        {
            this->logger->Debug("OTA update progress: " + String((float)progress / ((float)total / 100.0f)));
        }
    );

    ArduinoOTA.onError
    (
        [this](ota_error_t error)
        {
            if (error == OTA_AUTH_ERROR)
            {
                this->logger->Error("OTA update error: Auth failed");
            }
            else if (error == OTA_BEGIN_ERROR)
            {
                this->logger->Error("OTA update error: Begin failed");
            }
            else if (error == OTA_CONNECT_ERROR)
            {
                this->logger->Error("OTA update error: Connect failed");
            }
            else if (error == OTA_RECEIVE_ERROR)
            {
                this->logger->Error("OTA update error: Receive failed");
            }
            else if (error == OTA_END_ERROR)
            {
                this->logger->Error("OTA update error: End failed");
            }

            this->isUpdating = false;
    });

    ArduinoOTA.begin();
}

void OTAService::Loop()
{
    ArduinoOTA.handle();
}

bool OTAService::IsUpdating()
{
    return this->isUpdating;
}