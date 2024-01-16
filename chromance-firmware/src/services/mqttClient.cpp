#include "mqttClient.h"

using namespace Chromance;

MQTTClient::MQTTClient(Logger* logger, AnimationController* animationController) : 
    mqttClient(espClient), 
    lastReconnectAttempt(0),
    homeAssistantDiscoverySent(false)
{
    this->animationController = animationController;
    this->logger = logger;

    this->semaphore = xSemaphoreCreateMutex();

    for (int32_t i = 0; i < PublishQueueSize; i++)
    {
        this->publishQueue[i].publishedAt = 0U;
    }
}

MQTTClient::~MQTTClient()
{
    vSemaphoreDelete(this->semaphore);
}

void MQTTClient::Setup()
{   
    this->Configure();
    this->Connect();
}

void MQTTClient::Callback(char* topic, byte* payload, uint32_t length)
{
    String messageTopic = String(topic);

    this->logger->Debug("MQTT message arrived in topic: " + messageTopic);

    if (messageTopic == HomeAssistantStatusTopic)
    {
        String haStatus = String((char*)payload, length);

        if (haStatus == "online")
        {
            this->PublishDeviceDiscovery();
        }
    }
    else if (messageTopic == MQTTCommandTopic)
    {
        bool publishState = false;
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, (const char*)payload, length);

        if (error)
        {
            this->logger->Error("Deserialization error: " + String(error.c_str()));

            return;
        }

        if (!doc["reboot"].isNull())
        {
            ESP.restart();

            return;
        }

        ChromanceState chromanceState;

        chromanceState.animationStatus = this->animationController->GetAnimationStatus();
        chromanceState.animationType = this->animationController->GetAnimationType();
        chromanceState.brightness = this->animationController->GetBrightness();
        chromanceState.fps = this->animationController->GetFPS();

        if (!doc["state"].isNull())
        {
            const char* state = doc["state"];

            if
            (
                strcmp(state, "ON") == 0 && 
                chromanceState.animationStatus != ANIMATION_STATUS_PLAYING &&
                chromanceState.animationStatus != ANIMATION_STATUS_WAKING_UP
            )
            {
                this->animationController->Wake();
                chromanceState.animationStatus = ANIMATION_STATUS_WAKING_UP;
                publishState = true;
            }
            else if
            (
                strcmp(state, "OFF") == 0 && 
                chromanceState.animationStatus != ANIMATION_STATUS_GOING_TO_SLEEP &&
                chromanceState.animationStatus != ANIMATION_STATUS_SLEEPING
            )
            {
                this->animationController->Sleep();
                chromanceState.animationStatus = ANIMATION_STATUS_GOING_TO_SLEEP;
                publishState = true;
            }
        }
        
        if (!doc["brightness"].isNull())
        {
            int32_t brightness = doc["brightness"];

            if (chromanceState.brightness != brightness)
            {
                if (brightness > 255)
                {
                    brightness = 255;
                }

                if (brightness < 0)
                {
                    brightness = 0;
                }

                this->animationController->SetBrightness(brightness);
                chromanceState.brightness = brightness;
                publishState = true;
            }
        }

        if (!doc["effect"].isNull())
        {
            const char* effect = doc["effect"];
            AnimationType currentAnimationType = this->animationController->GetAnimationType();

            if (strcmp(effect, "Center Pulse") == 0 && currentAnimationType != ANIMATION_TYPE_CENTER_PULSE)
            {
                this->animationController->Play(ANIMATION_TYPE_CENTER_PULSE);
                chromanceState.animationType = ANIMATION_TYPE_CENTER_PULSE;
                publishState = true;
            }
            else if (strcmp(effect, "Cube Pulse") == 0 && currentAnimationType != ANIMATION_TYPE_CUBE_PULSE)
            {
                this->animationController->Play(ANIMATION_TYPE_CUBE_PULSE);
                chromanceState.animationType = ANIMATION_TYPE_CUBE_PULSE;
                publishState = true;
            }
            else if (strcmp(effect, "Pulse") == 0 && currentAnimationType != ANIMATION_TYPE_PULSE)
            {
                this->animationController->Play(ANIMATION_TYPE_PULSE);
                chromanceState.animationType = ANIMATION_TYPE_PULSE;
                publishState = true;
            }
            else if (strcmp(effect, "Rainbow Beat") == 0 && currentAnimationType != ANIMATION_TYPE_RAINBOW_BEAT)
            {
                this->animationController->Play(ANIMATION_TYPE_RAINBOW_BEAT);
                chromanceState.animationType = ANIMATION_TYPE_RAINBOW_BEAT;
                publishState = true;
            }
            else if (strcmp(effect, "Rainbow March") == 0 && currentAnimationType != ANIMATION_TYPE_RAINBOW_MARCH)
            {
                this->animationController->Play(ANIMATION_TYPE_RAINBOW_MARCH);
                chromanceState.animationType = ANIMATION_TYPE_RAINBOW_MARCH;
                publishState = true;
            }
            else if (strcmp(effect, "Random") == 0 && currentAnimationType != ANIMATION_TYPE_RANDOM_ANIMATION)
            {
                this->animationController->Play(ANIMATION_TYPE_RANDOM_ANIMATION);
                chromanceState.animationType = ANIMATION_TYPE_RANDOM_ANIMATION;
                publishState = true;
            }
            else if (strcmp(effect, "Random Pulse") == 0 && currentAnimationType != ANIMATION_TYPE_RANDOM_PULSE)
            {
                this->animationController->Play(ANIMATION_TYPE_RANDOM_PULSE);
                chromanceState.animationType = ANIMATION_TYPE_RANDOM_PULSE;
                publishState = true;
            }
            else if (strcmp(effect, "Star Burst Pulse") == 0 && currentAnimationType != ANIMATION_TYPE_STAR_BURST_PULSE)
            {
                this->animationController->Play(ANIMATION_TYPE_STAR_BURST_PULSE);
                chromanceState.animationType = ANIMATION_TYPE_STAR_BURST_PULSE;
                publishState = true;
            }
            else if (strcmp(effect, "Strip Test") == 0 && currentAnimationType != ANIMATION_TYPE_STRIP_TEST)
            {
                this->animationController->Play(ANIMATION_TYPE_STRIP_TEST);
                chromanceState.animationType = ANIMATION_TYPE_STRIP_TEST;
                publishState = true;
            }
            else if (strcmp(effect, "Around the World") == 0 && currentAnimationType != ANIMATION_TYPE_AROUND_THE_WORLD)
            {
                this->animationController->Play(ANIMATION_TYPE_AROUND_THE_WORLD);
                chromanceState.animationType = ANIMATION_TYPE_AROUND_THE_WORLD;
                publishState = true;
            }
        }

        if (publishState)
        {
            this->Publish(chromanceState);
        }
    }
}

