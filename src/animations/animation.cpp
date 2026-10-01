#include "animation.h"

using namespace Chromance;

Animation::Animation(int32_t id, const char* name, Config* config, Logger* logger) :
    id(id),
    name(name),
    transitionScale(0),
    transitionStartScale(0),
    transitionStartedAt(0),
    status(ANIMATION_STATUS_SLEEPING),
    lastElapsedAt(0UL)
{
    this->config = config;
    this->logger = logger;

    fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
}

void Animation::Sleep(bool fade)
{
    if (this->status != ANIMATION_STATUS_PLAYING && this->status != ANIMATION_STATUS_WAKING_UP)
    {
        return;
    }

    if (fade)
    {
        this->transitionStartScale = this->transitionScale;
        this->transitionStartedAt = millis();
        this->status = ANIMATION_STATUS_GOING_TO_SLEEP;
    }
    else
    {
        this->transitionScale = 0;
        this->status = ANIMATION_STATUS_SLEEPING;
        this->Reset();
    }
}

void Animation::Wake(bool fade)
{
    if (this->status != ANIMATION_STATUS_SLEEPING && this->status != ANIMATION_STATUS_GOING_TO_SLEEP)
    {
        return;
    }

    if (this->status == ANIMATION_STATUS_SLEEPING)
    {
        this->Reset();
    }

    if (fade)
    {
        this->transitionStartScale = this->transitionScale;
        this->transitionStartedAt = millis();
        this->status = ANIMATION_STATUS_WAKING_UP;
    }
    else
    {
        this->transitionScale = UINT8_MAX;
        this->status = ANIMATION_STATUS_PLAYING;
    }
}

uint8_t Animation::GetID()
{
    return this->id;
}

AnimationType Animation::GetAnimationType()
{
    return (AnimationType)this->id;
}

const char* Animation::GetName()
{
    return this->name;
}

CRGB* Animation::GetBuffer()
{
    return this->leds;
}

AnimationStatus Animation::GetStatus()
{
    return this->status;
}

uint8_t Animation::GetTransitionScale()
{
    return this->transitionScale;
}

void Animation::Transition()
{
    if (this->status != ANIMATION_STATUS_WAKING_UP && this->status != ANIMATION_STATUS_GOING_TO_SLEEP)
    {
        return;
    }

    // Steps from the scale the transition started at so a reversed transition carries on from where it was
    unsigned long elapsed = millis() - this->transitionStartedAt;
    unsigned long duration = this->config->GetTransitionDuration();
    uint8_t step = elapsed >= duration ? UINT8_MAX : (uint8_t)(UINT8_MAX * elapsed / duration);

    if (this->status == ANIMATION_STATUS_WAKING_UP)
    {
        if (this->transitionStartScale < UINT8_MAX - step)
        {
            this->transitionScale = this->transitionStartScale + step;
        }
        else
        {
            this->transitionScale = UINT8_MAX;
            this->status = ANIMATION_STATUS_PLAYING;
        }
    }
    else
    {
        if (this->transitionStartScale > step)
        {
            this->transitionScale = this->transitionStartScale - step;
        }
        else
        {
            this->transitionScale = 0;
            this->status = ANIMATION_STATUS_SLEEPING;
            this->Reset();
        }
    }
}

bool Animation::IsRippleAnimation()
{
    return false;
}

void Animation::Reset()
{
    fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
    this->lastElapsedAt = micros();
}

float Animation::GetSpeed()
{
    return this->config->GetAnimationSpeed(this->GetAnimationType());
}

float Animation::GetParameter(uint8_t index)
{
    return this->config->GetAnimationParameter(this->GetAnimationType(), index);
}

float Animation::GetElapsedSeconds()
{
    unsigned long now = micros();
    float seconds = (now - this->lastElapsedAt) / 1000000.0f;

    this->lastElapsedAt = now;

    return seconds * this->GetSpeed();
}
