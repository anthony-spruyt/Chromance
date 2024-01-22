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
            void Connect();

            Logger* logger;
    };
}

#endif