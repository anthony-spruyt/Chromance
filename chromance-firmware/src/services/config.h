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

            // Initialize the config service
            void Setup();
            /**
             * Set the log level
             * @param value The new log level
            */
            void SetLogLevel(uint8_t value);
            /**
             * Get the log level
             * @return The current log level
            */
            uint8_t GetLogLevel();
            /**
             * Set the global brightness scaling
             * @param value The new global brightness scaling [0,255]
            */
            void SetBrightness(uint8_t value);
            /**
             * Get the global brightness scaling
             * @return The current global brightness scaling [0,255]
            */
            uint8_t GetBrightness();
        
        private:

            Preferences preferences;
            uint8_t logLevel;
            uint8_t brightness;
    };
}

#endif