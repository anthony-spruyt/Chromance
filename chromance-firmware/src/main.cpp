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
ChromanceState currentChromanceState;
unsigned long lastChromanceStateUpdate = 0U;

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

    mqttClient.Setup();

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
#ifdef MONITOR_TASK_STACK_SIZES
        MonitorStackSize(AnimationControllerTaskStackSize, "AnimationControllerTask");
#endif
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
#ifdef MONITOR_TASK_STACK_SIZES
        MonitorStackSize(WiFiServiceTaskStackSize, "WiFiServiceTask");
#endif
        wifiService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void OTAServiceTask(void *pvParameters)
{
    logger.Debug("OTAServiceTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
#ifdef MONITOR_TASK_STACK_SIZES
        MonitorStackSize(OTAServiceTaskStackSize, "OTAServiceTask");
#endif
        otaService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void MQTTClientTask(void *pvParameters)
{
    logger.Debug("MQTTClientTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
#ifdef MONITOR_TASK_STACK_SIZES
        MonitorStackSize(MQTTClientTaskStackSize, "MQTTClientTask");
#endif
        mqttClient.Loop();

        // TODO move to state monitor service
        unsigned long now = millis();

        if
        (
            ChromanceStateUpdatesEnabled && 
            now - lastChromanceStateUpdate > ChromanceStateUpdateFrequency
        )
        {
            lastChromanceStateUpdate = now;

            currentChromanceState.animationStatus = animationController.GetAnimationStatus();
            currentChromanceState.animationType = animationController.GetAnimationType();
            currentChromanceState.brightness = animationController.GetBrightness();
            currentChromanceState.fps = animationController.GetFPS();

            mqttClient.Publish(currentChromanceState);
        }

        vTaskDelay(TaskDelay);
    }
}