void MQTTClient::Loop()
{
    unsigned long now = millis();

    if (!this->mqttClient.connected())
    {
        if (now - this->lastReconnectAttempt > 5000)
        {
            this->lastReconnectAttempt = now;
            this->Connect();
        }
    }
    else
    {
        this->mqttClient.loop();

        if (!this->homeAssistantDiscoverySent)
        {
            this->homeAssistantDiscoverySent = true;

            this->PublishDeviceDiscovery();

            return;
        }

        bool shouldPublishState = false;
        ChromanceState state;

        if (xSemaphoreTake(this->semaphore, 0U) == pdTRUE)
        {
            for (int32_t i = PublishQueueSize - 1; i >= 0; i--)
            {
                if (this->publishQueue[i].publishedAt > now)
                {
                    this->logger->Debug("Select publish request queue item: " + String(i));
                    this->publishQueue[i].publishedAt = now;
                    state = this->publishQueue[i].state;
                    shouldPublishState = true;

                    break;
                }
            }

            xSemaphoreGive(this->semaphore);
        }

        if (shouldPublishState)
        {
            this->PublishState(state);
        }
    }
}

bool MQTTClient::Publish(ChromanceState state)
{
    bool queued = false;

    this->logger->Debug("Publish chromance state");

    if (this->mqttClient.connected())
    {
        unsigned long now = millis();

        if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
        {
            for (int32_t i = 0; i < PublishQueueSize; i++)
            {
                if (this->publishQueue[i].publishedAt < now)
                {
                    this->logger->Debug("Add chromance state to publish request queue: " + String(i));
                    this->publishQueue[i].state = state;
                    this->publishQueue[i].publishedAt = UINT32_MAX;
                    queued = true;

                    break;
                }
            }

            xSemaphoreGive(this->semaphore);
        }
    }

    return queued;
}

