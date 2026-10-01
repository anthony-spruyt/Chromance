#include "config.h"

using namespace Chromance;

Config::Config() :
    preferences(),
    logLevel(0), // trace
    brightness(1),
    sleeping(false),
    transitionDuration(DefaultTransitionDuration),
    rippleStepsPerSecond(DefaultRippleStepsPerSecond),
    maxBrightness(DefaultMaxBrightness),
    maxCurrent(DefaultMaxCurrent),
    randomAnimationDuration(DefaultRandomAnimationDuration),
    dirty(false),
    changedAt(0UL)
{
    this->semaphore = xSemaphoreCreateMutex();

    for (int32_t i = 0; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        this->animationSpeed[i] = 1.0f;
        this->rippleLifespan[i] = 2000UL;
        this->ripplePulsePeriod[i] = 2000UL;
        this->rippleDecay[i] = 247U;
    }
}

Config::~Config()
{
    vSemaphoreDelete(this->semaphore);
}

void Config::Setup()
{
    preferences.begin(ConfigNamespace, true);
    this->logLevel = (uint8_t)this->preferences.getUShort(LogLevelConfigKey, this->logLevel);
    this->brightness = (uint8_t)this->preferences.getUShort(BrightnessConfigKey, this->brightness);
    this->sleeping = this->preferences.getBool(SleepingConfigKey, this->sleeping);
    this->transitionDuration = this->preferences.getULong(TransitionDurationConfigKey, this->transitionDuration);
    this->rippleStepsPerSecond = this->preferences.getULong(RippleStepsPerSecondConfigKey, this->rippleStepsPerSecond);
    this->maxBrightness = (uint8_t)this->preferences.getUShort(MaxBrightnessConfigKey, this->maxBrightness);
    this->maxCurrent = this->preferences.getULong(MaxCurrentConfigKey, this->maxCurrent);
    this->randomAnimationDuration = this->preferences.getULong(RandomAnimationDurationConfigKey, this->randomAnimationDuration);

    String speedKey;
    String lifespanKey;
    String pulsePeriodKey;
    String decayKey;

    for (int32_t i = 0; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        speedKey = this->GetAnimationSpeedKey((AnimationType)i);
        lifespanKey = this->GetRippleLifespanKey((AnimationType)i);
        pulsePeriodKey = this->GetRipplePulsePeriodKey((AnimationType)i);
        decayKey = this->GetRippleDecayKey((AnimationType)i);

        this->animationSpeed[i] = this->preferences.getFloat(speedKey.c_str(), this->animationSpeed[i]);
        this->rippleLifespan[i] = this->preferences.getULong(lifespanKey.c_str(), this->rippleLifespan[i]);
        this->ripplePulsePeriod[i] = this->preferences.getULong(pulsePeriodKey.c_str(), this->ripplePulsePeriod[i]);
        this->rippleDecay[i] = (uint8_t)this->preferences.getUShort(decayKey.c_str(), this->rippleDecay[i]);
    }

    preferences.end();
}

void Config::Save(bool now)
{
    if (!this->dirty || (!now && millis() - this->changedAt < ConfigSaveDelay))
    {
        return;
    }

    if (xSemaphoreTake(this->semaphore, now ? portMAX_DELAY : 0U) != pdTRUE)
    {
        return;
    }

    this->dirty = false;

    preferences.begin(ConfigNamespace, false);
    this->SaveUShort(LogLevelConfigKey, this->logLevel);
    this->SaveUShort(BrightnessConfigKey, this->brightness);
    this->SaveBool(SleepingConfigKey, this->sleeping);
    this->SaveULong(TransitionDurationConfigKey, this->transitionDuration);
    this->SaveULong(RippleStepsPerSecondConfigKey, this->rippleStepsPerSecond);
    this->SaveUShort(MaxBrightnessConfigKey, this->maxBrightness);
    this->SaveULong(MaxCurrentConfigKey, this->maxCurrent);
    this->SaveULong(RandomAnimationDurationConfigKey, this->randomAnimationDuration);

    for (int32_t i = 0; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        this->SaveFloat(this->GetAnimationSpeedKey((AnimationType)i).c_str(), this->animationSpeed[i]);
        this->SaveULong(this->GetRippleLifespanKey((AnimationType)i).c_str(), this->rippleLifespan[i]);
        this->SaveULong(this->GetRipplePulsePeriodKey((AnimationType)i).c_str(), this->ripplePulsePeriod[i]);
        this->SaveUShort(this->GetRippleDecayKey((AnimationType)i).c_str(), this->rippleDecay[i]);
    }

    preferences.end();

    xSemaphoreGive(this->semaphore);
}

