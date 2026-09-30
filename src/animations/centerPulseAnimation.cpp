#include "centerPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

CenterPulseAnimation::CenterPulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger) :
    RippleAnimation(id, "Center Pulse", ripplePool, config, logger)
{
}

void CenterPulseAnimation::Start()
{
    CHSV color = CHSV(random8(), UINT8_MAX, UINT8_MAX);    
    float speed = random(100) / 100.0f * 0.3f + this->GetSpeed();
    Ripple* ripple;

    for (int32_t i = 0; i < MaxPathsPerNode; i++)
    {
        ripple = this->ripplePool->Claim(this->id);

        if (ripple == nullptr)
        {
            this->logger->Warn("The ripple pool is empty");
            
            break;
        }

        ripple->Start
        (
            CenterNode,
            i,
            color,
            speed,
            this->GetLifespan(),
            RIPPLE_BEHAVIOR_FEISTY
        );
    }    
}