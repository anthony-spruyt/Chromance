#include "ledMap.h"
#include "ripples/map.h"

using namespace Chromance;

LEDMap::LEDMap()
{
    float minX = 0.0f;
    float maxX = 0.0f;
    float minY = 0.0f;
    float maxY = 0.0f;
    float maxDistance = 0.0f;
    float centerX = NodeCoordinates[CenterNode][0] * HexColumnWidth;
    float centerY = NodeCoordinates[CenterNode][1] * HexRowHeight;
    float ledX;
    float ledY;
    uint32_t led;

    for (int32_t segment = 0; segment < NumberOfSegments; segment++)
    {
        for (uint32_t step = 0; step < LEDsPerSegment; step++)
        {
            this->GetPosition(segment, step, ledX, ledY);

            if (segment == 0 && step == 0)
            {
                minX = maxX = ledX;
                minY = maxY = ledY;
            }

            minX = min(minX, ledX);
            maxX = max(maxX, ledX);
            minY = min(minY, ledY);
            maxY = max(maxY, ledY);
            maxDistance = max(maxDistance, hypotf(ledX - centerX, ledY - centerY));
        }
    }

    for (int32_t segment = 0; segment < NumberOfSegments; segment++)
    {
        for (uint32_t step = 0; step < LEDsPerSegment; step++)
        {
            this->GetPosition(segment, step, ledX, ledY);
            led = SegmentLED(segment, step);

            this->x[led] = lroundf((ledX - minX) * UINT8_MAX / (maxX - minX));
            this->y[led] = lroundf((ledY - minY) * UINT8_MAX / (maxY - minY));
            this->distance[led] = lroundf(hypotf(ledX - centerX, ledY - centerY) * UINT8_MAX / maxDistance);
            // Screen y points down, so -y is 12:00 and +x is 3:00
            this->angle[led] = (uint8_t)lroundf(atan2f(ledX - centerX, centerY - ledY) * 256.0f / TWO_PI);
        }
    }
}

uint8_t LEDMap::GetX(uint32_t led)
{
    return this->x[led];
}

uint8_t LEDMap::GetY(uint32_t led)
{
    return this->y[led];
}

uint8_t LEDMap::GetDistance(uint32_t led)
{
    return this->distance[led];
}

uint8_t LEDMap::GetAngle(uint32_t led)
{
    return this->angle[led];
}

void LEDMap::GetPosition(int32_t segment, uint32_t step, float& x, float& y)
{
    const int32_t* top = NodeCoordinates[SegmentConnections[segment][0]];
    const int32_t* bottom = NodeCoordinates[SegmentConnections[segment][1]];
    float along = (step + 0.5f) / LEDsPerSegment;

    x = (top[0] + (bottom[0] - top[0]) * along) * HexColumnWidth;
    y = (top[1] + (bottom[1] - top[1]) * along) * HexRowHeight;
}
