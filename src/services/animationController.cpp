#include "animationController.h"
#include "../animations/cubePulseAnimation.h"
#include "../animations/pulseAnimation.h"
#include "../animations/rainbowBeatAnimation.h"
#include "../animations/rainbowMarchAnimation.h"
#include "../animations/stripTestAnimation.h"
#include "../animations/starBurstPulseAnimation.h"
#include "../animations/centerPulseAnimation.h"
#include "../animations/randomPulseAnimation.h"
#include "../animations/aroundTheWorldAnimation.h"

using namespace Chromance;

AnimationController::AnimationController(Logger* logger, Config* config) :
    currentAnimationType(ANIMATION_TYPE_RANDOM_ANIMATION),
    lastRandomAnimationStarted(0),
    next(ANIMATION_REQUEST_NONE),
    ripplePool(),
    brightness(0U),
    brightnessFrom(0U),
    brightnessTarget(0U),
    brightnessChangedAt(0UL),
    current(0U)
{
    this->logger = logger;
    this->config = config;
    this->semaphore = xSemaphoreCreateMutex();
}

AnimationController::~AnimationController()
{
    Animation* animation = nullptr;

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        animation = this->animations[i];

        if (animation != nullptr)
        {
            delete animation;
        }
    }

    vSemaphoreDelete(this->semaphore);
}

void AnimationController::Setup()
{
    random16_set_seed(esp_random());

    FastLED.addLeds<NEOPIXEL, BlueStripDataPin>(this->leds, BlueStripOffset, BlueStripLength);
    FastLED.addLeds<NEOPIXEL, GreenStripDataPin>(this->leds, GreenStripOffset, GreenStripLength);
    FastLED.addLeds<NEOPIXEL, RedStripDataPin>(this->leds, RedStripOffset, RedStripLength);
    FastLED.addLeds<NEOPIXEL, BlackStripDataPin>(this->leds, BlackStripOffset, BlackStripLength);

    FastLED.setMaxRefreshRate(MaxRefreshRate);
    FastLED.setCorrection(TypicalLEDStrip);

    FastLED.clear();
    FastLED.show();

    this->animations[ANIMATION_TYPE_RANDOM_ANIMATION] = nullptr;
    this->animations[ANIMATION_TYPE_STRIP_TEST] = new StripTestAnimation(ANIMATION_TYPE_STRIP_TEST, this->config, this->logger);
    this->animations[ANIMATION_TYPE_RANDOM_PULSE] = RandomPulseAnimationEnabled ? new RandomPulseAnimation(ANIMATION_TYPE_RANDOM_PULSE, &ripplePool, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_CUBE_PULSE] = CubePulseAnimationEnabled ? new CubePulseAnimation(ANIMATION_TYPE_CUBE_PULSE, &ripplePool, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_STAR_BURST_PULSE] = StarBurstPulseAnimationEnabled ? new StarBurstPulseAnimation(ANIMATION_TYPE_STAR_BURST_PULSE, &ripplePool, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_CENTER_PULSE] = CenterPulseAnimationEnabled ? new CenterPulseAnimation(ANIMATION_TYPE_CENTER_PULSE, &ripplePool, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_RAINBOW_BEAT] = RainbowBeatAnimationEnabled ? new RainbowBeatAnimation(ANIMATION_TYPE_RAINBOW_BEAT, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_RAINBOW_MARCH] = RainbowMarchAnimationEnabled ? new RainbowMarchAnimation(ANIMATION_TYPE_RAINBOW_MARCH, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_PULSE] = PulseAnimationEnabled ? new PulseAnimation(ANIMATION_TYPE_PULSE, this->config, this->logger) : nullptr;
    this->animations[ANIMATION_TYPE_AROUND_THE_WORLD] = AroundTheWorldAnimationEnabled ? new AroundTheWorldAnimation(ANIMATION_TYPE_AROUND_THE_WORLD, &ripplePool, this->config, this->logger) : nullptr;

    // Deferred to the first Loop() so the fade in doesn't start during the task's startup delay
    if (!config->GetSleeping())
    {
        this->next = ANIMATION_REQUEST_WAKE;
    }
}

void AnimationController::Loop()
{
    if (xSemaphoreTake(this->semaphore, 0U) == pdTRUE)
    {
        this->HandleAnimationRequest();
        this->HandleRandomAnimation();

        xSemaphoreGive(this->semaphore);
    }

    this->Render();
    // Right after show() nothing is being sent, so the flash write stall can't garble the LEDs
    this->config->Save();

    vTaskDelay(TaskDelay);
}

void AnimationController::Sleep()
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = ANIMATION_REQUEST_SLEEP;

        xSemaphoreGive(this->semaphore);
    }
}

void AnimationController::Wake()
{
    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = ANIMATION_REQUEST_WAKE;

        xSemaphoreGive(this->semaphore);
    }
}

void AnimationController::Play(AnimationType animationType)
{
    if
    (
        animationType >= ANIMATION_TYPE_NUMBER_OF_ANIMATIONS ||
        (animationType != ANIMATION_TYPE_RANDOM_ANIMATION && this->animations[animationType] == nullptr)
    )
    {
        return;
    }

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = ANIMATION_REQUEST_PLAY;
        this->currentAnimationType = animationType;

        xSemaphoreGive(this->semaphore);
    }
}

AnimationType AnimationController::GetAnimationType()
{
    return this->currentAnimationType;
}

