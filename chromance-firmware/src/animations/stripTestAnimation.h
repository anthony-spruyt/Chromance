#ifndef STRIPTEST_ANIMATION_H_
#define STRIPTEST_ANIMATION_H_

#include "animation.h"

namespace Chromance
{
    class StripTestAnimation : public Animation
    {
        public:

            StripTestAnimation(int32_t id, Config* config, Logger* logger);

            void Loop();

        private:

            static const unsigned long PixelTestMillis = 500UL;
            static const unsigned long FillTestMillis = 5000UL;

            unsigned long lastFrame;
            uint32_t counter;
            bool fillTest1;
            bool fillTest2;
            bool fillTest3;
            bool shouldWaitForFillTestNextFrame;
            bool shouldWaitForPixelTestNextFrame;
            uint32_t nextPixelColorTest;
    };
}

#endif