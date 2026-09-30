#include "otaService.h"
#include <Update.h>

using namespace Chromance;

OTAService::OTAService(Logger* logger) :
    server(OTAHttpPort),
    isUpdating(false),
    httpUploadAuthorized(false)
{
    this->logger = logger;
}

void OTAService::Setup()
{
    this->SetupArduinoOTA();
    this->SetupHttpOTA();
}

void OTAService::Loop()
{
    ArduinoOTA.handle();
    this->server.handleClient();
}

bool OTAService::IsUpdating()
{
    return this->isUpdating;
}

void OTAService::SetupArduinoOTA()
{
    ArduinoOTA.setHostname(ChromanceNameLowercase);
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

/**
 * Firmware upload over plain HTTP: POST a multipart form with the image in a "firmware" field to OTAHttpPath.
 * Every connection is made by the uploader, so unlike ArduinoOTA (where the device connects back to the
 * uploader) this works from behind NAT, e.g. a dev container
*/
void OTAService::SetupHttpOTA()
{
    this->server.on
    (
        OTAHttpPath,
        HTTP_POST,
        [this]()
        {
            this->HandleHttpUploadComplete();
        },
        [this]()
        {
            this->HandleHttpUpload();
        }
    );

    this->server.begin();
}

void OTAService::HandleHttpUpload()
{
    HTTPUpload& upload = this->server.upload();

    switch (upload.status)
    {
        case UPLOAD_FILE_START:
            // Headers are parsed before the body, so credentials can be checked before anything is written
            this->httpUploadAuthorized = this->server.authenticate(OTAHttpUsername, OTAPassword);

            if (!this->httpUploadAuthorized)
            {
                this->logger->Warn("HTTP OTA update rejected: Auth failed");

                return;
            }

            this->isUpdating = true;
            this->logger->Info("Start HTTP OTA update");

            if (!Update.begin(UPDATE_SIZE_UNKNOWN))
            {
                this->logger->Error("HTTP OTA update error: " + String(Update.errorString()));
            }
            break;
        case UPLOAD_FILE_WRITE:
            if (this->httpUploadAuthorized && !Update.hasError() && Update.write(upload.buf, upload.currentSize) != upload.currentSize)
            {
                this->logger->Error("HTTP OTA update error: " + String(Update.errorString()));
            }
            break;
        case UPLOAD_FILE_END:
            if (this->httpUploadAuthorized && Update.end(true))
            {
                this->logger->Info("HTTP OTA update received " + String(upload.totalSize) + " bytes");
            }
            break;
        case UPLOAD_FILE_ABORTED:
            if (this->httpUploadAuthorized)
            {
                Update.abort();
                this->logger->Error("HTTP OTA update error: Upload aborted");
            }

            this->isUpdating = false;
            break;
    }
}

void OTAService::HandleHttpUploadComplete()
{
    this->httpUploadAuthorized = false;

    if (!this->server.authenticate(OTAHttpUsername, OTAPassword))
    {
        this->server.requestAuthentication();

        return;
    }

    if (Update.hasError() || !Update.isFinished())
    {
        String error = Update.hasError() ? String(Update.errorString()) : String("No firmware received");

        this->logger->Error("HTTP OTA update failed: " + error);
        this->server.send(500, "text/plain", "Update failed: " + error + "\n");
        this->isUpdating = false;

        return;
    }

    this->logger->Info("HTTP OTA update completed, rebooting");
    this->server.send(200, "text/plain", "Update complete, rebooting\n");

    // Give the response time to reach the client before restarting
    delay(500);
    ESP.restart();
}
