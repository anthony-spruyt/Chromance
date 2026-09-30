#include "aroundTheWorldAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

AroundTheWorldAnimation::AroundTheWorldAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger) :
    RippleAnimation(id, "Around the World", ripplePool, config, logger)
{
}

void AroundTheWorldAnimation::Start()
{
    Ripple* ripple = this->ripplePool->Claim(this->id);

    if (ripple == nullptr)
    {
        this->logger->Warn("The ripple pool is empty");

        return;
    }

    float speed = random(100) / 100.0f * 1.0f + this->GetSpeed();

    ripple->Start
    (
        BottomNode,
        1,
        CHSV(random8(), UINT8_MAX, UINT8_MAX),
        speed,
        this->GetLifespan(),
        RIPPLE_BEHAVIOR_ALWAYS_LEFT
    );  
}