#ifndef OTA_SERVICE_H_
#define OTA_SERVICE_H_

#include "../globals.h"
#include "logger.h"

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

            Logger* logger;
            bool isUpdating;
    };
}

#endif