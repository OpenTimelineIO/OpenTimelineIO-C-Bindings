#pragma once

#include "rationalTime.h"
#include "timeRange.h"
#include "timeTransform.h"

struct OptionalRationalTime {
    RationalTime value;
    bool valid;
};
typedef struct OptionalRationalTime OptionalRationalTime;

inline OptionalRationalTime OptionalRationalTime_create(RationalTime rationalTime) {
    OptionalRationalTime optionalRationalTime;
    optionalRationalTime.value = rationalTime;
    optionalRationalTime.valid = true;
}

inline OptionalRationalTime OptionalRationalTime_create_null() {
    OptionalRationalTime optionalRationalTime;
    optionalRationalTime.valid = false;
}

inline bool OptionalRationalTime_valid(OptionalRationalTime optionalRationalTime) {
    return optionalRationalTime.valid;
}

inline RationalTime OptionalRationalTime_value(OptionalRationalTime optionalRationalTime) {
    return optionalRationalTime.value;
}

struct OptionalTimeRange {
    TimeRange value;
    bool valid;
};
typedef struct OptionalTimeRange OptionalTimeRange;

inline OptionalTimeRange OptionalTimeRange_create(TimeRange timeRange) {
    OptionalTimeRange optionalTimeRange;
    optionalTimeRange.value = timeRange;
    optionalTimeRange.valid = true;
}

inline OptionalTimeRange OptionalTimeRange_create_null() {
    OptionalTimeRange optionalTimeRange;
    optionalTimeRange.valid = false;
}

inline bool OptionalTimeRange_valid(OptionalTimeRange optionalTimeRange) {
    return optionalTimeRange.valid;
}

inline TimeRange OptionalTimeRange_value(OptionalTimeRange optionalTimeRange) {
    return optionalTimeRange.value;
}