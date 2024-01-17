#include "starBurstPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

StarBurstPulseAnimation::StarBurstPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Star Burst Pulse", ripplePool, logger, 3000UL, 249U),
    speed(1.35f),
    lifespan(3000UL)
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
            CHSV(baseColor + (0xFFFF / 6) * i, 255U, 255U),
            this->speed,
            this->lifespan,
            behavior
        );
    }    
}