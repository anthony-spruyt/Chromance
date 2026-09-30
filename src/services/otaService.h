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

            friend class HttpOtaRequestHandler;

            void SetupArduinoOTA();
            void SetupHttpOTA();
            void HandleHttpUpload(HTTPUpload& upload);
            void HandleHttpUploadComplete();

            Logger* logger;
            WebServer server;
            bool isUpdating;
            bool httpUploadAuthorized;
    };

    /**
     * Routes POST OTAHttpPath to OTAService. Unlike server.on(uri, method, fn, uploadFn), this does not accept
     * raw (non-multipart) bodies: WebServer would call the upload callback for those with no HTTPUpload, and
     * server.upload() would dereference a null pointer
    */
    class HttpOtaRequestHandler : public RequestHandler
    {
        public:

            HttpOtaRequestHandler(OTAService* otaService);

            bool canHandle(HTTPMethod method, String uri) override;
            bool canUpload(String uri) override;
            bool canRaw(String uri) override;
            bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) override;
            void upload(WebServer& server, String requestUri, HTTPUpload& upload) override;

        private:

            OTAService* otaService;
    };
}

#endif
