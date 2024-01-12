#include "cubePulseAnimation.h"
#include "chromance/map.h"

using namespace Chromance;

CubePulseAnimation::CubePulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "cubePulse", ripplePool, logger),
    lastPulseNode(255U)
{
}

void CubePulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    float speed = 4.2f;
    unsigned long lifespan = 2000U;
    
    uint8_t node = CubeNodes[random(NumberOfCubeNodes)];

    while (node == this->lastPulseNode)
    {
        node = CubeNodes[random(NumberOfCubeNodes)];
    }

    this->lastPulseNode = node;

    RippleBehavior behavior = random(2) ? 
        RIPPLE_BEHAVIOR_ALWAYS_LEFT : 
        RIPPLE_BEHAVIOR_ALWAYS_RIGHT;
    Ripple* ripple;

    for (int32_t i = 0; i < MaxPathsPerNode; i++)
    {
        if (NodeConnections[node][i] >= 0)
        {
            ripple = this->ripplePool->Claim(this->id);

            if (ripple == nullptr)
            {
                this->logger->Warn("The ripple pool is empty");
                
                break;
            }

            ripple->Start
            (
                node,
                i,
                color,
                speed,
                lifespan,
                behavior
            );
        }
    }
}