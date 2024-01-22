#ifndef MODELS_H_
#define MODELS_H_

#include <Arduino.h>

namespace Chromance
{
    enum AnimationStatus
    {
        ANIMATION_STATUS_PLAYING,
        ANIMATION_STATUS_SLEEPING,
        ANIMATION_STATUS_WAKING_UP,
        ANIMATION_STATUS_GOING_TO_SLEEP
    };

    enum AnimationType
    {
        ANIMATION_TYPE_RANDOM_ANIMATION, // DO NOT MOVE- We have to use auto assignment and this needs to be 0
        ANIMATION_TYPE_STRIP_TEST, // DO NOT MOVE- We have to use auto assignment and this needs to be 1
        ANIMATION_TYPE_RANDOM_PULSE,
        ANIMATION_TYPE_CUBE_PULSE,
        ANIMATION_TYPE_STAR_BURST_PULSE,
        ANIMATION_TYPE_CENTER_PULSE,
        ANIMATION_TYPE_RAINBOW_BEAT,
        ANIMATION_TYPE_RAINBOW_MARCH,
        ANIMATION_TYPE_PULSE,
        ANIMATION_TYPE_AROUND_THE_WORLD,
        /**
         * This is not an animation type. It is a hack to get the count of animation types
         * DO NOT MOVE- We have to use auto assignment and this needs to be the last
        */
        ANIMATION_TYPE_NUMBER_OF_ANIMATIONS
    };

    enum AnimationRequest
    {
        ANIMATION_REQUEST_NONE,
        ANIMATION_REQUEST_PLAY,
        ANIMATION_REQUEST_SLEEP,
        ANIMATION_REQUEST_WAKE
    };

    enum LogLevel
    {
        LOG_LEVEL_TRACE = 0,
        LOG_LEVEL_DEBUG = 1,
        LOG_LEVEL_INFORMATION = 2,
        LOG_LEVEL_WARNING = 3,
        LOG_LEVEL_ERROR = 4,
        LOG_LEVEL_CRITICAL = 5,
        LOG_LEVEL_NONE = 6
    };

    struct ChromanceState
    {
        AnimationType animationType;
        AnimationStatus animationStatus;
        uint8_t brightness;
        uint32_t fps;
        const char* effect;
    };

    struct PublishRequest
    {
        unsigned long publishedAt;
        ChromanceState state;
    };
}

#endif