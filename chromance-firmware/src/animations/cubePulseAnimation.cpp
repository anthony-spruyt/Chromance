#include "cubePulseAnimation.h"
#include "chromance/map.h"

using namespace Chromance;

CubePulseAnimation::CubePulseAnimation(uint8_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "cubePulse", ripplePool, logger),
    lastPulseNode(255U)
{
}

void CubePulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    uint8_t node = CubeNodes[random(NumberOfCubeNodes)];

    while (node == this->lastPulseNode)
    {
        node = CubeNodes[random(NumberOfCubeNodes)];
    }

    this->lastPulseNode = node;

    RippleBehavior behavior = random(2) ? 
        RippleBehavior::AlwaysLeft : 
        RippleBehavior::AlwaysRight;
    Ripple* ripple;

    for (int i = 0; i < MaxPathsPerNode; i++)
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
                5.0f,
                2000U,
                behavior
            );
        }
    }
}