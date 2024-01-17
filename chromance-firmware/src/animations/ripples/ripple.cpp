/*
   A dot animation that travels along rails
   (C) Voidstar Lab LLC 2021
*/

#include "ripple.h"

using namespace Chromance;

Ripple::Ripple() :
    state(RIPPLE_STATE_DEAD),
    color(0U, 0U, 0U),
    speed(0.0f),
    lifespan(0U),
    behavior(RIPPLE_BEHAVIOR_COUCH_POTATO),
    justStarted(false),
    pressure(0.0f),
    startedAt(0U),
    node(0),
    direction(0)

{
}

void Ripple::Start(int32_t node, int32_t direction, CHSV color, float speed, unsigned long lifespan, RippleBehavior behavior)
{
    this->node = node;
    this->direction = direction;
    this->color = color;
    this->speed = speed;
    this->lifespan = lifespan;
    this->behavior = behavior;

    this->startedAt = millis();
    this->pressure = 0.0f;
    this->state = RIPPLE_STATE_WITHIN_NODE;
    this->justStarted = true;
}

void Ripple::Advance(CRGB* leds)
{
    unsigned long age = millis() - this->startedAt;

    if (this->state == RIPPLE_STATE_DEAD)
    {
        return;
    }

    // Ripple slows down as it ages
    this->pressure += Fmap(float(age), 0.0f, float(this->lifespan), this->speed, 0.0f);
    
    // TODO: Motion of ripple is severely affected by loop speed. Make it time invariant

    if (this->pressure < 1.0f && (this->state == RIPPLE_STATE_TRAVEL_UP || this->state == RIPPLE_STATE_TRAVEL_DOWN))
    {
        // Ripple is visible but hasn't moved - render it to avoid flickering
        this->Render(leds, age);
    }

    while (this->pressure >= 1)
    {
        switch (this->state)
        {
            case RIPPLE_STATE_WITHIN_NODE:
            {
                if (this->justStarted)
                {
                    this->justStarted = false;
                }
                else
                {
                    int32_t newDirection = -1;
                    int32_t sharpLeft = (this->direction + 1) % MaxPathsPerNode;
                    int32_t wideLeft = (this->direction + 2) % MaxPathsPerNode;
                    int32_t forward = (this->direction + 3) % MaxPathsPerNode;
                    int32_t wideRight = (this->direction + 4) % MaxPathsPerNode;
                    int32_t sharpRight = (this->direction + 5) % MaxPathsPerNode;

                    if (this->behavior <= RIPPLE_BEHAVIOR_ANGRY)
                    { 
                        // Semi-random aggressive turn mode
                        // The more aggressive a ripple, the tighter turns it wants to make.
                        // If there aren't any segments it can turn to, we need to adjust its behavior.
                        int32_t anger = this->behavior;
                        int32_t forwardConnection = NodeConnections[node][forward];

                        while (newDirection < 0)
                        {
                            if (anger == RIPPLE_BEHAVIOR_COUCH_POTATO)
                            {
                                // We can't go straight ahead - we need to take a rest
                                // Die now
                                age = this->lifespan;

                                break;
                            }

                            if (anger == RIPPLE_BEHAVIOR_LAZY)
                            {
                                if (forwardConnection < 0)
                                {
                                    // We can't go straight ahead - we need to take a rest
                                    // Die now
                                    age = this->lifespan;

                                    break;
                                }
                                else
                                {
                                    newDirection = forward;
                                }
                            }

                            if (anger == RIPPLE_BEHAVIOR_WEAK)
                            {
                                if (forwardConnection < 0)
                                {
                                    // We can't go straight ahead - we need to take a more aggressive angle
                                    anger++;
                                }
                                else
                                {
                                    newDirection = forward;
                                }
                            }

                            if (anger == RIPPLE_BEHAVIOR_FEISTY)
                            {
                                int32_t leftConnection = NodeConnections[node][wideLeft];
                                int32_t rightConnection = NodeConnections[node][wideRight];

                                if (leftConnection >= 0 && rightConnection >= 0)
                                {
                                    newDirection = random(2) ? wideLeft : wideRight;
                                }
                                else if (leftConnection >= 0)
                                {
                                    newDirection = wideLeft;
                                }
                                else if (rightConnection >= 0)
                                {
                                    newDirection = wideRight;
                                }
                                else
                                {
                                    anger++;  // Can't take shallow turn - must become more aggressive
                                }
                            }

                            if (anger == RIPPLE_BEHAVIOR_ANGRY)
                            {
                                int32_t leftConnection = NodeConnections[node][sharpLeft];
                                int32_t rightConnection = NodeConnections[node][sharpRight];

                                if (leftConnection >= 0 && rightConnection >= 0)
                                {
                                    newDirection = random(2) ? sharpLeft : sharpRight;
                                }
                                else if (leftConnection >= 0)
                                {
                                    newDirection = sharpLeft;
                                }
                                else if (rightConnection >= 0)
                                {
                                    newDirection = sharpRight;
                                }
                                else
                                {
                                    anger--;  // Can't take tight turn - must become less aggressive
                                }
                            }

                            // Note that this can't handle some circumstances,
                            // like a node with segments in nothing but the 0 and 3 positions.
                            // Good thing we don't have any of those!
                        }
                    }
                    else if (this->behavior == RIPPLE_BEHAVIOR_ALWAYS_RIGHT)
                    {
                        for (int32_t i = 1; i < MaxPathsPerNode; i++)
                        {
                            int32_t possibleDirection = (this->direction + i) % MaxPathsPerNode;

                            if (NodeConnections[this->node][possibleDirection] >= 0)
                            {
                                newDirection = possibleDirection;

                                break;
                            }
                        }
                    }
                    else if (this->behavior == RIPPLE_BEHAVIOR_ALWAYS_LEFT)
                    {
                        for (int32_t i = 5; i >= 1; i--)
                        {
                            int32_t possibleDirection = (this->direction + i) % MaxPathsPerNode;

                            if (NodeConnections[this->node][possibleDirection] >= 0)
                            {
                                newDirection = possibleDirection;

                                break;
                            }
                        }
                    }
                    else if (this->behavior == RIPPLE_BEHAVIOR_EXPLODING)
                    {
                        for (int32_t i = 5; i >= 1; i--)
                        {
                            int32_t possibleDirection = (this->direction + i) % MaxPathsPerNode;

                            if (NodeConnections[this->node][possibleDirection] >= 0 && (possibleDirection != node))
                            {
                                newDirection = possibleDirection;
                                // start ? this is custom from EasyAsABC123 fork
                            }
                        }
                    }

                    if (newDirection >= 0)
                    {
                        this->direction = newDirection;
                    }
                }

                this->node = NodeConnections[this->node][this->direction];  // Look up which segment we're on

                if (node > NumberOfNodes)
                {
                    // oops node out of bounds
                    // TODO log warning
                }

                if (this->direction == 5 || this->direction == 0 || this->direction == 1)
                {
                    // Top half of the node
                    this->state = RIPPLE_STATE_TRAVEL_UP;
                    this->direction = 0;  // Starting at bottom of segment
                }
                else
                {
                    this->state = RIPPLE_STATE_TRAVEL_DOWN;
                    this->direction = LEDsPerSegment - 1; // Starting at top of LED-strip
                }

                break;
            }

            case RIPPLE_STATE_TRAVEL_UP:
            {
                this->direction++;

                if (this->direction >= LEDsPerSegment)
                {
                    // We've reached the top!
                    // Enter the new node.
                    int32_t segment = this->node;

                    this->node = SegmentConnections[segment][0];

                    for (int32_t i = 0; i < MaxPathsPerNode; i++)
                    {
                        // Figure out from which direction the ripple is entering the node.
                        // Allows us to exit in an appropriately aggressive direction.
                        int32_t incomingConnection = NodeConnections[this->node][i];

                        if (incomingConnection == segment)
                        {
                            this->direction = i;
                        }
                    }

                    this->state = RIPPLE_STATE_WITHIN_NODE;
                }

                break;
            }

            case RIPPLE_STATE_TRAVEL_DOWN:
            {
                this->direction--;

                if (this->direction < 0)
                {
                    // We've reached the bottom!
                    // Enter the new node.
                    int32_t segment = this->node;

                    this->node = SegmentConnections[segment][1];

                    for (int32_t i = 0; i < MaxPathsPerNode; i++)
                    {
                        // Figure out from which direction the ripple is entering the node.
                        // Allows us to exit in an appropriately aggressive direction.
                        int32_t incomingConnection = NodeConnections[this->node][i];

                        if (incomingConnection == segment)
                        {
                            this->direction = i;
                        }
                    }

                    this->state = RIPPLE_STATE_WITHIN_NODE;
                }

                break;
            }

            default:
                break;
        }

        this->pressure -= 1.0f;

        if (this->state == RIPPLE_STATE_TRAVEL_UP || this->state == RIPPLE_STATE_TRAVEL_DOWN)
        {
            // Ripple is visible - render it
            this->Render(leds, age);
        }
    }

    if (this->lifespan && age >= this->lifespan)
    {
        // We Dead
        this->state = RIPPLE_STATE_DEAD;
        this->node = this->direction = this->pressure = age = 0;
    }
}

