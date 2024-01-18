#include "config.h"

using namespace Chromance;

Config::Config() :
    logLevel(0), // trace
    brightness(1),
    sleeping(false),
    preferences()
{
    for (int32_t i = 0; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        this->animationSpeed[i] = 1.0f;
        this->rippleLifespan[i] = 2000UL;
        this->ripplePulsePeriod[i] = 2000UL;
        this->rippleDecay[i] = 247U;
    }
}

void Config::Setup()
{
    preferences.begin(ConfigNamespace, true);
    this->logLevel = (uint8_t)this->preferences.getUShort(LogLevelConfigKey, this->logLevel);
    this->brightness = (uint8_t)this->preferences.getUShort(BrightnessConfigKey, this->brightness);
    this->sleeping = this->preferences.getBool(SleepingConfigKey, this->sleeping);

    String speedKey;
    String lifespanKey;
    String pulsePeriodKey;
    String decayKey;
    
    for (int32_t i = 2; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        speedKey = this->GetAnimationSpeedKey((AnimationType)i);
        lifespanKey = this->GetRippleLifespanKey((AnimationType)i);
        pulsePeriodKey = this->GetRipplePulsePeriodKey((AnimationType)i);
        decayKey = this->GetRippleDecayKey((AnimationType)i);

        this->animationSpeed[i] = this->preferences.getBool(speedKey.c_str(), this->animationSpeed[i]);
        this->rippleLifespan[i] = this->preferences.getBool(lifespanKey.c_str(), this->rippleLifespan[i]);
        this->ripplePulsePeriod[i] = this->preferences.getBool(pulsePeriodKey.c_str(), this->ripplePulsePeriod[i]);
        this->rippleDecay[i] = this->preferences.getBool(decayKey.c_str(), this->rippleDecay[i]);
    }
    
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

float Config::GetAnimationSpeed(AnimationType animationType)
{
    return this->animationSpeed[animationType];
}

unsigned long Config::GetRippleLifespan(AnimationType animationType)
{
    return this->rippleLifespan[animationType];
}

unsigned long Config::GetRipplePulsePeriod(AnimationType animationType)
{
    return this->ripplePulsePeriod[animationType];
}

uint8_t Config::GetRippleDecay(AnimationType animationType)
{
    return this->rippleDecay[animationType];
}

void Config::SetAnimationSpeed(AnimationType animationType, float value)
{
    String key = this->GetAnimationSpeedKey(animationType);

    this->animationSpeed[animationType] = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putFloat(key.c_str(), value);
    preferences.end();
}

void Config::SetRippleLifespan(AnimationType animationType, unsigned long value)
{
    String key = this->GetRippleLifespanKey(animationType);

    this->rippleLifespan[animationType] = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putULong(key.c_str(), value);
    preferences.end();
}

void Config::SetRipplePulsePeriod(AnimationType animationType, unsigned long value)
{
    String key = this->GetRipplePulsePeriodKey(animationType);

    this->ripplePulsePeriod[animationType] = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putULong(key.c_str(), value);
    preferences.end();
}

void Config::SetRippleDecay(AnimationType animationType, uint8_t value)
{
    String key = this->GetRippleDecayKey(animationType);

    this->rippleDecay[animationType] = value;
    preferences.begin(ConfigNamespace, false);
    this->preferences.putUInt(key.c_str(), value);
    preferences.end();
}

String Config::GetAnimationSpeedKey(AnimationType animationType)
{
    return AnimationSpeedConfigKeyPrefix + animationType;
}

String Config::GetRippleLifespanKey(AnimationType animationType)
{
    return RippleLifespanConfigKeyPrefix + animationType;
}

String Config::GetRipplePulsePeriodKey(AnimationType animationType)
{
    return RipplePulsePeriodConfigKeyPrefix + animationType;
}

String Config::GetRippleDecayKey(AnimationType animationType)
{
    return RippleDecayConfigKeyPrefix + animationType;
}
