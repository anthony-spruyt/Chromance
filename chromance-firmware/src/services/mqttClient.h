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
    struct ChromanceState
    {
        AnimationType animationType;
        AnimationStatus animationStatus;
        uint8_t brightness;
        uint32_t fps;
    };

    struct PublishRequest
    {
        unsigned long publishedAt;
        ChromanceState state;
    };

    class MQTTClient
    {
        public:

            MQTTClient(Logger* logger, AnimationController* animationController);
            ~MQTTClient();

            void Setup();
            void Loop();
            bool Publish(ChromanceState state);

        private:

            static const int32_t PublishQueueSize = 5;

            void Configure();
            void Callback(char* topic, byte* payload, uint32_t length);
            void Connect();
            void Subscribe();
            void PublishState(ChromanceState state);
            void PublishDeviceDiscovery();
            void PublishFPSSensorDiscovery(String deviceID);
            void PublishLightDiscovery(String deviceID);
            void PublishDocument(JsonDocument doc, const char* topic);
            String GetDeviceID();
            String GetDiscoveryTopic(String entityType, String deviceID, String uniqueID);

            Logger* logger;
            AnimationController* animationController;
            SemaphoreHandle_t semaphore;
            PublishRequest publishQueue[PublishQueueSize];
            char publishJSONBuffer[PublishJSONBufferSize];
            WiFiClient espClient;
            PubSubClient mqttClient;
            unsigned long lastReconnectAttempt;
            bool homeAssistantDiscoverySent;
    };
}

#endif