// Each put commits and wears flash, so only write values that differ from what is stored. Passing value as the default skips unset keys still at their default
void Config::SaveUShort(const char* key, uint16_t value)
{
    if (this->preferences.getUShort(key, value) != value)
    {
        this->preferences.putUShort(key, value);
    }
}

void Config::SaveULong(const char* key, uint32_t value)
{
    if (this->preferences.getULong(key, value) != value)
    {
        this->preferences.putULong(key, value);
    }
}

void Config::SaveFloat(const char* key, float value)
{
    if (this->preferences.getFloat(key, value) != value)
    {
        this->preferences.putFloat(key, value);
    }
}

void Config::SaveBool(const char* key, bool value)
{
    if (this->preferences.getBool(key, value) != value)
    {
        this->preferences.putBool(key, value);
    }
}

void Config::SetLogLevel(uint8_t value)
{
    if (value > LOG_LEVEL_NONE)
    {
        return;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->logLevel = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint8_t Config::GetLogLevel()
{
    return this->logLevel;
}

void Config::SetBrightness(uint8_t value)
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->brightness = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint8_t Config::GetBrightness()
{
    return this->brightness;
}

void Config::SetSleeping(bool value)
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->sleeping = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

bool Config::GetSleeping()
{
    return this->sleeping;
}

void Config::SetTransitionDuration(unsigned long value)
{
    if (value > MaxTransitionDuration)
    {
        value = MaxTransitionDuration;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->transitionDuration = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

unsigned long Config::GetTransitionDuration()
{
    return this->transitionDuration;
}

void Config::SetRippleStepsPerSecond(uint32_t value)
{
    // Zero would divide by zero when working out the step interval
    if (value < 1U)
    {
        value = 1U;
    }
    else if (value > MaxRippleStepsPerSecond)
    {
        value = MaxRippleStepsPerSecond;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->rippleStepsPerSecond = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint32_t Config::GetRippleStepsPerSecond()
{
    return this->rippleStepsPerSecond;
}

void Config::SetMaxBrightness(uint8_t value)
{
    if (value < 1U)
    {
        value = 1U;
    }
    else if (value > 100U)
    {
        value = 100U;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->maxBrightness = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint8_t Config::GetMaxBrightness()
{
    return this->maxBrightness;
}

void Config::SetMaxCurrent(uint32_t value)
{
    if (value < MinMaxCurrent)
    {
        value = MinMaxCurrent;
    }
    else if (value > MaxMaxCurrent)
    {
        value = MaxMaxCurrent;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->maxCurrent = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint32_t Config::GetMaxCurrent()
{
    return this->maxCurrent;
}

void Config::SetRandomAnimationDuration(uint32_t value)
{
    if (value < MinRandomAnimationDuration)
    {
        value = MinRandomAnimationDuration;
    }
    else if (value > MaxRandomAnimationDuration)
    {
        value = MaxRandomAnimationDuration;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->randomAnimationDuration = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

uint32_t Config::GetRandomAnimationDuration()
{
    return this->randomAnimationDuration;
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
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->animationSpeed[animationType] = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

void Config::SetRippleLifespan(AnimationType animationType, unsigned long value)
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->rippleLifespan[animationType] = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

void Config::SetRipplePulsePeriod(AnimationType animationType, unsigned long value)
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->ripplePulsePeriod[animationType] = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

void Config::SetRippleDecay(AnimationType animationType, uint8_t value)
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) != pdTRUE)
    {
        return;
    }

    this->rippleDecay[animationType] = value;
    this->dirty = true;
    this->changedAt = millis();

    xSemaphoreGive(this->semaphore);
}

String Config::GetAnimationSpeedKey(AnimationType animationType)
{
    String key;
    key.reserve(3);
    key += String(AnimationSpeedConfigKeyPrefix);
    key += String(animationType);

    return key;
}

String Config::GetRippleLifespanKey(AnimationType animationType)
{
    String key;
    key.reserve(3);
    key += String(RippleLifespanConfigKeyPrefix);
    key += String(animationType);

    return key;
}

String Config::GetRipplePulsePeriodKey(AnimationType animationType)
{
    String key;
    key.reserve(3);
    key += String(RipplePulsePeriodConfigKeyPrefix);
    key += String(animationType);

    return key;
}

String Config::GetRippleDecayKey(AnimationType animationType)
{
    String key;
    key.reserve(3);
    key += String(RippleDecayConfigKeyPrefix);
    key += String(animationType);

    return key;
}