AnimationStatus AnimationController::GetAnimationStatus()
{
    return this->config->GetSleeping() ? ANIMATION_STATUS_SLEEPING : ANIMATION_STATUS_PLAYING;
}

uint8_t AnimationController::GetBrightness()
{
    return this->config->GetBrightness();
}

void AnimationController::SetBrightness(uint8_t value)
{
    this->config->SetBrightness(value);
}

uint32_t AnimationController::GetFPS()
{
    return FastLED.getFPS();
}

uint32_t AnimationController::GetCurrent()
{
    return this->current;
}

Animation* AnimationController::GetAnimation(AnimationType animationType)
{
    return this->animations[animationType];
}

void AnimationController::HandleAnimationRequest()
{
    if (this->next == ANIMATION_REQUEST_PLAY)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(false);
        this->lastRandomAnimationStarted = millis();
        this->Show(this->currentAnimationType);
    }
    else if (this->next == ANIMATION_REQUEST_SLEEP)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(true);

        for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
        {
            if (this->animations[i] != nullptr)
            {
                this->animations[i]->Sleep(i != ANIMATION_TYPE_STRIP_TEST);
            }
        }
    }
    else if (this->next == ANIMATION_REQUEST_WAKE)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(false);
        this->lastRandomAnimationStarted = millis();
        this->Show(this->currentAnimationType);
    }
}

void AnimationController::HandleRandomAnimation()
{
    unsigned long now = millis();

    if
    (
        !config->GetSleeping() &&
        this->currentAnimationType == ANIMATION_TYPE_RANDOM_ANIMATION &&
        now - this->lastRandomAnimationStarted > this->config->GetRandomAnimationDuration() * 1000UL
    )
    {
        this->lastRandomAnimationStarted = now;
        this->Show(ANIMATION_TYPE_RANDOM_ANIMATION);
    }
}

void AnimationController::Show(AnimationType animationType)
{
    if (animationType == ANIMATION_TYPE_RANDOM_ANIMATION)
    {
        animationType = this->NextAnimation();
    }

    // The strip test blocks for seconds at a time so it can't take part in a fade
    bool fade = animationType != ANIMATION_TYPE_STRIP_TEST;

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        if (this->animations[i] != nullptr && i != animationType)
        {
            this->animations[i]->Sleep(fade && i != ANIMATION_TYPE_STRIP_TEST);
        }
    }

    this->animations[animationType]->Wake(fade);
}

void AnimationController::Render()
{
    Animation* animation;
    CRGB* buffer;
    uint8_t scale;

    fill_solid(this->leds, NumberOfLEDs, CRGB::Black);

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        animation = this->animations[i];

        if (animation == nullptr || animation->GetStatus() == ANIMATION_STATUS_SLEEPING)
        {
            continue;
        }

        animation->Loop();
        animation->Transition();

        // Eased scales of a crossfade still sum to full brightness, as ease(x) + ease(1 - x) == 1
        scale = ease8InOutCubic(animation->GetTransitionScale());

        if (scale == 0)
        {
            continue;
        }

        buffer = animation->GetBuffer();

        for (int32_t j = 0; j < NumberOfLEDs; j++)
        {
            this->leds[j] += buffer[j].scale8(scale);
        }
    }

    uint8_t brightness = this->GetFadedBrightness();
    uint32_t maxPower = this->config->GetMaxCurrent() * LEDVoltage;
    uint32_t power = calculate_unscaled_power_mW(this->leds, NumberOfLEDs) * brightness / 256U;

    if (power > maxPower)
    {
        brightness = (uint32_t)brightness * maxPower / power;
        power = maxPower;
    }

    this->current = power / LEDVoltage;

    // show() turns dithering off whenever FPS is under 100, which includes the first frames after boot, so turn it back on every frame
    FastLED.setDither(BINARY_DITHER);
    FastLED.show(brightness);
}

uint8_t AnimationController::GetFadedBrightness()
{
    uint8_t target = (uint32_t)this->config->GetBrightness() * this->config->GetMaxBrightness() / 100U;

    if (target != this->brightnessTarget)
    {
        this->brightnessFrom = this->brightness;
        this->brightnessTarget = target;
        this->brightnessChangedAt = millis();
    }

    unsigned long elapsed = millis() - this->brightnessChangedAt;
    unsigned long duration = this->config->GetTransitionDuration();

    this->brightness = elapsed >= duration ?
        target :
        lerp8by8(this->brightnessFrom, target, UINT8_MAX * elapsed / duration);

    return this->brightness;
}

AnimationType AnimationController::NextAnimation()
{
    int32_t exclude = -1;

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        if
        (
            this->animations[i] != nullptr &&
            (
                this->animations[i]->GetStatus() == ANIMATION_STATUS_PLAYING ||
                this->animations[i]->GetStatus() == ANIMATION_STATUS_WAKING_UP
            )
        )
        {
            exclude = i;

            break;
        }
    }

    int32_t next;
    const int32_t offset = 2;

    for (int32_t i = 0; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        next = random(ANIMATION_TYPE_NUMBER_OF_ANIMATIONS - offset) + offset;

        if (next != exclude && this->animations[next] != nullptr)
        {
            return (AnimationType)next;
        }
    }

    for (int32_t i = offset; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        if (this->animations[i] != nullptr)
        {
            return (AnimationType)i;
        }
    }

    return ANIMATION_TYPE_STRIP_TEST;
}
