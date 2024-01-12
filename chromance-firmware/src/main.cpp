#include "globals.h"
#include "services/animationController.h"
#include "services/config.h"
#include "services/timeService.h"
#include "services/logger.h"
#include "services/wifiService.h"
#include "services/otaService.h"
#include "services/mqttClient.h"

using namespace Chromance;

void loop(void);
void setup(void);
void MonitorStackSize(uint32_t stackSize, const char* taskName);
void AnimationControllerTask(void *pvParameters);
void WiFiServiceTask(void *pvParameters);
void OTAServiceTask(void *pvParameters);
void MQTTClientTask(void *pvParameters);
void RebootCommandHandler();
void WakeCommandHandler();
void SleepCommandHandler();
void SpeedCommandHandler(float speed);
void PlayCommandHandler(AnimationType animation);
void BrightnessCommandHandler(uint8_t brightness);
void LogLevelCommandHandler(LogLevel logLevel);
void OtherCommandHandler(char* topic, uint8_t* payload, uint32_t length);

Config config;
TimeService timeService;
Logger logger(&timeService, &config);
AnimationController animationController(&logger, &config);
MQTTClient mqttClient(&logger, &animationController);
OTAService otaService(&logger);
WiFiService wifiService(&logger);
TaskHandle_t AnimationControllerTaskHandle;
TaskHandle_t WiFiServiceTaskHandle;
TaskHandle_t OTAServiceTaskHandle;
TaskHandle_t MQTTClientTaskHandle;

void setup()
{
#ifdef SERIAL_ENABLED
    Serial.begin(115200);
    Serial.println("Booting");
#endif
    config.Setup();
    logger.Setup();
    animationController.Setup();

    if (AnimationControllerTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            AnimationControllerTask,
            "AnimationControllerTask",
            AnimationControllerTaskStackSize,
            nullptr,
            AnimationControllerTaskPriority,
            &AnimationControllerTaskHandle,
            AnimationControllerTaskCore
        );
    }

    wifiService.Setup();

    if (WiFiServiceTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            WiFiServiceTask,
            "WiFiServiceTask",
            WiFiServiceTaskStackSize,
            nullptr,
            WiFiServiceTaskPriority,
            &WiFiServiceTaskHandle,
            WiFiServiceTaskCore
        );
    }
    
    timeService.Setup();

    otaService.Setup();

    if (OTAServiceTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            OTAServiceTask,
            "OTAServiceTask",
            OTAServiceTaskStackSize,
            nullptr,
            OTAServiceTaskPriority,
            &OTAServiceTaskHandle,
            OTAServiceTaskCore
        );
    }

    mqttClient.Setup
    (
        RebootCommandHandler,
        WakeCommandHandler,
        SleepCommandHandler,
        SpeedCommandHandler,
        PlayCommandHandler,
        BrightnessCommandHandler,
        LogLevelCommandHandler,
        OtherCommandHandler
    );

    if (MQTTClientTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            MQTTClientTask,
            "MQTTClientTask",
            MQTTClientTaskStackSize,
            nullptr,
            MQTTClientTaskPriority,
            &MQTTClientTaskHandle,
            MQTTClientTaskCore
        );
    }
}

void loop()
{
}

void MonitorStackSize(uint32_t stackSize, const char* taskName)
{
    float highWaterMark = (float)uxTaskGetStackHighWaterMark(NULL);

    if (highWaterMark / (float)stackSize < 0.2f)
    {
        logger.Warn("Task " + String(taskName) + " stack is over 80% utilized. Consider increasing its size");
    }
    else if (highWaterMark / (float)stackSize > 0.5f)
    {
        logger.Warn("Task " + String(taskName) + " stack is under 50% utilized. Consider reducing its size");
    }
}

void AnimationControllerTask(void *pvParameters)
{
    logger.Debug("AnimationControllerTask running on core: " + String(xPortGetCoreID()));

    delay(StartupDelay);

    logger.Debug("LED controller is now running");

    for (;;)
    {
        MonitorStackSize(AnimationControllerTaskStackSize, "AnimationControllerTask");

        if (!otaService.IsUpdating())
        {
            animationController.Loop();
        }
        else
        {
            vTaskDelay(TaskDelay);
        }
    }
}

void WiFiServiceTask(void *pvParameters)
{
    logger.Debug("WiFiServiceTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        MonitorStackSize(WiFiServiceTaskStackSize, "WiFiServiceTask");

        wifiService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void OTAServiceTask(void *pvParameters)
{
    logger.Debug("OTAServiceTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        MonitorStackSize(OTAServiceTaskStackSize, "OTAServiceTask");

        otaService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void MQTTClientTask(void *pvParameters)
{
    logger.Debug("MQTTClientTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        MonitorStackSize(MQTTClientTaskStackSize, "MQTTClientTask");

        mqttClient.Loop();

        vTaskDelay(TaskDelay);
    }
}

void RebootCommandHandler()
{
    delay(1000);
    ESP.restart();
}

void WakeCommandHandler()
{
    animationController.Wake();
}

void SleepCommandHandler()
{
    animationController.Sleep();
}

void SpeedCommandHandler(float speed)
{
    config.SetSpeed(speed);
    logger.Info("Animation speed set to: " + String(speed, 2));
}

void BrightnessCommandHandler(u8_t brightness)
{
    config.SetBrightness(brightness);
    logger.Info("LED brightness set to: " + String(brightness));
}

void PlayCommandHandler(AnimationType animation)
{
    animationController.Play(animation);
}

void LogLevelCommandHandler(LogLevel logLevel)
{
    config.SetLogLevel((uint8_t)logLevel);
}

void OtherCommandHandler(char* topic, uint8_t* payload, uint32_t length)
{
    logger.Info("Command not supported");
}