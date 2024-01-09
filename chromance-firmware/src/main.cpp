//////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////

#include "globals.h"
#include "services/animationController.h"
#include "services/config.h"
#include "services/timeService.h"
#include "services/logger.h"
#include "services/wifiService.h"
#include "services/otaService.h"
#include "services/mqttClient.h"

using namespace Chromance;

//////////////////////////////////////////
// FUNCTION DEFINITIONS
//////////////////////////////////////////

void loop(void);
void setup(void);
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
void OtherCommandHandler(char* topic, uint8_t* payload, unsigned int length);

//////////////////////////////////////////
// VARIABLE DEFINITIONS
//////////////////////////////////////////

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

//////////////////////////////////////////
// FUNCTION IMPLEMENTATIONS
//////////////////////////////////////////

//////////////////////////////////////////
// SKETCH FUNCTIONS
//////////////////////////////////////////

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
            AnimationControllerTask, // task function
            "AnimationControllerTask", // task name
            AnimationControllerTaskStackSize, // task stack size
            nullptr, // task parameter
            AnimationControllerTaskPriority, // task priority
            &AnimationControllerTaskHandle, // task handle to keep track of created task
            AnimationControllerTaskCore // core affinity, note system tasks are on core 0
        );
    }

    wifiService.Setup();

    if (WiFiServiceTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            WiFiServiceTask, // task function
            "WiFiServiceTask", // task name
            WiFiServiceTaskStackSize, // task stack size
            nullptr, // task parameter
            WiFiServiceTaskPriority, // task priority
            &WiFiServiceTaskHandle, // task handle to keep track of created task
            WiFiServiceTaskCore // core affinity, note system tasks are on core 0
        );
    }

    // Below services require WiFi connectivity
    timeService.Setup();

    otaService.Setup();

    if (OTAServiceTaskHandle == nullptr)
    {
        xTaskCreatePinnedToCore
        (
            OTAServiceTask, // task function
            "OTAServiceTask", // task name
            OTAServiceTaskStackSize, // task stack size
            nullptr, // task parameter
            OTAServiceTaskPriority, // task priority
            &OTAServiceTaskHandle, // task handle to keep track of created task
            OTAServiceTaskCore // core affinity, note system tasks are on core 0
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
            MQTTClientTask, // task function
            "MQTTClientTask", // task name
            MQTTClientTaskStackSize, // task stack size
            nullptr, // task parameter
            MQTTClientTaskPriority, // task priority
            &MQTTClientTaskHandle, // task handle to keep track of created task
            MQTTClientTaskCore // core affinity, note system tasks are on core 0
        );
    }
}

void loop()
{
}

//////////////////////////////////////////
// TASKS
//////////////////////////////////////////

void AnimationControllerTask(void *pvParameters)
{
    logger.Debug("AnimationControllerTask running on core: " + String(xPortGetCoreID()));

    delay(StartupDelay);

    logger.Debug("LED controller is now running");

    for (;;)
    {
        if (!otaService.IsUpdating())
        {
            animationController.Loop();
        }
        
        vTaskDelay(TaskDelay);
    }
}

void WiFiServiceTask(void *pvParameters)
{
    logger.Debug("WiFiServiceTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        wifiService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void OTAServiceTask(void *pvParameters)
{
    logger.Debug("OTAServiceTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        otaService.Loop();

        vTaskDelay(TaskDelay);
    }
}

void MQTTClientTask(void *pvParameters)
{
    logger.Debug("MQTTClientTask running on core: " + String(xPortGetCoreID()));

    for (;;)
    {
        mqttClient.Loop();

        vTaskDelay(TaskDelay);
    }
}

//////////////////////////////////////////
// COMMAND HANDLERS
//////////////////////////////////////////

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

void OtherCommandHandler(char* topic, uint8_t* payload, unsigned int length)
{
    logger.Info("Command not supported");
}