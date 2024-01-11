/*
   A dot animation that travels along rails
   (C) Voidstar Lab LLC 2021
*/

#include "ripple.h"

using namespace Chromance;

Ripple::Ripple() :
    state(RippleState::Dead),
    color(CRGB::Black),
    speed(0.0f),
    lifespan(0U),
    behavior(RippleBehavior::CouchPotato),
    justStarted(false),
    pressure(0.0f),
    birthday(0U),
    node(0),
    direction(0)

{
}

void Ripple::Start(int node, int direction, CRGB color, float speed, unsigned long lifespan, RippleBehavior behavior)
{
    this->node = node;
    this->direction = direction;
    this->color = color;
    this->speed = speed;
    this->lifespan = lifespan;
    this->behavior = behavior;

    this->birthday = millis();
    this->pressure = 0.0f;
    this->state = RippleState::WithinNode;
    this->justStarted = true;
}

void Ripple::Advance(CRGB* leds)
{
    unsigned long age = millis() - this->birthday;

    if (this->state == RippleState::Dead)
    {
        return;
    }

    // Ripple slows down as it ages
    this->pressure += Fmap(float(age), 0.0f, float(this->lifespan), this->speed, 0.0f);
    
    // TODO: Motion of ripple is severely affected by loop speed. Make it time invariant

    if (this->pressure < 1.0f && (this->state == RippleState::TravelUp || this->state == RippleState::TravelDown))
    {
        // Ripple is visible but hasn't moved - render it to avoid flickering
        this->Render(leds, age);
    }

    while (this->pressure >= 1)
    {
        switch (this->state)
        {
            case RippleState::WithinNode:
            {
                if (this->justStarted)
                {
                    this->justStarted = false;
                }
                else
                {
                    int newDirection = -1;
                    int sharpLeft = (this->direction + 1) % MaxPathsPerNode;
                    int wideLeft = (this->direction + 2) % MaxPathsPerNode;
                    int forward = (this->direction + 3) % MaxPathsPerNode;
                    int wideRight = (this->direction + 4) % MaxPathsPerNode;
                    int sharpRight = (this->direction + 5) % MaxPathsPerNode;

                    if (this->behavior <= RippleBehavior::Angry)
                    { 
                        // Semi-random aggressive turn mode
                        // The more aggressive a ripple, the tighter turns it wants to make.
                        // If there aren't any segments it can turn to, we need to adjust its behavior.
                        int anger = (int)this->behavior;
                        int forwardConnection = NodeConnections[node][forward];

                        while (newDirection < 0)
                        {
                            if (anger == (int)RippleBehavior::CouchPotato)
                            {
                                // We can't go straight ahead - we need to take a rest
                                // Die now
                                age = this->lifespan;

                                break;
                            }

                            if (anger == (int)RippleBehavior::Lazy)
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

                            if (anger == (int)RippleBehavior::Weak)
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

                            if (anger == (int)RippleBehavior::Feisty)
                            {
                                int leftConnection = NodeConnections[node][wideLeft];
                                int rightConnection = NodeConnections[node][wideRight];

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

                            if (anger == (int)RippleBehavior::Angry)
                            {
                                int leftConnection = NodeConnections[node][sharpLeft];
                                int rightConnection = NodeConnections[node][sharpRight];

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
                    else if (this->behavior == RippleBehavior::AlwaysRight)
                    {
                        for (int i = 1; i < MaxPathsPerNode; i++)
                        {
                            int possibleDirection = (this->direction + i) % MaxPathsPerNode;

                            if (NodeConnections[this->node][possibleDirection] >= 0)
                            {
                                newDirection = possibleDirection;

                                break;
                            }
                        }
                    }
                    else if (this->behavior == RippleBehavior::AlwaysLeft)
                    {
                        for (int i = 5; i >= 1; i--)
                        {
                            int possibleDirection = (this->direction + i) % MaxPathsPerNode;

                            if (NodeConnections[this->node][possibleDirection] >= 0)
                            {
                                newDirection = possibleDirection;

                                break;
                            }
                        }
                    }
                    else if (this->behavior == RippleBehavior::Exploding)
                    {
                        for (int i = 5; i >= 1; i--)
                        {
                            int possibleDirection = (this->direction + i) % MaxPathsPerNode;

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
                    this->state = RippleState::TravelUp;
                    this->direction = 0;  // Starting at bottom of segment
                }
                else
                {
                    this->state = RippleState::TravelDown;
                    this->direction = LEDsPerSegment - 1; // Starting at top of LED-strip
                }

                break;
            }

            case RippleState::TravelUp:
            {
                this->direction++;

                if (this->direction >= LEDsPerSegment)
                {
                    // We've reached the top!
                    // Enter the new node.
                    int segment = this->node;

                    this->node = SegmentConnections[segment][0];

                    for (int i = 0; i < MaxPathsPerNode; i++)
                    {
                        // Figure out from which direction the ripple is entering the node.
                        // Allows us to exit in an appropriately aggressive direction.
                        int incomingConnection = NodeConnections[this->node][i];

                        if (incomingConnection == segment)
                        {
                            this->direction = i;
                        }
                    }

                    this->state = RippleState::WithinNode;
                }

                break;
            }

            case RippleState::TravelDown:
            {
                this->direction--;

                if (this->direction < 0)
                {
                    // We've reached the bottom!
                    // Enter the new node.
                    int segment = this->node;

                    this->node = SegmentConnections[segment][1];

                    for (int i = 0; i < MaxPathsPerNode; i++)
                    {
                        // Figure out from which direction the ripple is entering the node.
                        // Allows us to exit in an appropriately aggressive direction.
                        int incomingConnection = NodeConnections[this->node][i];

                        if (incomingConnection == segment)
                        {
                            this->direction = i;
                        }
                    }

                    this->state = RippleState::WithinNode;
                }

                break;
            }

            default:
                break;
        }

        this->pressure -= 1.0f;

        if (this->state == RippleState::TravelUp || this->state == RippleState::TravelDown)
        {
            // Ripple is visible - render it
            this->Render(leds, age);
        }
    }

    if (this->lifespan && age >= this->lifespan)
    {
        // We Dead
        this->state = RippleState::Dead;
        this->node = this->direction = this->pressure = age = 0;
    }
}

void Ripple::Claim(uint8_t animationId)
{
    this->animationId = animationId;
}

RippleState Ripple::GetState()
{
    return this->state;
}

uint8_t Ripple::GetAnimationId()
{
    return this->animationId;
}

void Ripple::Render(CRGB* leds, unsigned long age)
{
    int segment = this->node;
    int fromBottom = this->direction;    
    int strip = LEDAssignments[segment][0];
    int led = round
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
    uint16_t offset = strip == BlueStripIndex ?
        BlueStripOffset :
            strip == GreenStripIndex ?
                GreenStripOffset :
                    strip == RedStripIndex ?
                        RedStripOffset :
                        BlackStripOffset;
    uint32_t color = (uint32_t)this->color;
    uint8_t r = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), (color >> 8) & 0xFF, 0.0f)) + this->color.r)));
    uint8_t g = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), (color >> 16) & 0xFF, 0.0f)) + this->color.g)));
    uint8_t b = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), color & 0xFF, 0.0f)) + this->color.b)));

    leds[offset + led].setRGB(r, g, b);
}

