#pragma once

#include "rationalTime.h"
#include "timeRange.h"
#include "timeTransform.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentime/timeTransform.h>

inline RationalTime _OTRationalTime_to_COTRationalTime(opentime::RationalTime rationalTime) {
    return RationalTime_create(rationalTime.value(), rationalTime.rate());
}

inline opentime::RationalTime _COTRationalTime_to_OTRationalTime(RationalTime rationalTime) {
    return opentime::RationalTime(rationalTime.value, rationalTime.rate);
}

inline TimeRange _OTTimeRange_to_COTTimeRange(opentime::TimeRange timeRange) {
    RationalTime startTime = _OTRationalTime_to_COTRationalTime(timeRange.start_time());
    RationalTime duration = _OTRationalTime_to_COTRationalTime(timeRange.duration());
    return TimeRange_create_with_start_time_and_duration(startTime, duration);
}

inline opentime::TimeRange _COTTimeRange_to_OTTimeRange(TimeRange timeRange) {
    opentime::RationalTime startTime = _COTRationalTime_to_OTRationalTime(timeRange.start_time);
    opentime::RationalTime duration = _COTRationalTime_to_OTRationalTime(timeRange.duration);
    return opentime::TimeRange(startTime, duration);
}

inline TimeTransform _OTTimeTransform_to_COTTimeTransform(opentime::TimeTransform timeTransform) {
    RationalTime offset = _OTRationalTime_to_COTRationalTime(timeTransform.offset());
    return TimeTransform_create_with_offset_scale_rate(offset, timeTransform.scale(), timeTransform.rate());
}

inline opentime::TimeTransform _COTTimeTransform_to_OTTimeTransform(TimeTransform timeTransform) {
    opentime::RationalTime offset = _COTRationalTime_to_OTRationalTime(timeTransform.offset);
    return opentime::TimeTransform(offset, timeTransform.scale, timeTransform.rate);
}