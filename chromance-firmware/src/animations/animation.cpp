#include "animation.h"

using namespace Chromance;

Animation::Animation(uint8_t id, const char* name, Logger* logger) :
    id(id),
    name(name),
    transitionScale(0),
    status(AnimationStatus::Sleeping),
    fade(false)
{
    this->logger = logger;
    
    fill_solid(this->leds, NumberOfLEDs, CRGB::Black);
}

void Animation::Sleep(bool fade)
{
    if (this->status == AnimationStatus::Playing || this->status == AnimationStatus::WakingUp)
    {
        this->fade = fade;
        this->status = AnimationStatus::GoingToSleep;
    }
}

void Animation::Wake(bool fade)
{
    if (this->status == AnimationStatus::Sleeping || this->status == AnimationStatus::GoingToSleep)
    {
        this->fade = fade;
        this->status = AnimationStatus::WakingUp;
    }
}

uint8_t Animation::GetID()
{
    return this->id;
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
    if (this->status == AnimationStatus::WakingUp)
    {
        if (this->transitionScale < 255 - AnimationTransitionSpeed)
        {
            this->transitionScale += AnimationTransitionSpeed;
        }
        else
        {
            this->transitionScale = 255;
            this->status = AnimationStatus::Playing;
        }

        if (this->fade)
        {
            this->Fade();
        }
    }
    else if (this->status == AnimationStatus::GoingToSleep)
    {
        if (this->transitionScale > AnimationTransitionSpeed)
        {
            this->transitionScale -= AnimationTransitionSpeed;
        }
        else
        {
            this->transitionScale = 0;
            this->status = AnimationStatus::Sleeping;
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
