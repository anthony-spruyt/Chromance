#include "animationController.h"
#include "../animations/cubePulseAnimation.h"
#include "../animations/pulseAnimation.h"
#include "../animations/rainbowBeatAnimation.h"
#include "../animations/rainbowMarchAnimation.h"
#include "../animations/stripTestAnimation.h"
#include "../animations/starBurstPulseAnimation.h"
#include "../animations/centerPulseAnimation.h"
#include "../animations/RandomPulseAnimation.h"

using namespace Chromance;

AnimationController::AnimationController(Logger* logger, Config* config) :
    //currentAnimationType(AnimationType::RandomAnimation),
    currentAnimationType(AnimationType::RandomPulse),
    sleeping(false),
    lastRandomAnimationStarted(0),
    transitionScale(0),
    next(AnimationRequest::None),
    ripplePool()
{
    this->logger = logger;
    this->config = config;
    this->semaphore = xSemaphoreCreateMutex();
}

AnimationController::~AnimationController()
{
    Animation* animation = nullptr;

    for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
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
    this->logger->Info("Configure LED controller");

    random16_set_seed(esp_random());

    this->logger->Debug("Add strips to FastLED");
    FastLED.addLeds<NEOPIXEL, BlueStripDataPin>(this->leds, BlueStripOffset, BlueStripLength);
    FastLED.addLeds<NEOPIXEL, GreenStripDataPin>(this->leds, GreenStripOffset, GreenStripLength);
    FastLED.addLeds<NEOPIXEL, RedStripDataPin>(this->leds, RedStripOffset, RedStripLength);
    FastLED.addLeds<NEOPIXEL, BlackStripDataPin>(this->leds, BlackStripOffset, BlackStripLength);

    this->logger->Debug("Set max refreshrate: " + String(MaxRefreshRate) + "Hz");
    FastLED.setMaxRefreshRate(MaxRefreshRate);
    this->logger->Debug("Set startup brightness: " + String((float)StartupBrightness / 255.0f * 100.0f, 0) + "%");
    FastLED.setBrightness(StartupBrightness);
    this->logger->Debug("Set color correction: " + String(TypicalLEDStrip));
    FastLED.setCorrection(TypicalLEDStrip);

    FastLED.clear();
    FastLED.show();

    this->logger->Debug("Register and initialize animations");
    this->animations[(uint8_t)AnimationType::RandomAnimation] = nullptr;
    this->animations[(uint8_t)AnimationType::StripTest] = new StripTestAnimation((uint8_t)AnimationType::StripTest, this->logger);
    this->animations[(uint8_t)AnimationType::RandomPulse] = RandomPulseAnimationEnabled ? new RandomPulseAnimation((uint8_t)AnimationType::RandomPulse, &ripplePool, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::CubePulse] = CubePulseAnimationEnabled ? new CubePulseAnimation((uint8_t)AnimationType::CubePulse, &ripplePool, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::StarBurstPulse] = StarBurstPulseAnimationEnabled ? new StarBurstPulseAnimation((uint8_t)AnimationType::StarBurstPulse, &ripplePool, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::CenterPulse] = CenterPulseAnimationEnabled ? new CenterPulseAnimation((uint8_t)AnimationType::CenterPulse, &ripplePool, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::RainbowBeat] = RainbowBeatAnimationEnabled ? new RainbowBeatAnimation((uint8_t)AnimationType::RainbowBeat, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::RainbowMarch] = RainbowMarchAnimationEnabled ? new RainbowMarchAnimation((uint8_t)AnimationType::RainbowMarch, this->logger) : nullptr;
    this->animations[(uint8_t)AnimationType::Pulse] = PulseAnimationEnabled ? new PulseAnimation((uint8_t)AnimationType::Pulse, this->logger) : nullptr;
    
    this->lastRandomAnimationStarted = millis() + StartupDelay;
    
    // The constructor sets this to random currently but in future this can be from config
    if (this->currentAnimationType == AnimationType::RandomAnimation)
    {
        Animation* nextAnimation = this->animations[(uint8_t)this->RandomAnimation()];

        this->logger->Debug("Random animation selected: " + String(nextAnimation->GetName()));

        nextAnimation->Wake(true);
    }
    else if (this->animations[(uint8_t)currentAnimationType] != nullptr)
    {
         if (this->currentAnimationType == AnimationType::StripTest)
        {
            this->animations[(uint8_t)currentAnimationType]->Wake(false);
        }
        else
        {
            this->animations[(uint8_t)currentAnimationType]->Wake(true);
        }
    }
}

