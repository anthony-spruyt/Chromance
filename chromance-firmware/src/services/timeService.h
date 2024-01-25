#ifndef TIME_SERVICE_H_
#define TIME_SERVICE_H_

#include "../globals.h"
#include <ezTime.h>

namespace Chromance
{
    class TimeService
    {
        public:

            TimeService();

            void Setup();
            String Now();
            String Now(String format);
            String NowUTC();
            String NowUTC(String format);
            Timezone GetLocalTimezone();
            Timezone GetUTCTimezone();

        private:

            Timezone localTimezone;
            bool isInitialized;
    };
}

#endif