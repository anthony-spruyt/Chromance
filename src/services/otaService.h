#ifndef OTA_SERVICE_H_
#define OTA_SERVICE_H_

#include "../globals.h"
#include "logger.h"
#include <WebServer.h>

namespace Chromance
{
    class OTAService
    {
        public:

            OTAService(Logger* logger);

            void Setup();
            void Loop();
            bool IsUpdating();

        private:

            void SetupArduinoOTA();
            void SetupHttpOTA();
            void HandleHttpUpload();
            void HandleHttpUploadComplete();

            Logger* logger;
            WebServer server;
            bool isUpdating;
            bool httpUploadAuthorized;
    };
}

#endif
