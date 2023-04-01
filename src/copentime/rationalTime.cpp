// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/rationalTime.h"
#include "copentime/util.h"
#include <opentime/rationalTime.h>

OTIO_API RationalTime RationalTime_create(double value, double rate) {
    RationalTime rationalTime;
    rationalTime.value = value;
    rationalTime.rate = rate;
    return rationalTime;
}

OTIO_API bool RationalTime_is_invalid_time(RationalTime self) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    return ot_self.is_invalid_time();
}

OTIO_API double RationalTime_value(RationalTime self) {
    return self.value;
}

OTIO_API double RationalTime_rate(RationalTime self) {
    return self.rate;
}

OTIO_API RationalTime RationalTime_rescaled_to(RationalTime self, double new_rate) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    opentime::RationalTime ot_rescaledRationalTime = ot_self.rescaled_to(new_rate);
    return CppRationalTime_to_CRationalTime(ot_rescaledRationalTime);
}

OTIO_API RationalTime
RationalTime_rescaled_to_rational_time(RationalTime self, RationalTime rt) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    opentime::RationalTime ot_rescaleTime = CRationalTime_to_CppRationalTime(rt);
    opentime::RationalTime ot_rescaledRationalTime = ot_self.rescaled_to(ot_rescaleTime);
    return CppRationalTime_to_CRationalTime(ot_rescaledRationalTime);
}

OTIO_API double
RationalTime_value_rescaled_to_rate(RationalTime self, double new_rate) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    return ot_self.value_rescaled_to(new_rate);
}

OTIO_API double RationalTime_value_rescaled_to_rational_time(
        RationalTime self, RationalTime rt) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    opentime::RationalTime ot_rescaleTime = CRationalTime_to_CppRationalTime(rt);
    return ot_self.value_rescaled_to(ot_rescaleTime);
}

OTIO_API bool RationalTime_almost_equal(
        RationalTime self, RationalTime other, double delta) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    opentime::RationalTime ot_otherTime = CRationalTime_to_CppRationalTime(other);
    return ot_self.almost_equal(ot_otherTime, delta);
}

OTIO_API RationalTime RationalTime_duration_from_start_end_time(
        RationalTime start_time, RationalTime end_time_exclusive) {
    opentime::RationalTime ot_startTime = CRationalTime_to_CppRationalTime(start_time);
    opentime::RationalTime ot_endTimeExclusive = CRationalTime_to_CppRationalTime(end_time_exclusive);
    opentime::RationalTime ot_result =
            opentime::RationalTime::duration_from_start_end_time(ot_startTime, ot_endTimeExclusive);
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API bool RationalTime_is_valid_timecode_rate(double rate) {
    return opentime::RationalTime::is_valid_timecode_rate(rate);
}

OTIO_API RationalTime RationalTime_from_frames(double frame, double rate) {
    opentime::RationalTime ot_rationalTime =
            opentime::RationalTime::from_frames(frame, rate);
    return CppRationalTime_to_CRationalTime(ot_rationalTime);
}

OTIO_API RationalTime RationalTime_from_seconds(double seconds) {
    opentime::RationalTime ot_rationalTime =
            opentime::RationalTime::from_seconds(seconds);
    return CppRationalTime_to_CRationalTime(ot_rationalTime);
}

OTIO_API RationalTime RationalTime_from_timecode(
        const char *timecode, double rate, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_rationalTime = opentime::RationalTime::from_timecode(
            timecode,
            rate,
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return CppRationalTime_to_CRationalTime(ot_rationalTime);
}

OTIO_API RationalTime RationalTime_from_time_string(
        const char *time_string, double rate, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_rationalTime = opentime::RationalTime::from_time_string(
            time_string,
            rate,
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return CppRationalTime_to_CRationalTime(ot_rationalTime);
}

OTIO_API int RationalTime_to_frames(RationalTime self) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    return ot_self.to_frames();
}

OTIO_API int RationalTime_to_frames_with_rate(RationalTime self, double rate) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    return ot_self.to_frames(rate);
}

OTIO_API double RationalTime_to_seconds(RationalTime self) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    return ot_self.to_seconds();
}

OTIO_API const char *RationalTime_to_timecode(
        RationalTime self,
        double rate,
        OpenTime_IsDropFrameRate drop_frame,
        OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    std::string returnStr =
            ot_self.to_timecode(
                    rate,
                    static_cast<opentime::IsDropFrameRate>(drop_frame),
                    reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return _strdup(returnStr.c_str());
}

OTIO_API const char *RationalTime_to_timecode_auto(
        RationalTime self, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    std::string returnStr = ot_self.to_timecode(
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return _strdup(returnStr.c_str());
}

OTIO_API const char *RationalTime_to_time_string(RationalTime self) {
    opentime::RationalTime ot_self = CRationalTime_to_CppRationalTime(self);
    std::string returnStr = ot_self.to_time_string();
    return _strdup(returnStr.c_str());
}

OTIO_API RationalTime RationalTime_add(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs = CRationalTime_to_CppRationalTime(lhs);
    opentime::RationalTime ot_rhs = CRationalTime_to_CppRationalTime(rhs);
    opentime::RationalTime ot_result = ot_lhs + ot_rhs;
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API RationalTime RationalTime_subtract(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs = CRationalTime_to_CppRationalTime(lhs);
    opentime::RationalTime ot_rhs = CRationalTime_to_CppRationalTime(rhs);
    opentime::RationalTime ot_result = ot_lhs - ot_rhs;
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API RationalTime RationalTime_compare(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs = CRationalTime_to_CppRationalTime(lhs);
    opentime::RationalTime ot_rhs = CRationalTime_to_CppRationalTime(rhs);
    opentime::RationalTime ot_result = ot_rhs - ot_lhs;
    return CppRationalTime_to_CRationalTime(ot_result);
}

OTIO_API bool RationalTime_equal(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs = CRationalTime_to_CppRationalTime(lhs);
    opentime::RationalTime ot_rhs = CRationalTime_to_CppRationalTime(rhs);
    return ot_rhs == ot_lhs;
}

OTIO_API bool RationalTime_not_equal(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs = CRationalTime_to_CppRationalTime(lhs);
    opentime::RationalTime ot_rhs = CRationalTime_to_CppRationalTime(rhs);
    return ot_rhs != ot_lhs;
}