void Ripple::Claim(int32_t animationId)
{
    this->animationId = animationId;
}

RippleState Ripple::GetState()
{
    unsigned long now = millis();

    // I have a suspicion that ripples arent being killed properly over time and then the pool runs out of ripples. Possibly due to transitions?
    if (this->state != RIPPLE_STATE_DEAD)
    {
        if (now - this->startedAt > RandomAnimationDuration)
        {
            this->state = RIPPLE_STATE_DEAD;
            this->node = this->direction = this->pressure = 0;
        }
    }

    return this->state;
}

int32_t Ripple::GetAnimationId()
{
    return this->animationId;
}

unsigned long Ripple::GetStartedAt()
{
    return this->startedAt;
}

void Ripple::Render(CRGB* leds, unsigned long age)
{
    int32_t segment = this->node;
    int32_t fromBottom = this->direction;    
    int32_t strip = LEDAssignments[segment][0];
    int32_t led = round
    (
        Fmap
        (
            (float)fromBottom,
            0.0f, 
            (float)(LEDsPerSegment - 1),
            (float)LEDAssignments[segment][2], 
            (float)LEDAssignments[segment][1]
        )
    );
    uint32_t offset = strip == BlueStripIndex ?
        BlueStripOffset :
            strip == GreenStripIndex ?
                GreenStripOffset :
                    strip == RedStripIndex ?
                        RedStripOffset :
                        BlackStripOffset;
    float p = (float)age / (float)lifespan;
    uint8_t scale = p > 0.99f ? 0 : UINT8_MAX - (uint8_t)(p * UINT8_MAX);
    uint8_t minS = 150U;
    uint8_t sScale = scale > minS ? scale : minS;
    CRGB agedColor = CHSV(this->color.hue, sScale, 255U);
    uint16_t index = offset + led;

    if
    (
        index < NumberOfLEDs &&
        (
            leds[index] == CRGB::Black ||
            agedColor.getLuma() > leds[index].getLuma()
        )
    )
    {
        leds[index] = agedColor;
    }
}