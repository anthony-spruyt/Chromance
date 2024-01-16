#include "config.h"

using namespace Chromance;

Config::Config() :
    logLevel(0), // trace
    brightness(1),
    sleeping(false),
    preferences()
{   
}

void Config::Setup()
{
    preferences.begin(ConfigNamespace, true);
    this->logLevel = (uint8_t)this->preferences.getUShort(LogLevelConfigKey, this->logLevel);
    this->brightness = (uint8_t)this->preferences.getUShort(BrightnessConfigKey, this->brightness);
    this->sleeping = this->preferences.getBool(SleepingConfigKey, this->sleeping);
    preferences.end();
}

void Config::SetLogLevel(uint8_t value)
{
    if (value < 0 || value > 6)
    {
        return;
    }

    this->logLevel = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putUShort(LogLevelConfigKey, value);
    preferences.end();
}

uint8_t Config::GetLogLevel()
{
    return this->logLevel;
}

void Config::SetBrightness(uint8_t value)
{
    this->brightness = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putUShort(BrightnessConfigKey, value);
    preferences.end();
}

uint8_t Config::GetBrightness()
{
    return this->brightness;
}

void Config::SetSleeping(bool value)
{
    this->sleeping = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putBool(SleepingConfigKey, value);
    preferences.end();
}

bool Config::GetSleeping()
{
    return this->sleeping;
}