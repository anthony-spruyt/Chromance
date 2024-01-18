#include "animation.h"

using namespace Chromance;

Animation::Animation(int32_t id, const char* name, Config* config, Logger* logger) :
    id(id),
    name(name),
    transitionScale(0),
    status(ANIMATION_STATUS_SLEEPING),
    fade(false)
{
    this->config = config;
    this->logger = logger;
    
    fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
}

void Animation::Sleep(bool fade)
{
    if (this->status == ANIMATION_STATUS_PLAYING || this->status == ANIMATION_STATUS_WAKING_UP)
    {
        this->fade = fade;
        this->status = ANIMATION_STATUS_GOING_TO_SLEEP;
    }
}

void Animation::Wake(bool fade)
{
    if (this->status == ANIMATION_STATUS_SLEEPING || this->status == ANIMATION_STATUS_GOING_TO_SLEEP)
    {
        this->fade = fade;
        this->status = ANIMATION_STATUS_WAKING_UP;
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

void Animation::Transition()
{
    if (this->status == ANIMATION_STATUS_WAKING_UP)
    {
        if (this->transitionScale < 255 - AnimationTransitionSpeed)
        {
            this->transitionScale += AnimationTransitionSpeed;
        }
        else
        {
            this->transitionScale = 255;
            this->status = ANIMATION_STATUS_PLAYING;
        }

        if (this->fade)
        {
            this->Fade();
        }
    }
    else if (this->status == ANIMATION_STATUS_GOING_TO_SLEEP)
    {
        if (this->transitionScale > AnimationTransitionSpeed)
        {
            this->transitionScale -= AnimationTransitionSpeed;
        }
        else
        {
            this->transitionScale = 0;
            this->status = ANIMATION_STATUS_SLEEPING;
        }

        if (this->fade)
        {
            this->Fade();
        }
    }
}

void Animation::Fade()
{
    if (this->transitionScale == 0)
    {
        fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
    }
    else if (this->transitionScale != 255)
    {
        nscale8(this->leds, NumberOfLEDs, this->transitionScale);
    }
}

float Animation::GetSpeed()
{
    return this->config->GetAnimationSpeed(this->GetAnimationType());
}