#include "config.h"

using namespace Chromance;

Config::Config() :
    logLevel(0), // trace
    brightness(1),
    speed(1),
    preferences()
{   
}

void Config::Setup()
{
    preferences.begin(ConfigNamespace, true);
    this->logLevel = (uint8_t)this->preferences.getUShort(LogLevelConfigKey, this->logLevel);
    preferences.remove(BrightnessConfigKey); // TODO temp line - remove once type has been changed
    this->brightness = (uint8_t)this->preferences.getUShort(BrightnessConfigKey, this->brightness);
    this->speed = this->preferences.getFloat(SpeedConfigKey, this->speed);
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

void Config::SetSpeed(float value)
{
    if (value <= 0)
    {
        return;
    }

    this->speed = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putFloat(SpeedConfigKey, value);
    preferences.end();
}

float Config::GetSpeed()
{
    return this->speed;
}