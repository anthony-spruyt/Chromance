#include "randomPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

RandomPulseAnimation::RandomPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Random Pulse", ripplePool, logger),
    lastPulseNode(255U)
{
}

void RandomPulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    float speed = random(100) / 100.0f * 0.8f + 1.8f;
    unsigned long lifespan = 3500UL;

    int32_t node = FunNodes[random(NumberOfFunNodes)];

    while (node == this->lastPulseNode)
    {
        node = CubeNodes[random(NumberOfCubeNodes)];
    }

    this->lastPulseNode = node;

    RippleBehavior behavior = RIPPLE_BEHAVIOR_FEISTY;
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
            node,
            i,
            color,
            speed,
            lifespan,
            behavior
        );
    }    
}