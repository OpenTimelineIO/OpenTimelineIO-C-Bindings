// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/optionalPairRationalTime.h"
#include <opentime/rationalTime.h>
//#include <opentimelineio/optional.h>
#include <utility>
#include <optional>

typedef std::pair<
        std::optional<opentime::RationalTime>,
        std::optional<opentime::RationalTime>>
        PairDef;

OTIO_API OptionalPairRationalTime *
OptionalPairRationalTime_create(OptionalRationalTime first, OptionalRationalTime second) {
    std::optional<opentime::RationalTime> firstRationalTimeOptional =
            std::nullopt;
    std::optional<opentime::RationalTime> secondRationalTimeOptional =
            std::nullopt;
    if (first.valid) {
        opentime::RationalTime rationalTime = CRationalTime_to_CppRationalTime(first.value);
        firstRationalTimeOptional =
                std::optional<opentime::RationalTime>(rationalTime);
    }
    if (second.valid) {
        opentime::RationalTime rationalTime = CRationalTime_to_CppRationalTime(second.value);
        secondRationalTimeOptional =
                std::optional<opentime::RationalTime>(rationalTime);
    }
    return reinterpret_cast<OptionalPairRationalTime *>(
            new std::pair<
                    std::optional<opentime::RationalTime>,
                    std::optional<opentime::RationalTime>>(
                    firstRationalTimeOptional, secondRationalTimeOptional));
}

OTIO_API OptionalRationalTime OptionalPairRationalTime_first(OptionalPairRationalTime *self) {
    std::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->first;
    if (rationalTimeOptional == std::nullopt) return OptionalRationalTime_create_null();
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    return OptionalRationalTime_create(CppRationalTime_to_CRationalTime(rtValue));
}

OTIO_API OptionalRationalTime
OptionalPairRationalTime_second(OptionalPairRationalTime *self) {
    std::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->second;
    if (rationalTimeOptional == std::nullopt) return OptionalRationalTime_create_null();
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    return OptionalRationalTime_create(CppRationalTime_to_CRationalTime(rtValue));
}

OTIO_API void OptionalPairRationalTime_destroy(OptionalPairRationalTime *self) {
    delete reinterpret_cast<PairDef *>(self);
}
