#pragma once

#include "rationalTime.h"
#include "timeRange.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>

inline RationalTime _OTRationalTime_to_COTRationalTime(opentime::RationalTime rationalTime) {
    return RationalTime_create(rationalTime.value(), rationalTime.rate());
}

inline opentime::RationalTime _COTRationalTime_to_OTRationalTime(RationalTime rationalTime) {
    return opentime::RationalTime(rationalTime.value, rationalTime.rate);
}

inline TimeRange _OTTimeRange_to_COTTimeRange(opentime::TimeRange timeRange) {
    RationalTime startTime = _OTRationalTime_to_COTRationalTime(timeRange.start_time());
    RationalTime duration = _OTRationalTime_to_COTRationalTime(timeRange.duration());
    TimeRange cot_timeRange;
    cot_timeRange.start_time = startTime;
    cot_timeRange.duration = duration;
    return cot_timeRange;
}

inline opentime::TimeRange _COTTimeRange_to_OTTimeRange(TimeRange timeRange) {
    opentime::RationalTime startTime = _COTRationalTime_to_OTRationalTime(timeRange.start_time);
    opentime::RationalTime duration = _COTRationalTime_to_OTRationalTime(timeRange.duration);
    return opentime::TimeRange(startTime, duration);
}