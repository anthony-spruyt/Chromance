#include "starBurstPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

StarBurstPulseAnimation::StarBurstPulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger) :
    RippleAnimation(id, "Star Burst Pulse", ripplePool, config, logger)
{
}

void StarBurstPulseAnimation::Start()
{
    uint32_t baseColor = random(0xFFFF);
    RippleBehavior behavior = random(2) ? 
        RIPPLE_BEHAVIOR_ALWAYS_LEFT : 
        RIPPLE_BEHAVIOR_ALWAYS_RIGHT;
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
            CHSV(baseColor + (0xFFFF / 6) * i, UINT8_MAX, UINT8_MAX),
            this->GetSpeed(),
            this->GetLifespan(),
            behavior
        );
    }    
}