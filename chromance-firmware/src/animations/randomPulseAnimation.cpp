#include "randomPulseAnimation.h"
#include "ripples/map.h"

using namespace Chromance;

RandomPulseAnimation::RandomPulseAnimation(int32_t id, RipplePool* ripplePool, Config* config, Logger* logger) :
    RippleAnimation(id, "Random Pulse", ripplePool, config, logger),
    lastPulseNode(INT32_MAX)
{
}

void RandomPulseAnimation::Start()
{
    float speed = random(100) / 100.0f * 0.5f + this->GetSpeed();

    int32_t node = this->randomPulseNodes[random(RandomPulseAnimation::NumberOfRandomPulseNodes)];

    while (node == this->lastPulseNode)
    {
        node = randomPulseNodes[random(NumberOfRandomPulseNodes)];
    }

    this->lastPulseNode = node;

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
            speed,
            this->GetLifespan(),
            RIPPLE_BEHAVIOR_FEISTY
        );
    }    
}