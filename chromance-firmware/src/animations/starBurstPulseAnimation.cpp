#include "starBurstPulseAnimation.h"
#include "chromance/map.h"

using namespace Chromance;

StarBurstPulseAnimation::StarBurstPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "starBurstPulse", ripplePool, logger)
{
}

void StarBurstPulseAnimation::Start()
{
    uint32_t baseColor = random(0xFFFF);
    float speed = 2.4f;
    unsigned long lifespan = 2600U;
    
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
            StarburstNode,
            i,
            CHSV(baseColor + (0xFFFF / 6) * i, 255U, 255U),
            speed,
            lifespan,
            behavior
        );
    }    
}