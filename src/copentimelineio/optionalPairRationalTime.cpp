// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/optionalPairRationalTime.h"
#include <opentime/rationalTime.h>
#include <opentimelineio/optional.h>
#include <utility>

typedef std::pair<
        nonstd::optional<opentime::RationalTime>,
        nonstd::optional<opentime::RationalTime>>
        PairDef;

OTIO_API OptionalPairRationalTime *
OptionalPairRationalTime_create(OptionalRationalTime first, OptionalRationalTime second) {
    nonstd::optional<opentime::RationalTime> firstRationalTimeOptional =
            nonstd::nullopt;
    nonstd::optional<opentime::RationalTime> secondRationalTimeOptional =
            nonstd::nullopt;
    if (first.valid) {
        opentime::RationalTime rationalTime = CRationalTime_to_CppRationalTime(first.value);
        firstRationalTimeOptional =
                nonstd::optional<opentime::RationalTime>(rationalTime);
    }
    if (second.valid) {
        opentime::RationalTime rationalTime = CRationalTime_to_CppRationalTime(second.value);
        secondRationalTimeOptional =
                nonstd::optional<opentime::RationalTime>(rationalTime);
    }
    return reinterpret_cast<OptionalPairRationalTime *>(
            new std::pair<
                    nonstd::optional<opentime::RationalTime>,
                    nonstd::optional<opentime::RationalTime>>(
                    firstRationalTimeOptional, secondRationalTimeOptional));
}

OTIO_API OptionalRationalTime OptionalPairRationalTime_first(OptionalPairRationalTime *self) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->first;
    if (rationalTimeOptional == nonstd::nullopt) return OptionalRationalTime_create_null();
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    return OptionalRationalTime_create(CppRationalTime_to_CRationalTime(rtValue));
}

OTIO_API OptionalRationalTime
OptionalPairRationalTime_second(OptionalPairRationalTime *self) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->second;
    if (rationalTimeOptional == nonstd::nullopt) return OptionalRationalTime_create_null();
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    return OptionalRationalTime_create(CppRationalTime_to_CRationalTime(rtValue));
}

OTIO_API void OptionalPairRationalTime_destroy(OptionalPairRationalTime *self) {
    delete reinterpret_cast<PairDef *>(self);
}
