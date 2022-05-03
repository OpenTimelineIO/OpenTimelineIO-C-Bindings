// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/timeTransform.h"
#include "copentime/util.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentime/timeTransform.h>

OTIO_API TimeTransform TimeTransform_create() {
    opentime::TimeTransform timeTransform;
    return CppTimeTransform_to_CTimeTransform(timeTransform);
}

OTIO_API TimeTransform TimeTransform_create_with_offset_scale_rate(
        RationalTime offset, double scale, double rate) {
    TimeTransform timeTransform;
    timeTransform.offset = offset;
    timeTransform.scale = scale;
    timeTransform.rate = rate;
    return timeTransform;
}

OTIO_API RationalTime TimeTransform_offset(TimeTransform self) {
    return self.offset;
}

OTIO_API double TimeTransform_scale(TimeTransform self) {
    return self.scale;
}

OTIO_API double TimeTransform_rate(TimeTransform self) {
    return self.rate;
}

OTIO_API TimeRange
TimeTransform_applied_to_time_range(TimeTransform self, TimeRange other) {
    opentime::TimeTransform ot_self = CTimeTransform_to_CppTimeTransform(self);
    opentime::TimeRange ot_other = CTimeRange_to_CppTimeRange(other);
    opentime::TimeRange ot_result = ot_self.applied_to(ot_other);
    return CppTimeRange_to_CTimeRange(ot_result);
}

OTIO_API TimeTransform TimeTransform_applied_to_time_transform(
        TimeTransform self, TimeTransform other) {
    opentime::TimeTransform ot_self = CTimeTransform_to_CppTimeTransform(self);
    opentime::TimeTransform ot_other = CTimeTransform_to_CppTimeTransform(other);
    opentime::TimeTransform ot_result = ot_self.applied_to(ot_other);
    return CppTimeTransform_to_CTimeTransform(ot_result);
}

OTIO_API RationalTime TimeTransform_applied_to_rational_time(
        TimeTransform self, RationalTime other) {
    opentime::TimeTransform ot_self = CTimeTransform_to_CppTimeTransform(self);
    opentime::RationalTime ot_other = CRationalTime_to_CppRationalTime(other);
    opentime::RationalTime ot_result = ot_self.applied_to(ot_other);
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API bool TimeTransform_equal(TimeTransform lhs, TimeTransform rhs) {
    opentime::TimeTransform ot_lhs = CTimeTransform_to_CppTimeTransform(lhs);
    opentime::TimeTransform ot_rhs = CTimeTransform_to_CppTimeTransform(rhs);
    return ot_lhs == ot_rhs;
}

OTIO_API bool TimeTransform_not_equal(TimeTransform lhs, TimeTransform rhs) {
    opentime::TimeTransform ot_lhs = CTimeTransform_to_CppTimeTransform(lhs);
    opentime::TimeTransform ot_rhs = CTimeTransform_to_CppTimeTransform(rhs);
    return ot_lhs != ot_rhs;
}
