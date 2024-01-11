#include "randomPulseAnimation.h"
#include "chromance/map.h"

using namespace Chromance;

RandomPulseAnimation::RandomPulseAnimation(uint8_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "randomPulse", ripplePool, logger),
    lastPulseNode(255U)
{
}

void RandomPulseAnimation::Start()
{
    CRGB color = CHSV(random8(), 255U, 255U);
    float speed = random(100) / 100.0f * 0.2f + 2.5f;
    unsigned long lifespan = 3000U;

    int node = FunNodes[random(NumberOfFunNodes)];

    while (node == this->lastPulseNode)
    {
        node = CubeNodes[random(NumberOfCubeNodes)];
    }

    this->lastPulseNode = node;

    RippleBehavior behavior = RippleBehavior::Feisty;
    Ripple* ripple;

    for (int i = 0; i < MaxPathsPerNode; i++)
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