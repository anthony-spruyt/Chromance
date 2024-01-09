#ifndef STRIPTEST_ANIMATION_H_
#define STRIPTEST_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class StripTestAnimation : public Animation
    {
        public:

            StripTestAnimation(uint8_t id);

            void Loop();

        private:

            static const unsigned long PixelTestMillis = 500;
            static const unsigned long FillTestMillis = 5000;

            unsigned long lastFrame;
            uint16_t counter;
            bool fillTest1;
            bool fillTest2;
            bool fillTest3;
            bool shouldWaitForFillTestNextFrame;
            bool shouldWaitForPixelTestNextFrame;
            uint8_t nextPixelColorTest;
    };
}

#endif