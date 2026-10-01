#ifndef LED_MAP_H_
#define LED_MAP_H_

#include "../globals.h"

namespace Chromance
{
    // Where every LED sits on the wall, worked out once from the hex map so animations can draw in 2D
    class LEDMap
    {
        public:

            LEDMap();

            // 0 is the left-most LED and 255 the right-most
            uint8_t GetX(uint32_t led);
            // 0 is the top-most LED and 255 the bottom-most
            uint8_t GetY(uint32_t led);
            // From the center node, 255 is the farthest LED. Unlike x and y it isn't stretched, so equal distances form circles
            uint8_t GetDistance(uint32_t led);
            // Around the center node, 0 is 12:00 and it increases clockwise, so 64 is 3:00
            uint8_t GetAngle(uint32_t led);

        private:

            void GetPosition(int32_t segment, uint32_t step, float& x, float& y);

            uint8_t x[NumberOfLEDs];
            uint8_t y[NumberOfLEDs];
            uint8_t distance[NumberOfLEDs];
            uint8_t angle[NumberOfLEDs];
    };
}

#endif
