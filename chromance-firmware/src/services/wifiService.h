#ifndef WIFI_SERVICE_H_
#define WIFI_SERVICE_H_

#include "../globals.h"
#include <WiFi.h>
#include "logger.h"

namespace Chromance
{
    class WiFiService
    {
        public:
    
            WiFiService(Logger* logger);

            void Setup();
            void Loop();

        private:

            void Configure();
            void RegisterEventHandlers();
            void Connect();
            void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
            void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
            void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

            Logger* logger;
    };
}

#endif