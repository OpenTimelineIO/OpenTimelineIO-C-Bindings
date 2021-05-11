#include "copentime/timeTransform.h"
#include "copentime/util.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentime/timeTransform.h>

OTIO_API TimeTransform TimeTransform_create() {
    opentime::TimeTransform timeTransform;
    return _OTTimeTransform_to_COTTimeTransform(timeTransform);
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
    opentime::TimeTransform ot_self = _COTTimeTransform_to_OTTimeTransform(self);
    opentime::TimeRange ot_other = _COTTimeRange_to_OTTimeRange(other);
    opentime::TimeRange ot_result = ot_self.applied_to(ot_other);
    return _OTTimeRange_to_COTTimeRange(ot_result);
}

OTIO_API TimeTransform TimeTransform_applied_to_time_transform(
        TimeTransform self, TimeTransform other) {
    opentime::TimeTransform ot_self = _COTTimeTransform_to_OTTimeTransform(self);
    opentime::TimeTransform ot_other = _COTTimeTransform_to_OTTimeTransform(other);
    opentime::TimeTransform ot_result = ot_self.applied_to(ot_other);
    return _OTTimeTransform_to_COTTimeTransform(ot_result);
}

OTIO_API RationalTime TimeTransform_applied_to_rational_time(
        TimeTransform self, RationalTime other) {
    opentime::TimeTransform ot_self = _COTTimeTransform_to_OTTimeTransform(self);
    opentime::RationalTime ot_other = _COTRationalTime_to_OTRationalTime(other);
    opentime::RationalTime ot_result = ot_self.applied_to(ot_other);
    return _OTRationalTime_to_COTRationalTime(ot_result);
}

OTIO_API bool TimeTransform_equal(TimeTransform lhs, TimeTransform rhs) {
    opentime::TimeTransform ot_lhs = _COTTimeTransform_to_OTTimeTransform(lhs);
    opentime::TimeTransform ot_rhs = _COTTimeTransform_to_OTTimeTransform(rhs);
    return ot_lhs == ot_rhs;
}

OTIO_API bool TimeTransform_not_equal(TimeTransform lhs, TimeTransform rhs) {
    opentime::TimeTransform ot_lhs = _COTTimeTransform_to_OTTimeTransform(lhs);
    opentime::TimeTransform ot_rhs = _COTTimeTransform_to_OTTimeTransform(rhs);
    return ot_lhs != ot_rhs;
}
