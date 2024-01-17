#include "randomPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

RandomPulseAnimation::RandomPulseAnimation(int32_t id, RipplePool* ripplePool, Logger* logger) :
    RippleAnimation(id, "Random Pulse", ripplePool, logger, 3000UL, 225U),
    speed(1.0f),
    lifespan(3500UL),
    lastPulseNode(INT32_MAX)
{
}

void RandomPulseAnimation::Start()
{
    this->speed = random(100) / 100.0f * 0.5f + 1.0f;

    int32_t node = this->randomPulseNodes[random(RandomPulseAnimation::NumberOfRandomPulseNodes)];

    while (node == this->lastPulseNode)
    {
        node = randomPulseNodes[random(NumberOfRandomPulseNodes)];
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
            CHSV(random8(), 255U, 255U),
            this->speed,
            this->lifespan,
            behavior
        );
    }    
}