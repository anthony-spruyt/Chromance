#include "timeService.h"

using namespace Chromance;

TimeService::TimeService() :
    isInitialized(false)
{
}

void TimeService::Setup()
{
    waitForSync(NTPSyncTimeout);

    this->localTimezone.setLocation(TimeZoneLocation);

    this->isInitialized = true;
}

String TimeService::Now()
{
    return this->Now(ISO8601);
}

String TimeService::Now(String format)
{
    return this->isInitialized ?
        this->localTimezone.dateTime(format) :
        String(millis());
}

String TimeService::NowUTC()
{
    return this->NowUTC(ISO8601);
}

String TimeService::NowUTC(String format)
{
    return this->isInitialized ?
        UTC.dateTime(format) :
        String(millis());
}

Timezone TimeService::GetLocalTimezone()
{
    return this->localTimezone;
}

Timezone TimeService::GetUTCTimezone()
{
    return UTC;
}