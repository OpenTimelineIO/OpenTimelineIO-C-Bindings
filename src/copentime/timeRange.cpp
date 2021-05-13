#include "copentime/timeRange.h"
#include "copentime/util.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>

OTIO_API TimeRange TimeRange_create() {
    opentime::TimeRange ot_timeRange;
    return CppTimeRange_to_CTimeRange(ot_timeRange);
}

OTIO_API TimeRange TimeRange_create_with_start_time(RationalTime start_time) {
    opentime::RationalTime ot_startTime(start_time.value, start_time.rate);
    opentime::TimeRange ot_timeRange(ot_startTime);
    return CppTimeRange_to_CTimeRange(ot_timeRange);
}

OTIO_API TimeRange TimeRange_create_with_duration(RationalTime duration) {
    opentime::RationalTime ot_duration(duration.value, duration.rate);
    opentime::TimeRange ot_timeRange(opentime::RationalTime(), ot_duration);
    return CppTimeRange_to_CTimeRange(ot_timeRange);
}

OTIO_API TimeRange TimeRange_create_with_start_time_and_duration(
        RationalTime start_time, RationalTime duration) {
    TimeRange timeRange;
    timeRange.start_time = start_time;
    timeRange.duration = duration;
    return timeRange;
}

OTIO_API RationalTime TimeRange_start_time(TimeRange self) {
    return self.start_time;
}

OTIO_API RationalTime TimeRange_duration(TimeRange self) {
    return self.duration;
}

OTIO_API RationalTime TimeRange_end_time_inclusive(TimeRange self) {
    opentime::RationalTime ot_startTime = CRationalTime_to_CppRationalTime(self.start_time);
    opentime::RationalTime ot_duration = CRationalTime_to_CppRationalTime(self.duration);
    opentime::TimeRange ot_self(ot_startTime, ot_duration);
    opentime::RationalTime ot_result = ot_self.end_time_inclusive();
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API RationalTime TimeRange_end_time_exclusive(TimeRange self) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::RationalTime ot_result = ot_self.end_time_exclusive();
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API TimeRange
TimeRange_duration_extended_by(TimeRange self, RationalTime other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::RationalTime ot_other = CRationalTime_to_CppRationalTime(other);
    opentime::TimeRange ot_result = ot_self.duration_extended_by(ot_other);
    return CppTimeRange_to_CTimeRange(ot_result);
}

OTIO_API TimeRange TimeRange_extended_by(TimeRange self, TimeRange other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::TimeRange ot_other = CTimeRange_to_CppTimeRange(other);
    opentime::TimeRange ot_result = ot_self.extended_by(ot_other);
    return CppTimeRange_to_CTimeRange(ot_result);
}

OTIO_API RationalTime
TimeRange_clamped_with_rational_time(TimeRange self, RationalTime other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::RationalTime ot_other = CRationalTime_to_CppRationalTime(other);
    opentime::RationalTime ot_result = ot_self.clamped(ot_other);
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API TimeRange
TimeRange_clamped_with_time_range(TimeRange self, TimeRange other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::TimeRange ot_other = CTimeRange_to_CppTimeRange(other);
    opentime::TimeRange ot_result = ot_self.clamped(ot_other);
    return CppTimeRange_to_CTimeRange(ot_result);
}

OTIO_API bool TimeRange_contains_rational_time(TimeRange self, RationalTime other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::RationalTime ot_other = CRationalTime_to_CppRationalTime(other);
    return ot_self.contains(ot_other);
}

OTIO_API bool TimeRange_contains_time_range(TimeRange self, TimeRange other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::TimeRange ot_other = CTimeRange_to_CppTimeRange(other);
    return ot_self.contains(ot_other);
}

OTIO_API bool TimeRange_overlaps_rational_time(TimeRange self, RationalTime other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::RationalTime ot_other = CRationalTime_to_CppRationalTime(other);
    return ot_self.overlaps(ot_other);
}

OTIO_API bool TimeRange_overlaps_time_range(TimeRange self, TimeRange other) {
    opentime::TimeRange ot_self = CTimeRange_to_CppTimeRange(self);
    opentime::TimeRange ot_other = CTimeRange_to_CppTimeRange(other);
    return ot_self.overlaps(ot_other);
}

OTIO_API bool TimeRange_equal(TimeRange lhs, TimeRange rhs) {
    opentime::TimeRange ot_lhs = CTimeRange_to_CppTimeRange(lhs);
    opentime::TimeRange ot_rhs = CTimeRange_to_CppTimeRange(rhs);
    return ot_lhs == ot_rhs;
}

OTIO_API bool TimeRange_not_equal(TimeRange lhs, TimeRange rhs) {
    opentime::TimeRange ot_lhs = CTimeRange_to_CppTimeRange(lhs);
    opentime::TimeRange ot_rhs = CTimeRange_to_CppTimeRange(rhs);
    return ot_lhs != ot_rhs;
}

OTIO_API TimeRange TimeRange_range_from_start_end_time(
        RationalTime start_time, RationalTime end_time_exclusive) {
    opentime::RationalTime ot_startTime = CRationalTime_to_CppRationalTime(start_time);
    opentime::RationalTime ot_endTimeExclusive = CRationalTime_to_CppRationalTime(end_time_exclusive);
    opentime::TimeRange ot_result = opentime::TimeRange::range_from_start_end_time(
            ot_startTime, ot_endTimeExclusive);
    return CppTimeRange_to_CTimeRange(ot_result);
}
