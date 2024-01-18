#include "cubePulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

CubePulseAnimation::CubePulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger) :
    RippleAnimation(id, "Cube Pulse", ripplePool, config, logger),
    lastPulseNode(INT32_MAX)
{
}

void CubePulseAnimation::Start()
{   
    uint8_t node = this->cubeNodes[random(NumberOfCubeNodes)];
    CHSV color = CHSV(random8(), 255U, 255U);

    while (node == this->lastPulseNode)
    {
        node = this->cubeNodes[random(NumberOfCubeNodes)];
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
                this->GetSpeed(),
                this->GetLifespan(),
                behavior
            );
        }
    }
}