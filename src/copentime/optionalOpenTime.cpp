#include "copentime/optionalOpenTime.h"

OTIO_API OptionalRationalTime OptionalRationalTime_create(RationalTime rationalTime) {
    OptionalRationalTime optionalRationalTime;
    optionalRationalTime.value = rationalTime;
    optionalRationalTime.valid = true;
    return optionalRationalTime;
}

OTIO_API OptionalRationalTime OptionalRationalTime_create_null() {
    OptionalRationalTime optionalRationalTime;
    optionalRationalTime.valid = false;
    return optionalRationalTime;
}

OTIO_API bool OptionalRationalTime_valid(OptionalRationalTime optionalRationalTime) {
    return optionalRationalTime.valid;
}

OTIO_API RationalTime OptionalRationalTime_value(OptionalRationalTime optionalRationalTime) {
    return optionalRationalTime.value;
}

OTIO_API OptionalTimeRange OptionalTimeRange_create(TimeRange timeRange) {
    OptionalTimeRange optionalTimeRange;
    optionalTimeRange.value = timeRange;
    optionalTimeRange.valid = true;
    return optionalTimeRange;
}

OTIO_API OptionalTimeRange OptionalTimeRange_create_null() {
    OptionalTimeRange optionalTimeRange;
    optionalTimeRange.valid = false;
    return optionalTimeRange;
}

OTIO_API bool OptionalTimeRange_valid(OptionalTimeRange optionalTimeRange) {
    return optionalTimeRange.valid;
}

OTIO_API TimeRange OptionalTimeRange_value(OptionalTimeRange optionalTimeRange) {
    return optionalTimeRange.value;
}