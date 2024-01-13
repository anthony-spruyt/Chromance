#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include "../globals.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "logger.h"
#include "animationController.h"

#define REBOOT_COMMAND_CALLBACK_SIGNATURE std::function<void()>
#define WAKE_COMMAND_CALLBACK_SIGNATURE std::function<void()>
#define SLEEP_COMMAND_CALLBACK_SIGNATURE std::function<void()>
#define SPEED_COMMAND_CALLBACK_SIGNATURE std::function<void(uint8_t)>
#define PLAY_COMMAND_CALLBACK_SIGNATURE std::function<void(AnimationType)>
#define BRIGHTNESS_COMMAND_CALLBACK_SIGNATURE std::function<void(uint8_t)>
#define LOGLEVEL_COMMAND_CALLBACK_SIGNATURE std::function<void(LogLevel)>
#define OTHER_COMMAND_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, uint32_t)>

namespace Chromance
{
    struct ChromanceState
    {
        AnimationType animationType;
        AnimationStatus animationStatus;
        uint8_t brightness;
        uint8_t speed;
        LogLevel logLevel;
    };

    class MQTTClient
    {
        public:

            MQTTClient(Logger* logger, AnimationController* animationController);

            void Setup
            (
                REBOOT_COMMAND_CALLBACK_SIGNATURE rebootCommandCallback,
                WAKE_COMMAND_CALLBACK_SIGNATURE wakeCommandCallback,
                SLEEP_COMMAND_CALLBACK_SIGNATURE sleepCommandCallback,
                SPEED_COMMAND_CALLBACK_SIGNATURE speedCommandCallback,
                PLAY_COMMAND_CALLBACK_SIGNATURE playCommandCallback,
                BRIGHTNESS_COMMAND_CALLBACK_SIGNATURE brightnessCommandCallback,
                LOGLEVEL_COMMAND_CALLBACK_SIGNATURE logLevelCommandCallback,
                OTHER_COMMAND_CALLBACK_SIGNATURE otherCommandCallback
            );
            void Loop();
            void PublishState(ChromanceState state);
            void PublishFPS(uint32_t fps);
            

        private:

            void Configure();
            void Callback(char* topic, byte* payload, uint32_t length);
            void Connect();
            void Subscribe();
            void Publish();

            Logger* logger;
            AnimationController* animationController;
            REBOOT_COMMAND_CALLBACK_SIGNATURE rebootCommandCallback;
            WAKE_COMMAND_CALLBACK_SIGNATURE wakeCommandCallback;
            SLEEP_COMMAND_CALLBACK_SIGNATURE sleepCommandCallback;
            SPEED_COMMAND_CALLBACK_SIGNATURE speedCommandCallback;
            PLAY_COMMAND_CALLBACK_SIGNATURE playCommandCallback;
            BRIGHTNESS_COMMAND_CALLBACK_SIGNATURE brightnessCommandCallback;
            LOGLEVEL_COMMAND_CALLBACK_SIGNATURE logLevelCommandCallback;
            OTHER_COMMAND_CALLBACK_SIGNATURE otherCommandCallback;
            WiFiClient espClient;
            PubSubClient mqttClient;
            unsigned long lastReconnectAttempt;
    };
}

#endif