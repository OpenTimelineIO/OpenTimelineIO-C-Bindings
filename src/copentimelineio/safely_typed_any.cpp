#include "copentime/util.h"
#include "copentimelineio/safely_typed_any.h"
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentime/timeTransform.h>
#include <opentimelineio/any.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/safely_typed_any.h>
#include <opentimelineio/serializableObject.h>
#include <opentimelineio/version.h>
#include <string.h>
#include <utility>
#include <vector>

OTIO_API Any *create_safely_typed_any_bool(bool boolValue) {
    OTIO_NS::any anyValue =
            OTIO_NS::create_safely_typed_any(std::move(boolValue));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_int(int intValue) {
    OTIO_NS::any anyValue =
            OTIO_NS::create_safely_typed_any(std::move(intValue));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_int64(int64_t int64Value) {
    OTIO_NS::any anyValue =
            OTIO_NS::create_safely_typed_any(std::move(int64Value));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_double(double doubleValue) {
    OTIO_NS::any anyValue =
            OTIO_NS::create_safely_typed_any(std::move(doubleValue));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_string(const char *stringValue) {
    std::string str = stringValue;
    OTIO_NS::any anyValue =
            OTIO_NS::create_safely_typed_any(std::move(str));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_rational_time(RationalTime rationalTimeValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(std::move(
            _COTRationalTime_to_OTRationalTime(rationalTimeValue)));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_time_range(TimeRange timeRangeValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(
            _COTTimeRange_to_OTTimeRange(timeRangeValue));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *
create_safely_typed_any_time_transform(TimeTransform timeTransformValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(std::move(
            _COTTimeTransform_to_OTTimeTransform(timeTransformValue)));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_any_vector(AnyVector *anyVectorValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(
            std::move(*reinterpret_cast<OTIO_NS::AnyVector *>(anyVectorValue)));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *
create_safely_typed_any_any_dictionary(AnyDictionary *anyDictionaryValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(std::move(
            *reinterpret_cast<OTIO_NS::AnyDictionary *>(anyDictionaryValue)));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}
OTIO_API Any *create_safely_typed_any_serializable_object(
        OTIOSerializableObject *serializableObjectValue) {
    OTIO_NS::any anyValue = OTIO_NS::create_safely_typed_any(
            reinterpret_cast<OTIO_NS::SerializableObject *>(
                    serializableObjectValue));
    return reinterpret_cast<Any *>(new OTIO_NS::any(anyValue));
}

OTIO_API bool safely_cast_bool_any(Any *a) {
    return OTIO_NS::safely_cast_bool_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
}
OTIO_API int safely_cast_int_any(Any *a) {
    return OTIO_NS::safely_cast_int_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
}
OTIO_API int64_t safely_cast_int64_any(Any *a) {
    return OTIO_NS::safely_cast_int64_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
}
OTIO_API double safely_cast_double_any(Any *a) {
    return OTIO_NS::safely_cast_double_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
}
OTIO_API const char *safely_cast_string_any(Any *a) {
    std::string returnStr = OTIO_NS::safely_cast_string_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}
OTIO_API RationalTime safely_cast_rational_time_any(Any *a) {
    opentime::RationalTime rationalTime =
            OTIO_NS::safely_cast_rational_time_any(
                    *reinterpret_cast<OTIO_NS::any *>(a));
    return _OTRationalTime_to_COTRationalTime(rationalTime);
}
OTIO_API TimeRange safely_cast_time_range_any(Any *a) {
    opentime::TimeRange timeRange = OTIO_NS::safely_cast_time_range_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
    return _OTTimeRange_to_COTTimeRange(timeRange);
}
OTIO_API TimeTransform safely_cast_time_transform_any(Any *a) {
    opentime::TimeTransform timeTransform =
            OTIO_NS::safely_cast_time_transform_any(
                    *reinterpret_cast<OTIO_NS::any *>(a));
    return _OTTimeTransform_to_COTTimeTransform(timeTransform);
}
OTIO_API OTIOSerializableObject *safely_cast_retainer_any(Any *a) {
    return reinterpret_cast<OTIOSerializableObject *>(
            OTIO_NS::safely_cast_retainer_any(
                    *reinterpret_cast<OTIO_NS::any *>(a)));
}

OTIO_API AnyDictionary *safely_cast_any_dictionary_any(Any *a) {
    OTIO_NS::AnyDictionary anyDictionary =
            OTIO_NS::safely_cast_any_dictionary_any(
                    *reinterpret_cast<OTIO_NS::any *>(a));
    return reinterpret_cast<AnyDictionary *>(
            new OTIO_NS::AnyDictionary(anyDictionary));
}

OTIO_API AnyVector *safely_cast_any_vector_any(Any *a) {
    OTIO_NS::AnyVector anyVector = OTIO_NS::safely_cast_any_vector_any(
            *reinterpret_cast<OTIO_NS::any *>(a));
    return reinterpret_cast<AnyVector *>(new OTIO_NS::AnyVector(anyVector));
}

