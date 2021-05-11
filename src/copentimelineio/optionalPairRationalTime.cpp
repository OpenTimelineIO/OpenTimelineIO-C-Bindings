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
OptionalPairRationalTime_create(RationalTime *first, RationalTime *second) {
    nonstd::optional<opentime::RationalTime> firstRationalTimeOptional =
            nonstd::nullopt;
    nonstd::optional<opentime::RationalTime> secondRationalTimeOptional =
            nonstd::nullopt;
    if (first != NULL) {
        opentime::RationalTime rationalTime = _COTRationalTime_to_OTRationalTime(*first);
        firstRationalTimeOptional =
                nonstd::optional<opentime::RationalTime>(rationalTime);
    }
    if (second != NULL) {
        opentime::RationalTime rationalTime = _COTRationalTime_to_OTRationalTime(*second);
        secondRationalTimeOptional =
                nonstd::optional<opentime::RationalTime>(rationalTime);
    }
    return reinterpret_cast<OptionalPairRationalTime *>(
            new std::pair<
                    nonstd::optional<opentime::RationalTime>,
                    nonstd::optional<opentime::RationalTime>>(
                    firstRationalTimeOptional, secondRationalTimeOptional));
}

OTIO_API bool OptionalPairRationalTime_first(OptionalPairRationalTime *self, RationalTime &first) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->first;
    if (rationalTimeOptional == nonstd::nullopt) return false;
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    first = _OTRationalTime_to_COTRationalTime(rtValue);
    return true;
}

OTIO_API bool
OptionalPairRationalTime_second(OptionalPairRationalTime *self, RationalTime &second) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<PairDef *>(self)->second;
    if (rationalTimeOptional == nonstd::nullopt) return false;
    opentime::RationalTime rtValue = rationalTimeOptional.value();
    second = _OTRationalTime_to_COTRationalTime(rtValue);
    return true;
}

OTIO_API void OptionalPairRationalTime_destroy(OptionalPairRationalTime *self) {
    delete reinterpret_cast<PairDef *>(self);
}