/*void render()
{
    uint32_t color = (uint32_t)this->color;
    uint8_t ledColors[40][14][3];
    int segment = node;
    int strip = LEDAssignments[segment][0];
    int led = LEDAssignments[segment][2] + direction;
    int red = ledColors[segment][direction][0];
    int green = ledColors[segment][direction][1];
    int blue = ledColors[segment][direction][2];

    ledColors[segment][direction][0] = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), (color >> 8) & 0xFF, 0.0f)) + red)));
    ledColors[segment][direction][1] = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), (color >> 16) & 0xFF, 0.0f)) + green)));
    ledColors[segment][direction][2] = uint8_t(min(255, max(0, int(Fmap(float(age), 0.0, float(lifespan), color & 0xFF, 0.0f)) + blue)));

    for (int segment = 0; segment < NumberOfSegments; segment++)
    {
        for (int fromBottom = 0; fromBottom < LEDsPerSegment; fromBottom++)
        {
            int strip = LEDAssignments[segment][0];
            int led = round
            (
                Fmap
                (
                    fromBottom,
                    0, 
                    (LEDsPerSegment - 1),
                    LEDAssignments[segment][2], 
                    LEDAssignments[segment][1]
                )
            );
            
            strips[strip].setPixelColor
            (
                led,
                ledColors[segment][fromBottom][0],
                ledColors[segment][fromBottom][1],
                ledColors[segment][fromBottom][2]
            );
        }
    }
}*/