void MQTTClient::Configure()
{
    this->logger->Info("Configure MQTT");
    this->mqttClient.setBufferSize(PublishJSONBufferSize);
    this->mqttClient.setServer(MQTTBroker, MQTTPort);
    this->mqttClient.setCallback
    (
        [this](char* topic, byte* payload, uint32_t length)
        {
            this->Callback(topic, payload, length);
        }
    );
    this->mqttClient.setKeepAlive(MQTTKeepAlive);
}

void MQTTClient::Connect()
{
    if (this->mqttClient.connected() || !WiFi.isConnected())
    {
        return;
    }

    this->logger->Debug("MQTT connecting...");

    String deviceID = this->GetDeviceID();
    uint8_t willQOS = 1U;

    if (this->mqttClient.connect(deviceID.c_str(), MQTTUsername, MQTTPassword, MQTTStateTopic.c_str(), willQOS, false, "{\"availability\": \"0\", \"state\": \"None\", \"fps\": 0}"))
    {
        this->logger->Debug("MQTT connected");
        this->Subscribe();
        this->lastReconnectAttempt = 0;

        return;
    }

    this->logger->Error("MQTT connect failed: " + String(this->mqttClient.state()));
}

void MQTTClient::Subscribe()
{
    uint8_t qos = 1U;
    this->mqttClient.subscribe(MQTTCommandTopic.c_str(), qos);
    this->logger->Debug("MQTT topic subscribed: " + MQTTCommandTopic);
    this->mqttClient.subscribe(HomeAssistantStatusTopic.c_str(), qos);
    this->logger->Debug("MQTT topic subscribed: " + HomeAssistantStatusTopic);
}

void MQTTClient::PublishState(ChromanceState state)
{
    this->logger->Debug("Publish to state MQTT topic");

    JsonDocument doc;

    doc["animationStatus"] = state.animationStatus;
    doc["animationType"] = state.animationType;
    doc["brightness"] = state.brightness;
    doc["fps"] = state.fps;
    doc["availability"] = "1";
    doc["state"] = state.animationStatus != ANIMATION_STATUS_PLAYING && state.animationStatus != ANIMATION_STATUS_WAKING_UP ? "OFF" : "ON";
    
    if (state.animationType == ANIMATION_TYPE_CENTER_PULSE)
    {
        doc["effect"] = "Center Pulse";
    }
    else if (state.animationType == ANIMATION_TYPE_CUBE_PULSE)
    {
        doc["effect"] = "Cube Pulse";
    }
    else if (state.animationType == ANIMATION_TYPE_PULSE)
    {
        doc["effect"] = "Pulse";
    }
    else if (state.animationType == ANIMATION_TYPE_RAINBOW_BEAT)
    {
        doc["effect"] = "Rainbow Beat";
    }
    else if (state.animationType == ANIMATION_TYPE_RAINBOW_MARCH)
    {
        doc["effect"] = "Rainbow March";
    }
    else if (state.animationType == ANIMATION_TYPE_RANDOM_ANIMATION)
    {
        doc["effect"] = "Random";
    }
    else if (state.animationType == ANIMATION_TYPE_RANDOM_PULSE)
    {
        doc["effect"] = "Random Pulse";
    }
    else if (state.animationType == ANIMATION_TYPE_STAR_BURST_PULSE)
    {
        doc["effect"] = "Star Burst Pulse";
    }
    else if (state.animationType == ANIMATION_TYPE_STRIP_TEST)
    {
        doc["effect"] = "Strip Test";
    }
    else if (state.animationType == ANIMATION_TYPE_AROUND_THE_WORLD)
    {
        doc["effect"] = "Around the World";
    }
    
    doc.shrinkToFit();

    size_t n = serializeJson(doc, this->publishJSONBuffer, PublishJSONBufferSize);

    this->mqttClient.publish(MQTTStateTopic.c_str(), (const uint8_t*)&this->publishJSONBuffer, n, false);
}

