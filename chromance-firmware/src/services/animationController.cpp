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
    transitionScale(0),
    next(ANIMATION_REQUEST_NONE),
    ripplePool()
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
    FastLED.setBrightness(StartupBrightness);
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
    
    this->lastRandomAnimationStarted = millis() + StartupDelay;
    
    // The constructor sets this to random currently but in future this can be from config
    if (this->currentAnimationType == ANIMATION_TYPE_RANDOM_ANIMATION)
    {
        Animation* nextAnimation = this->animations[this->NextAnimation()];

        if (!config->GetSleeping())
        {
            nextAnimation->Wake(true);
        }
    }
    else if (this->animations[this->currentAnimationType] != nullptr)
    {
        if (!config->GetSleeping())
        {
            if (this->currentAnimationType == ANIMATION_TYPE_STRIP_TEST)
            {
                this->animations[this->currentAnimationType]->Wake(false);
            }
            else
            {
                this->animations[this->currentAnimationType]->Wake(true);
            }
        }
    }
}

void AnimationController::Loop()
{
    if (xSemaphoreTake(this->semaphore, 0U) == pdTRUE)
    {
        this->HandleBrightness();
        this->HandleAnimationRequest();
        this->HandleRandomAnimation();
        
        xSemaphoreGive(this->semaphore);
    }

    this->Render();

    FastLED.delay(TaskDelay);
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
        animationType == ANIMATION_TYPE_NUMBER_OF_ANIMATIONS
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

Animation* AnimationController::GetAnimation(AnimationType animationType)
{
    return this->animations[animationType];
}

void AnimationController::HandleBrightness()
{
    uint8_t configBrightness = this->config->GetBrightness();
        
    if (FastLED.getBrightness() != configBrightness)
    {
        FastLED.setBrightness(configBrightness);
    }
}

void AnimationController::HandleAnimationRequest()
{
    if (this->next == ANIMATION_REQUEST_PLAY)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(false);
        this->transitionScale = 0;

        for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
        {
            if (this->animations[i] != nullptr)
            {
                this->animations[i]->Sleep(false);
            }
        }

        if (this->currentAnimationType == ANIMATION_TYPE_RANDOM_ANIMATION)
        {
            Animation* nextAnimation = this->animations[this->NextAnimation()];

            nextAnimation->Wake(false);
        }
        else
        {
            this->animations[this->currentAnimationType]->Wake(false);
        }
    }
    else if (this->next == ANIMATION_REQUEST_SLEEP)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(true);

        for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
        {
            if (this->animations[i] != nullptr)
            {
                this->animations[i]->Sleep(true);
            }
        }
    }
    else if (this->next == ANIMATION_REQUEST_WAKE)
    {
        this->next = ANIMATION_REQUEST_NONE;
        this->config->SetSleeping(false);
        this->lastRandomAnimationStarted = millis();

        if (this->currentAnimationType == ANIMATION_TYPE_RANDOM_ANIMATION)
        {
            this->animations[this->NextAnimation()]->Wake(true);
        }
        else
        {
            this->animations[this->currentAnimationType]->Wake(true);
        }
    }
}

void AnimationController::HandleRandomAnimation()
{
    unsigned long now = millis();

    if
    (
        !config->GetSleeping() &&
        this->currentAnimationType == ANIMATION_TYPE_RANDOM_ANIMATION &&
        now - this->lastRandomAnimationStarted > RandomAnimationDuration
    )
    {
        this->lastRandomAnimationStarted = now;
        
        Animation* animation = this->animations[this->NextAnimation()];
        
        for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
        {
            if (this->animations[i] != nullptr && i != animation->GetID())
            {
                this->animations[i]->Sleep(false);
            }
        }

        animation->Wake(false);
        this->transitionScale = 0;
    }
}

void AnimationController::Render()
{
    Animation* animation;
    uint32_t activeAnimationsCount = 0;
    Animation* activeAnimations[ANIMATION_TYPE_NUMBER_OF_ANIMATIONS - 1];

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        animation = this->animations[i];

        if (animation == nullptr)
        {
            continue;
        }
        
        switch (animation->GetStatus())
        {
            case ANIMATION_STATUS_PLAYING:
                animation->Loop();
                activeAnimations[activeAnimationsCount] = animation;
                activeAnimationsCount++;
                break;
            case ANIMATION_STATUS_WAKING_UP:
            case ANIMATION_STATUS_GOING_TO_SLEEP:
                animation->Loop();
                animation->Transition();
                activeAnimations[activeAnimationsCount] = animation;
                activeAnimationsCount++;
                break;
            default:
                break;
        }
    }

    if (activeAnimationsCount > 0)
    {
        if (activeAnimationsCount == 1)
        {
            CRGB* currentAnimationBuffer = activeAnimations[0]->GetBuffer();

            for (int32_t i = 0; i < NumberOfLEDs; i++)
            {
                this->leds[i] = currentAnimationBuffer[i];
            }
        }
        else
        {
            Animation* wakingUpAnimation;
            CRGB* wakingUpAnimationBuffer;
            Animation* goingToSleepAnimation;
            CRGB* goingToSleepAnimationBuffer;
            uint32_t wakingUpIndex;
            uint32_t goingToSleepIndex;

            if (activeAnimations[0]->GetStatus() == ANIMATION_STATUS_WAKING_UP || activeAnimations[0]->GetStatus() == ANIMATION_STATUS_PLAYING)
            {
                wakingUpIndex = 0;
                goingToSleepIndex = 1;
            }
            else
            {
                wakingUpIndex = 1;
                goingToSleepIndex = 0;
            }

            wakingUpAnimation = activeAnimations[wakingUpIndex];
            wakingUpAnimationBuffer = activeAnimations[wakingUpIndex]->GetBuffer();
            goingToSleepAnimation = activeAnimations[goingToSleepIndex];
            goingToSleepAnimationBuffer = activeAnimations[goingToSleepIndex]->GetBuffer();

            if (this->transitionScale <= 255U - AnimationTransitionSpeed)
            {
                this->transitionScale += AnimationTransitionSpeed;

                for (int32_t i = 0; i < NumberOfLEDs; i++)
                {
                    this->leds[i] = blend(goingToSleepAnimationBuffer[i], wakingUpAnimationBuffer[i], this->transitionScale);
                }
            }
            else
            {
                for (int32_t i = 0; i < NumberOfLEDs; i++)
                {
                    this->leds[i] = wakingUpAnimationBuffer[i];
                }
            }
        }
    }
    else
    {
        FastLED.clear();
    }

    FastLED.show();
    FastLED.countFPS();
}

AnimationType AnimationController::NextAnimation()
{
    int32_t exclude = -1;

    for (int32_t i = 1; i < ANIMATION_TYPE_NUMBER_OF_ANIMATIONS; i++)
    {
        if (this->animations[i] != nullptr && this->animations[i]->GetStatus() == ANIMATION_STATUS_PLAYING)
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

    return ANIMATION_TYPE_CUBE_PULSE;
}