void AnimationController::Loop()
{
    unsigned long now = millis();
    uint8_t configBrightness = this->config->GetBrightness();
    Animation* animation;
    uint8_t activeAnimationsCount = 0;
    Animation* activeAnimations[(uint8_t)AnimationType::NumberOfAnimations - 1];

    if (xSemaphoreTake(this->semaphore, 0U) == pdTRUE)
    {
        if (this->next == AnimationRequest::Play)
        {
            this->next = AnimationRequest::None;
            this->sleeping = false;
            this->transitionScale = 0;

            for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
            {
                if (this->animations[i] != nullptr)
                {
                    this->animations[i]->Sleep(false);
                }
            }

            if (this->currentAnimationType == AnimationType::RandomAnimation)
            {
                Animation* nextAnimation = this->animations[(uint8_t)this->RandomAnimation()];

                this->logger->Debug("Random animation selected: " + String(nextAnimation->GetName()));

                nextAnimation->Wake(false);
            }
            else
            {
                this->animations[(uint8_t)this->currentAnimationType]->Wake(false);
            }
        }
        else if (this->next == AnimationRequest::Sleep)
        {
            this->next = AnimationRequest::None;
            this->sleeping = true;

            for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
            {
                if (this->animations[i] != nullptr)
                {
                    this->animations[i]->Sleep(true);
                }
            }
        }
        else if (this->next == AnimationRequest::Wake)
        {
            this->next = AnimationRequest::None;
            this->sleeping = false;
            this->lastRandomAnimationStarted = now;

            if (this->currentAnimationType == AnimationType::RandomAnimation)
            {
                this->animations[(uint8_t)this->RandomAnimation()]->Wake(true);
            }
            else
            {
                this->animations[(uint8_t)this->currentAnimationType]->Wake(true);
            }
        }

        if (FastLED.getBrightness() != configBrightness)
        {
            this->logger->Debug("Apply brightness change: " + String((float)configBrightness / 255.0f * 100.0f, 0) + "%");
            FastLED.setBrightness(configBrightness);
        }

        if
        (
            !this->sleeping &&
            this->currentAnimationType == AnimationType::RandomAnimation &&
            now - this->lastRandomAnimationStarted > RandomAnimationDuration
        )
        {
            this->lastRandomAnimationStarted = now;
            
            animation = this->animations[(uint8_t)this->RandomAnimation()];
            
            this->logger->Debug("Next random animation selected: " + String(animation->GetName()));

            for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
            {
                if (this->animations[i] != nullptr && i != animation->GetID())
                {
                    this->animations[i]->Sleep(false);
                }
            }

            animation->Wake(false);
            this->transitionScale = 0;
        }

        xSemaphoreGive(this->semaphore);
    }

    for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
    {
        animation = this->animations[i];

        if (animation == nullptr)
        {
            continue;
        }
        
        switch (animation->GetStatus())
        {
            case AnimationStatus::Playing:
                animation->Loop();
                activeAnimations[activeAnimationsCount] = animation;
                activeAnimationsCount++;
                break;
            case AnimationStatus::WakingUp:
            case AnimationStatus::GoingToSleep:
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
        if (activeAnimationsCount > 2)
        {
            this->logger->Warn("More than 2 concurrent animations is not supported");
        }

        if (activeAnimationsCount == 1)
        {
            CRGB* currentAnimationBuffer = activeAnimations[0]->GetBuffer();

            for (int i = 0; i < NumberOfLEDs; i++)
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
            uint8_t wakingUpIndex;
            uint8_t goingToSleepIndex;

            if (activeAnimations[0]->GetStatus() == AnimationStatus::WakingUp || activeAnimations[0]->GetStatus() == AnimationStatus::Playing)
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

            if (this->transitionScale <= 255 - AnimationTransitionSpeed)
            {
                this->transitionScale += AnimationTransitionSpeed;

                for (int i = 0; i < NumberOfLEDs; i++)
                {
                    this->leds[i] = blend(goingToSleepAnimationBuffer[i], wakingUpAnimationBuffer[i], this->transitionScale);
                }
            }
            else
            {
                for (int i = 0; i < NumberOfLEDs; i++)
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
    FastLED.delay(TaskDelay);
}

void AnimationController::Sleep()
{
    this->logger->Debug("LED controller going to sleep");

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = AnimationRequest::Sleep;

        xSemaphoreGive(this->semaphore);
    }
}

void AnimationController::Wake()
{
    this->logger->Debug("LED controller waking up");

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = AnimationRequest::Wake;

        xSemaphoreGive(this->semaphore);
    }
}

void AnimationController::Play(AnimationType animationType)
{
    if
    (
        animationType == AnimationType::NumberOfAnimations
    )
    {
        return;
    }

    this->logger->Debug("Play animation: " + String((uint8_t)animationType));

    if (xSemaphoreTake(this->semaphore, portMAX_DELAY) == pdTRUE)
    {
        this->next = AnimationRequest::Play;
        this->currentAnimationType = animationType;

        xSemaphoreGive(this->semaphore);
    }
}

AnimationType AnimationController::RandomAnimation()
{
    uint8_t exclude = 255;

    for (int i = 1; i < (int)AnimationType::NumberOfAnimations; i++)
    {
        if (this->animations[i] != nullptr && this->animations[i]->GetStatus() == AnimationStatus::Playing)
        {
            exclude = i;
            
            break;
        }
    }

    uint8_t next;
    uint8_t count = 0;
    const uint16_t offset = 2;

    while (count < 255)
    {
        next = (uint8_t)random16((uint16_t)AnimationType::NumberOfAnimations - offset) + offset;

        if (next != exclude && this->animations[next] != nullptr)
        {
            return (AnimationType)next;
        }

        count++;
    }

    return AnimationType::RainbowMarch;
}
