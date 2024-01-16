#ifndef CONFIG_H_
#define CONFIG_H_

#include "../globals.h"
#include <Preferences.h>

namespace Chromance
{
    class Config
    {
        public:

            Config();

            void Setup();
            void SetLogLevel(uint8_t value);
            uint8_t GetLogLevel();
            void SetBrightness(uint8_t value);
            uint8_t GetBrightness();
            void SetSleeping(bool value);
            bool GetSleeping();
        
        private:

            Preferences preferences;
            uint8_t logLevel;
            uint8_t brightness;
            bool sleeping;
    };
}

#endif