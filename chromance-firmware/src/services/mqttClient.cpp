#include "mqttClient.h"
#include <ArduinoJson.h>

using namespace Chromance;

MQTTClient::MQTTClient(Logger* logger, AnimationController* animationController) : 
    mqttClient(espClient), 
    lastReconnectAttempt(0)
{
    this->animationController = animationController;
    this->logger = logger;
}

void MQTTClient::Setup
(
    REBOOT_COMMAND_CALLBACK_SIGNATURE rebootCommandCallback,
    WAKE_COMMAND_CALLBACK_SIGNATURE wakeCommandCallback,
    SLEEP_COMMAND_CALLBACK_SIGNATURE sleepCommandCallback,
    SPEED_COMMAND_CALLBACK_SIGNATURE speedCommandCallback,
    PLAY_COMMAND_CALLBACK_SIGNATURE playCommandCallback,
    BRIGHTNESS_COMMAND_CALLBACK_SIGNATURE brightnessCommandCallback,
    LOGLEVEL_COMMAND_CALLBACK_SIGNATURE logLevelCommandCallback,
    OTHER_COMMAND_CALLBACK_SIGNATURE otherCommandCallback
)
{
    this->rebootCommandCallback = rebootCommandCallback;
    this->wakeCommandCallback = wakeCommandCallback;
    this->sleepCommandCallback = sleepCommandCallback;
    this->speedCommandCallback = speedCommandCallback;
    this->playCommandCallback = playCommandCallback;
    this->brightnessCommandCallback = brightnessCommandCallback;
    this->logLevelCommandCallback = logLevelCommandCallback;
    this->otherCommandCallback = otherCommandCallback;

    this->Configure();
    this->Connect();
}

void MQTTClient::Callback(char* topic, byte* payload, uint32_t length)
{
    this->logger->Debug("MQTT message arrived in topic: " + String(topic));
    
    DynamicJsonDocument doc(64);
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        this->logger->Error("Deserialization error: " + String(error.c_str()));

        return;
    }

    const char* messageType = doc[MQTTPayloadTypeKey];
    
    this->logger->Debug("MQTT message type: " + String(messageType));

    if (strcmp(messageType, "statusCommand") == 0)
    {
        const char* command = doc[MQTTPayloadBodyKey];

        this->logger->Debug("MQTT message body: " + String(command));

        if (strcmp(command, "wake") == 0)
        {
            this->wakeCommandCallback();

            return;
        }
        else if (strcmp(command, "sleep") == 0)
        {
            this->sleepCommandCallback();

            return;
        }
        else if (strcmp(command, "reboot") == 0)
        {
            this->rebootCommandCallback();

            return;
        }
    }
    else if (strcmp(messageType, "speedCommand") == 0)
    {
        float speed = doc[MQTTPayloadBodyKey];

        this->logger->Debug("MQTT message body: " + String(speed, 2));
        this->speedCommandCallback(speed);

        return;
    }
    else if (strcmp(messageType, "brightnessCommand") == 0)
    {
        uint8_t brightness = doc[MQTTPayloadBodyKey];

        this->logger->Debug("MQTT message body: " + String(brightness));
        this->brightnessCommandCallback(brightness);

        return;
    }
    else if (strcmp(messageType, "playCommand") == 0)
    {
        const char* animation = doc[MQTTPayloadBodyKey];

        this->logger->Debug("MQTT message body: " + String(animation));
        
        if (strcmp(animation, "stripTest") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_STRIP_TEST);
        }
        else if (strcmp(animation, "randomAnimation") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_RANDOM_ANIMATION);
        }
        else if (strcmp(animation, "randomPulse") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_RANDOM_PULSE);
        }
        else if (strcmp(animation, "cubePulse") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_CUBE_PULSE);
        }
        else if (strcmp(animation, "starBurstPulse") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_STAR_BURST_PULSE);
        }
        else if (strcmp(animation, "centerPulse") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_CENTER_PULSE);
        }
        else if (strcmp(animation, "rainbowBeat") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_RAINBOW_BEAT);
        }
        else if (strcmp(animation, "rainbowMarch") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_RAINBOW_MARCH);
        }
        else if (strcmp(animation, "pulse") == 0)
        {
            this->playCommandCallback(ANIMATION_TYPE_PULSE);
        }

        return;
    }
    else if (strcmp(messageType, "logLevelCommand") == 0)
    {
        const char* logLevel = doc[MQTTPayloadBodyKey];

        this->logger->Debug("MQTT message body: " + String(logLevel));
        
        if (strcmp(logLevel, "trace") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_TRACE);
        }
        else if (strcmp(logLevel, "debug") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_DEBUG);
        }
        else if (strcmp(logLevel, "info") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_INFORMATION);
        }
        else if (strcmp(logLevel, "warn") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_WARNING);
        }
        else if (strcmp(logLevel, "error") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_ERROR);
        }
        else if (strcmp(logLevel, "crit") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_CRITICAL);
        }
        else if (strcmp(logLevel, "none") == 0)
        {
            this->logLevelCommandCallback(LOG_LEVEL_NONE);
        }

        return;
    }

    this->otherCommandCallback(topic, payload, length);
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
    }
}

void MQTTClient::Configure()
{
    this->logger->Info("Configure MQTT");
    this->mqttClient.setServer(MQTTBroker, MQTTPort);
    this->mqttClient.setCallback
    (
        [this](char* topic, byte* payload, uint32_t length)
        {
            this->Callback(topic, payload, length);
        }
    );
    this->mqttClient.setKeepAlive(15);
}

void MQTTClient::Connect()
{
    if (this->mqttClient.connected() || !WiFi.isConnected())
    {
        return;
    }

    this->logger->Debug("MQTT connecting...");

    String client_id = "chromance-client-";
    client_id += String(WiFi.macAddress());

    if (this->mqttClient.connect(client_id.c_str(), MQTTUsername, MQTTPassword))
    {
        this->logger->Debug("MQTT connected");
        //TODO publish initial state IE connected, brightness etc
        this->Subscribe();
        this->lastReconnectAttempt = 0;

        return;
    }

    this->logger->Error("MQTT connect failed: " + String(this->mqttClient.state()));
}

void MQTTClient::Subscribe()
{
    this->mqttClient.subscribe(MQTTTopic);
    this->logger->Debug("MQTT topic subscribed: " + String(MQTTTopic));
}