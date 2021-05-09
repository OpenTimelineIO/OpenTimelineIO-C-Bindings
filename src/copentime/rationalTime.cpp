#include "copentime/rationalTime.h"
#include <opentime/rationalTime.h>
#include <string.h>

RationalTime RationalTime_create(double value, double rate) {
    RationalTime rationalTime;
    rationalTime.value = value;
    rationalTime.rate = rate;
    return rationalTime;
}

bool RationalTime_is_invalid_time(RationalTime self) {
    opentime::RationalTime ot_self(self.value, self.rate);
    return ot_self.is_invalid_time();
}

double RationalTime_value(RationalTime self) {
    return self.value;
}

double RationalTime_rate(RationalTime self) {
    return self.rate;
}

RationalTime RationalTime_rescaled_to(RationalTime self, double new_rate) {
    opentime::RationalTime ot_self(self.value, self.rate);
    opentime::RationalTime ot_rescaledRationalTime = ot_self.rescaled_to(new_rate);
    return RationalTime_create(ot_rescaledRationalTime.value(), ot_rescaledRationalTime.rate());
}

RationalTime
RationalTime_rescaled_to_rational_time(RationalTime self, RationalTime rt) {
    opentime::RationalTime ot_self(self.value, self.rate);
    opentime::RationalTime ot_rescaleTime(rt.value, rt.rate);
    opentime::RationalTime ot_rescaledRationalTime = ot_self.rescaled_to(ot_rescaleTime);
    return RationalTime_create(ot_rescaledRationalTime.value(), ot_rescaledRationalTime.rate());
}

double
RationalTime_value_rescaled_to_rate(RationalTime self, double new_rate) {
    opentime::RationalTime ot_self(self.value, self.rate);
    return ot_self.value_rescaled_to(new_rate);
}

double RationalTime_value_rescaled_to_rational_time(
        RationalTime self, RationalTime rt) {
    opentime::RationalTime ot_self(self.value, self.rate);
    opentime::RationalTime ot_rescaleTime(rt.value, rt.rate);
    return ot_self.value_rescaled_to(ot_rescaleTime);
}

bool RationalTime_almost_equal(
        RationalTime self, RationalTime other, double delta) {
    opentime::RationalTime ot_self(self.value, self.rate);
    opentime::RationalTime ot_otherTime(other.value, other.rate);
    return ot_self.almost_equal(ot_otherTime, delta);
}

RationalTime RationalTime_duration_from_start_end_time(
        RationalTime start_time, RationalTime end_time_exclusive) {
    opentime::RationalTime ot_startTime(start_time.value, start_time.rate);
    opentime::RationalTime ot_endTimeExclusive(end_time_exclusive.value, end_time_exclusive.rate);
    opentime::RationalTime obj =
            opentime::RationalTime::duration_from_start_end_time(ot_startTime, ot_endTimeExclusive);
    return RationalTime_create(obj.value(), obj.rate());
}

bool RationalTime_is_valid_timecode_rate(double rate) {
    return opentime::RationalTime::is_valid_timecode_rate(rate);
}

RationalTime RationalTime_from_frames(double frame, double rate) {
    opentime::RationalTime ot_rationalTime =
            opentime::RationalTime::from_frames(frame, rate);
    return RationalTime_create(ot_rationalTime.value(), ot_rationalTime.rate());
}

RationalTime RationalTime_from_seconds(double seconds) {
    opentime::RationalTime ot_rationalTime =
            opentime::RationalTime::from_seconds(seconds);
    return RationalTime_create(ot_rationalTime.value(), ot_rationalTime.rate());
}

RationalTime RationalTime_from_timecode(
        const char *timecode, double rate, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_rationalTime = opentime::RationalTime::from_timecode(
            timecode,
            rate,
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return RationalTime_create(ot_rationalTime.value(), ot_rationalTime.rate());
}

RationalTime RationalTime_from_time_string(
        const char *time_string, double rate, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_rationalTime = opentime::RationalTime::from_time_string(
            time_string,
            rate,
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    return RationalTime_create(ot_rationalTime.value(), ot_rationalTime.rate());
}

int RationalTime_to_frames(RationalTime self) {
    opentime::RationalTime ot_self(self.value, self.rate);
    return ot_self.to_frames();
}

int RationalTime_to_frames_with_rate(RationalTime self, double rate) {
    opentime::RationalTime ot_self(self.value, self.rate);
    return ot_self.to_frames(rate);
}

double RationalTime_to_seconds(RationalTime self) {
    opentime::RationalTime ot_self(self.value, self.rate);
    return ot_self.to_seconds();
}

const char *RationalTime_to_timecode(
        RationalTime self,
        double rate,
        OpenTime_IsDropFrameRate drop_frame,
        OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_self(self.value, self.rate);
    std::string returnStr =
            ot_self.to_timecode(
                    rate,
                    static_cast<opentime::IsDropFrameRate>(drop_frame),
                    reinterpret_cast<opentime::ErrorStatus *>(error_status));
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}

const char *RationalTime_to_timecode_auto(
        RationalTime self, OpenTimeErrorStatus *error_status) {
    opentime::RationalTime ot_self(self.value, self.rate);
    std::string returnStr = ot_self.to_timecode(
            reinterpret_cast<opentime::ErrorStatus *>(error_status));
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}

const char *RationalTime_to_time_string(RationalTime self) {
    opentime::RationalTime ot_self(self.value, self.rate);
    std::string returnStr = ot_self.to_time_string();
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}

RationalTime RationalTime_add(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs(lhs.value, lhs.rate);
    opentime::RationalTime ot_rhs(rhs.value, rhs.rate);
    opentime::RationalTime ot_result = ot_lhs + ot_rhs;
    return RationalTime_create(ot_result.value(), ot_result.rate());
}

RationalTime RationalTime_subtract(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs(lhs.value, lhs.rate);
    opentime::RationalTime ot_rhs(rhs.value, rhs.rate);
    opentime::RationalTime ot_result = ot_lhs - ot_rhs;
    return RationalTime_create(ot_result.value(), ot_result.rate());
}

RationalTime RationalTime_compare(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs(lhs.value, lhs.rate);
    opentime::RationalTime ot_rhs(rhs.value, rhs.rate);
    opentime::RationalTime ot_result = ot_rhs - ot_lhs;
    return RationalTime_create(ot_result.value(), ot_result.rate());
}

bool RationalTime_equal(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs(lhs.value, lhs.rate);
    opentime::RationalTime ot_rhs(rhs.value, rhs.rate);
    return ot_rhs == ot_lhs;
}

bool RationalTime_not_equal(RationalTime lhs, RationalTime rhs) {
    opentime::RationalTime ot_lhs(lhs.value, lhs.rate);
    opentime::RationalTime ot_rhs(rhs.value, rhs.rate);
    return ot_rhs != ot_lhs;
}
