#include "mqttClient.h"

using namespace Chromance;

MQTTClient::MQTTClient(Logger* logger, Config* config, AnimationController* animationController) : 
    mqttClient(espClient), 
    lastReconnectAttempt(0),
    homeAssistantDiscoverySent(false),
    deviceID("")
{
    this->animationController = animationController;
    this->config = config;
    this->logger = logger;

    this->semaphore = xSemaphoreCreateMutex();

    for (int32_t i = 0; i < PublishQueueSize; i++)
    {
        this->publishQueue[i].state.animationStatus = ANIMATION_STATUS_SLEEPING;
        this->publishQueue[i].state.animationType = ANIMATION_TYPE_RANDOM_ANIMATION;
        this->publishQueue[i].state.brightness = 255U;
        this->publishQueue[i].state.fps = 0U;
        this->publishQueue[i].publishedAt = 0U;
    }

    this->deviceID.reserve(12);
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

        ChromanceState chromanceState = this->GetChromanceState();

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

        AnimationType animationType = ANIMATION_TYPE_RANDOM_ANIMATION;

        if (!doc["effect"].isNull())
        {
            const char* effect = doc["effect"];
            AnimationType currentAnimationType = this->animationController->GetAnimationType();

            if (strcmp(effect, "Random") == 0 && currentAnimationType != ANIMATION_TYPE_RANDOM_ANIMATION)
            {
                this->animationController->Play(ANIMATION_TYPE_RANDOM_ANIMATION);
                chromanceState.animationType = ANIMATION_TYPE_RANDOM_ANIMATION;
                publishState = true;
            }
            else
            {
                for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
                {
                    animationType = (AnimationType)i;

                    if
                    (
                        strcmp(effect, this->animationController->GetAnimation(animationType)->GetName()) == 0 && 
                        currentAnimationType != animationType
                    )
                    {
                        this->animationController->Play(animationType);
                        chromanceState.animationType = animationType;
                        publishState = true;

                        break;
                    }
                }
            }
        }

        String animationSpeedKey;
        float animationSpeed;
        float currentAnimationSpeed;

        String rippleDecayKey;
        uint8_t rippleDecay;
        uint8_t currentRippleDecay;

        String rippleLifespanKey;
        unsigned long rippleLifespan;
        unsigned long currentRippleLifespan;

        String ripplePulsePeriodKey;
        unsigned long ripplePulsePeriod;
        unsigned long currentRipplePulsePeriod;

        for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
        {
            animationType = (AnimationType)i;

            animationSpeedKey = this->config->GetAnimationSpeedKey(animationType);

            if (!doc[animationSpeedKey].isNull())
            {
                animationSpeed = doc[animationSpeedKey];
                currentAnimationSpeed = this->config->GetAnimationSpeed(animationType);

                if (currentAnimationSpeed != animationSpeed)
                {
                    this->config->SetAnimationSpeed(animationType, animationSpeed);
                    publishState = true;
                }
            }

            rippleDecayKey = this->config->GetRippleDecayKey(animationType);

            if (!doc[rippleDecayKey].isNull())
            {
                rippleDecay = doc[rippleDecayKey];
                currentRippleDecay = this->config->GetRippleDecay(animationType);

                if (currentRippleDecay != rippleDecay)
                {
                    this->config->SetRippleDecay(animationType, rippleDecay);
                    publishState = true;
                }
            }

            rippleLifespanKey = this->config->GetRippleLifespanKey(animationType);

            if (!doc[rippleLifespanKey].isNull())
            {
                rippleLifespan = doc[rippleLifespanKey];
                currentRippleLifespan = this->config->GetRippleLifespan(animationType);

                if (currentRippleLifespan != rippleLifespan)
                {
                    this->config->SetRippleLifespan(animationType, rippleLifespan);
                    publishState = true;
                }
            }

            ripplePulsePeriodKey = this->config->GetRipplePulsePeriodKey(animationType);

            if (!doc[ripplePulsePeriodKey].isNull())
            {
                ripplePulsePeriod = doc[ripplePulsePeriodKey];
                currentRipplePulsePeriod = this->config->GetRipplePulsePeriod(animationType);

                if (currentRipplePulsePeriod != ripplePulsePeriod)
                {
                    this->config->SetRipplePulsePeriod(animationType, ripplePulsePeriod);
                    publishState = true;
                }
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

bool MQTTClient::Publish()
{
    return this->Publish(this->GetChromanceState());
}

bool MQTTClient::Publish(ChromanceState chromanceState)
{
    bool queued = false;

    if (this->mqttClient.connected())
    {
        unsigned long now = millis();

        if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
        {
            for (int32_t i = 0; i < PublishQueueSize; i++)
            {
                if (this->publishQueue[i].publishedAt < now)
                {
                    this->publishQueue[i].state = chromanceState;
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
    this->mqttClient.setBufferSize(PublishJsonBufferSize);
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

    uint8_t mac[6];
    
    WiFi.macAddress(mac);
    this->deviceID = "";
    this->deviceID += String(mac[0], HEX);
    this->deviceID += String(mac[1], HEX);
    this->deviceID += String(mac[2], HEX);
    this->deviceID += String(mac[3], HEX);
    this->deviceID += String(mac[4], HEX);
    this->deviceID += String(mac[5], HEX);

    uint8_t willQOS = 1U;

    if (this->mqttClient.connect(this->deviceID.c_str(), MQTTUsername, MQTTPassword, MQTTStateTopic, willQOS, false, "{\"availability\": \"0\", \"state\": \"None\", \"fps\": 0}"))
    {
        this->Subscribe();
        this->lastReconnectAttempt = 0;

        return;
    }
}

void MQTTClient::Subscribe()
{
    uint8_t qos = 1U;
    this->mqttClient.subscribe(MQTTCommandTopic, qos);
    this->mqttClient.subscribe(HomeAssistantStatusTopic, qos);
}

void MQTTClient::PublishState(ChromanceState state)
{
    JsonDocument doc;

    doc["animationStatus"] = state.animationStatus;
    doc["animationType"] = state.animationType;
    doc["brightness"] = state.brightness;
    doc["fps"] = state.fps;
    doc["availability"] = "1";
    doc["state"] = state.animationStatus != ANIMATION_STATUS_PLAYING && state.animationStatus != ANIMATION_STATUS_WAKING_UP ? "OFF" : "ON";
    doc["effect"] = state.effect;

    AnimationType animationType;

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        animationType = (AnimationType)i;

        doc[this->config->GetAnimationSpeedKey(animationType)] = this->config->GetAnimationSpeed(animationType);
        doc[this->config->GetRippleDecayKey(animationType)] = this->config->GetRippleDecay(animationType);
        doc[this->config->GetRippleLifespanKey(animationType)] = this->config->GetRippleLifespan(animationType);
        doc[this->config->GetRipplePulsePeriodKey(animationType)] = this->config->GetRipplePulsePeriod(animationType);
    }
    
    doc.shrinkToFit();

    size_t n = serializeJson(doc, this->publishJsonBuffer, PublishJsonBufferSize);

    this->mqttClient.publish(MQTTStateTopic, (const uint8_t*)&this->publishJsonBuffer, n, false);
}

void MQTTClient::PublishDeviceDiscovery()
{   
    this->PublishFPSSensorDiscovery();
    this->PublishLightDiscovery();

    AnimationType animationType;

    for (uint32_t i = 2; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        animationType = (AnimationType)i;

        this->PublishAnimationSpeedDiscovery(animationType);

        if (this->animationController->GetAnimation(animationType)->IsRippleAnimation())
        {
            this->PublishRipplePulsePeriodDiscovery(animationType);
            this->PublishRipplePulsePeriodDiscovery(animationType);
            this->PublishRippleDecayDiscovery(animationType);
            this->PublishRippleLifespanDiscovery(animationType);
        }
    }
}

void MQTTClient::PublishFPSSensorDiscovery()
{
    String uniqueID = String("chrfps1");
    String topic = this->GetDiscoveryTopic("sensor", uniqueID);
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
    doc["val_tpl"] = "{{ value_json.fps }}";
    doc["dev_cla"] = "frequency";
    doc["unit_of_meas"] = "Hz";
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishAnimationSpeedDiscovery(AnimationType animationType)
{
    String uniqueID;
    uniqueID += String("chrspd");
    uniqueID += String(animationType);

    String topic = this->GetDiscoveryTopic("number", uniqueID);

    String name;
    name += this->animationController->GetAnimation(animationType)->GetName();
    name += String(" Speed");

    String valueTemplate;
    valueTemplate += String("{{ value_json.");
    valueTemplate += this->config->GetAnimationSpeedKey(animationType);
    valueTemplate += String(" }}");

    String commandTemplate;
    commandTemplate += String("{ \"");
    commandTemplate += this->config->GetAnimationSpeedKey(animationType);
    commandTemplate += String("\": {{ value }} }");

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

    doc["name"] = name;
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["cmd_tpl"] = commandTemplate;
    doc["min"] = 0.01f;
    doc["max"] = 10.0f;
    doc["step"] = 0.01f;
    doc["val_tpl"] = valueTemplate;
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishRippleLifespanDiscovery(AnimationType animationType)
{
    String uniqueID;
    uniqueID += String("chrlfsp");
    uniqueID += String(animationType);

    String topic = this->GetDiscoveryTopic("number", uniqueID);

    String name;
    name += this->animationController->GetAnimation(animationType)->GetName();
    name += String(" Lifespan");

    String valueTemplate;
    valueTemplate += String("{{ value_json.");
    valueTemplate += this->config->GetRippleLifespanKey(animationType);
    valueTemplate += String(" }}");

    String commandTemplate;
    commandTemplate += String("{ \"");
    commandTemplate += this->config->GetRippleLifespanKey(animationType);
    commandTemplate += String("\": {{ value }} }");

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

    doc["name"] = name;
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["cmd_tpl"] = commandTemplate;
    doc["min"] = 1UL;
    doc["max"] = 30000UL;
    doc["step"] = 1UL;
    doc["val_tpl"] = valueTemplate;
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishRipplePulsePeriodDiscovery(AnimationType animationType)
{
    String uniqueID;
    uniqueID += String("chrplsprd");
    uniqueID += String(animationType);

    String topic = this->GetDiscoveryTopic("number", uniqueID);

    String name;
    name += this->animationController->GetAnimation(animationType)->GetName();
    name += String(" Pulse Period");

    String valueTemplate;
    valueTemplate += String("{{ value_json.");
    valueTemplate += this->config->GetRipplePulsePeriodKey(animationType);
    valueTemplate += String(" }}");

    String commandTemplate;
    commandTemplate += String("{ \"");
    commandTemplate += this->config->GetRipplePulsePeriodKey(animationType);
    commandTemplate += String("\": {{ value }} }");

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

    doc["name"] = name;
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["cmd_tpl"] = commandTemplate;
    doc["min"] = 1UL;
    doc["max"] = 30000UL;
    doc["step"] = 1UL;
    doc["val_tpl"] = valueTemplate;
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishRippleDecayDiscovery(AnimationType animationType)
{
    String uniqueID;
    uniqueID += String("chrdcy");
    uniqueID += String(animationType);

    String topic = this->GetDiscoveryTopic("number", uniqueID);

    String name;
    name += this->animationController->GetAnimation(animationType)->GetName();
    name += String(" Decay");

    String valueTemplate;
    valueTemplate += String("{{ value_json.");
    valueTemplate += this->config->GetRippleDecayKey(animationType);
    valueTemplate += String(" }}");

    String commandTemplate;
    commandTemplate += String("{ \"");
    commandTemplate += this->config->GetRippleDecayKey(animationType);
    commandTemplate += String("\": {{ value }} }");

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

    doc["name"] = name;
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["cmd_tpl"] = commandTemplate;
    doc["min"] = 0U;
    doc["max"] = 255U;
    doc["step"] = 1U;
    doc["val_tpl"] = valueTemplate;
    
    this->PublishDocument(doc, topic.c_str());
}


void MQTTClient::PublishLightDiscovery()
{
    String uniqueID = String("chrlight1");
    String topic = this->GetDiscoveryTopic("light", uniqueID);
    
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

    doc["name"] = "LED Controller";
    doc["uniq_id"] = uniqueID;
    doc["stat_t"] = MQTTStateRoute;
    doc["cmd_t"] = MQTTCommandRoute;
    doc["schema"] = "json";
    doc["color_temp"] = false;
    doc["brightness"] = true;
    doc["effect"] = true;

    JsonArray effects = doc["effect_list"].to<JsonArray>();
    effects.add("Random");

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        effects.add(this->animationController->GetAnimation((AnimationType)i)->GetName());
    }
    
    this->PublishDocument(doc, topic.c_str());
}

void MQTTClient::PublishDocument(JsonDocument doc, const char* topic)
{
    doc.shrinkToFit();
    size_t n = serializeJson(doc, this->publishJsonBuffer, PublishJsonBufferSize);
    this->mqttClient.publish(topic, (const uint8_t*)&this->publishJsonBuffer, n, false);
}

String MQTTClient::GetDiscoveryTopic(const char* entityType, String uniqueID)
{
    String topic;
    topic.reserve(128);
    topic += String("homeassistant/");
    topic += String(entityType);
    topic += String("/");
    topic += String(ChromanceNameLowercase);
    topic += String("/");
    topic += this->deviceID;
    topic += uniqueID;
    topic += String("/config");

    return topic;
}

ChromanceState MQTTClient::GetChromanceState()
{
    ChromanceState chromanceState;

    chromanceState.animationStatus = this->animationController->GetAnimationStatus();
    chromanceState.animationType = this->animationController->GetAnimationType();
    chromanceState.brightness = this->animationController->GetBrightness();
    chromanceState.fps = this->animationController->GetFPS();
    chromanceState.effect = chromanceState.animationType == ANIMATION_TYPE_RANDOM_ANIMATION ? 
        "Random" :
        this->animationController->GetAnimation(chromanceState.animationType)->GetName();

    return chromanceState;
}