void MQTTClient::PublishDeviceDiscovery()
{   
    this->logger->Debug("Publish to home assistant discovery MQTT topic");

    String deviceID = this->GetDeviceID();

    this->PublishFPSSensorDiscovery(deviceID);
    this->PublishLightDiscovery(deviceID);
}

void MQTTClient::PublishFPSSensorDiscovery(String deviceID)
{
    String uniqueID = "chrfps1";
    String topic = this->GetDiscoveryTopic("sensor", deviceID, uniqueID);
    JsonDocument doc;

    doc["~"] = MQTTBaseTopic;
    doc["avty_t"] = MQTTStateRoute;
    doc["avty_tpl"] = "{{ value_json.availability }}";
    doc["pl_avail"] = "1";
    doc["pl_not_avail"] = "0";
    JsonObject device = doc["dev"].to<JsonObject>();
    device["name"] = ChromanceNameCapitalized;
    device["mf"] = "Voidstar Lab";
    device["mdl"] = ChromanceNameCapitalized;
    device["sw"] = "v1.0.0";
    device["hw"] = "v1.0.0";
    device["sa"] = "Study";
    JsonArray ids = device["ids"].to<JsonArray>();
    ids.add("chr");

    doc["name"] = "FPS";
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["stat_cla"] = "measurement";
    doc["value_template"] = "{{ value_json.fps }}";
    doc["dev_cla"] = "frequency";
    doc["unit_of_meas"] = "Hz";
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishLightDiscovery(String deviceID)
{
    String uniqueID = "chrlight1";
    String topic = this->GetDiscoveryTopic("light", deviceID, uniqueID);
    JsonDocument doc;

    doc["~"] = MQTTBaseTopic;
    doc["avty_t"] = MQTTStateRoute;
    doc["avty_tpl"] = "{{ value_json.availability }}";
    doc["pl_avail"] = "1";
    doc["pl_not_avail"] = "0";
    JsonObject device = doc["dev"].to<JsonObject>();
    device["name"] = ChromanceNameCapitalized;
    device["mf"] = "Voidstar Lab";
    device["mdl"] = ChromanceNameCapitalized;
    device["sw"] = "v1.0.0";
    device["hw"] = "v1.0.0";
    device["sa"] = "Study";
    JsonArray ids = device["ids"].to<JsonArray>();
    ids.add("chr");

    doc["name"] = "LED strips";
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["schema"] = "json";
    doc["color_temp"] = false;
    doc["brightness"] = true;
    doc["effect"] = true;
    JsonArray effects = doc["effect_list"].to<JsonArray>();
    effects.add("Center Pulse");
    effects.add("Cube Pulse");
    effects.add("Pulse");
    effects.add("Rainbow Beat");
    effects.add("Rainbow March");
    effects.add("Random");
    effects.add("Random Pulse");
    effects.add("Star Burst Pulse");
    effects.add("Strip Test");
    effects.add("Around the World");
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishDocument(JsonDocument doc, const char* topic)
{
    doc.shrinkToFit();
    size_t n = serializeJson(doc, this->publishJSONBuffer, PublishJSONBufferSize);
    this->mqttClient.publish(topic, (const uint8_t*)&this->publishJSONBuffer, n, false);
}

String MQTTClient::GetDeviceID()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String deviceID = String(mac[0], HEX) + String(mac[1], HEX) + String(mac[2], HEX) + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);

    return deviceID;
}

String MQTTClient::GetDiscoveryTopic(String entityType, String deviceID, String uniqueID)
{
    return "homeassistant/" + entityType + "/" + ChromanceNameLowercase + "/" + deviceID + uniqueID + "/config";
}