#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include "../globals.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "logger.h"
#include "animationController.h"

namespace Chromance
{
    class MQTTClient
    {
        public:

            MQTTClient(Logger* logger, Config* config, AnimationController* animationController);
            ~MQTTClient();

            void Setup();
            void Loop();
            bool Publish();

        private:

            static const int32_t PublishQueueSize = 5;

            void Configure();
            void Callback(char* topic, byte* payload, uint32_t length);
            void Connect();
            void Subscribe();
            bool Publish(ChromanceState state);
            void PublishState(ChromanceState state);
            void PublishDeviceDiscovery();
            void PublishFPSSensorDiscovery();
            void PublishAnimationSpeedDiscoveries();
            void PublishRippleLifespanDiscoveries();
            void PublishRipplePulsePeriodDiscoveries();
            void PublishRippleDecayDiscoveries();
            void PublishAnimationSpeedDiscovery(AnimationType animationType);
            void PublishRippleLifespanDiscovery(AnimationType animationType);
            void PublishRipplePulsePeriodDiscovery(AnimationType animationType);
            void PublishRippleDecayDiscovery(AnimationType animationType);
            void PublishLightDiscovery();
            void PublishDocument(JsonDocument doc, const char* topic);
            String GetDiscoveryTopic(const char* entityType, String uniqueID);
            ChromanceState GetChromanceState();

            Logger* logger;
            Config* config;
            AnimationController* animationController;
            SemaphoreHandle_t semaphore;
            PublishRequest publishQueue[PublishQueueSize];
            char publishJsonBuffer[PublishJsonBufferSize];
            WiFiClient espClient;
            PubSubClient mqttClient;
            unsigned long lastReconnectAttempt;
            bool homeAssistantDiscoverySent;
            String deviceID;
    };
}

#endif