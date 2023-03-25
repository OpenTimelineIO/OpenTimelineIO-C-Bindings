// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/transition.h"
#include <copentimelineio/composition.h>
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/errorStatus.h>
#include <opentimelineio/transition.h>
#include <string.h>


const char *TransitionType_SMPTE_Dissolve =
        OTIO_NS::Transition::Type::SMPTE_Dissolve;
const char *TransitionType_Custom = OTIO_NS::Transition::Type::Custom;

OTIO_API Transition *Transition_create(
        const char *name,
        const char *transition_type,
        OptionalRationalTime in_offset,
        OptionalRationalTime out_offset,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    std::string transition_type_str = std::string();
    if (transition_type != NULL) transition_type_str = transition_type;

    OTIO_NS::RationalTime in_offset_rt = opentime::RationalTime();
    OTIO_NS::RationalTime out_offset_rt = opentime::RationalTime();
    if (in_offset.valid)
        in_offset_rt = CRationalTime_to_CppRationalTime(in_offset.value);
    if (out_offset.valid)
        out_offset_rt = CRationalTime_to_CppRationalTime(out_offset.value);

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    return reinterpret_cast<Transition *>(new OTIO_NS::Transition(
            name_str,
            transition_type_str,
            in_offset_rt,
            out_offset_rt,
            metadataDictionary));
}
OTIO_API bool Transition_overlapping(Transition *self) {
    return reinterpret_cast<OTIO_NS::Transition *>(self)->overlapping();
}
OTIO_API otiostr Transition_transition_type(Transition *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::Transition *>(self)->transition_type();
    otiostr transitionTypeStr = otiostr_create(returnStr.c_str());
    return transitionTypeStr;
}
OTIO_API void Transition_set_transition_type(
        Transition *self, const char *transition_type) {
    reinterpret_cast<OTIO_NS::Transition *>(self)->set_transition_type(
            transition_type);
}
OTIO_API RationalTime Transition_in_offset(Transition *self) {
    opentime::RationalTime rationalTime =
            reinterpret_cast<OTIO_NS::Transition *>(self)->in_offset();
    return CppRationalTime_to_CRationalTime(rationalTime);
}
OTIO_API void Transition_set_in_offset(Transition *self, RationalTime in_offset) {
    reinterpret_cast<OTIO_NS::Transition *>(self)->set_in_offset(
            CRationalTime_to_CppRationalTime(in_offset));
}
OTIO_API RationalTime Transition_out_offset(Transition *self) {
    opentime::RationalTime rationalTime =
            reinterpret_cast<OTIO_NS::Transition *>(self)->out_offset();
    return CppRationalTime_to_CRationalTime(rationalTime);
}
OTIO_API void Transition_set_out_offset(Transition *self, RationalTime out_offset) {
    reinterpret_cast<OTIO_NS::Transition *>(self)->set_out_offset(
            CRationalTime_to_CppRationalTime(out_offset));
}
OTIO_API RationalTime
Transition_duration(Transition *self, OTIOErrorStatus *error_status) {
    opentime::RationalTime rationalTime =
            reinterpret_cast<OTIO_NS::Transition *>(self)->duration(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return CppRationalTime_to_CRationalTime(rationalTime);
}
OTIO_API OptionalTimeRange
Transition_range_in_parent(Transition *self, OTIOErrorStatus *error_status) {
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
            reinterpret_cast<OTIO_NS::Transition *>(self)->range_in_parent(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    if (timeRangeOptional == nonstd::nullopt) return OptionalTimeRange_create_null();
    return OptionalTimeRange_create(CppTimeRange_to_CTimeRange(timeRangeOptional.value()));
}
OTIO_API OptionalTimeRange Transition_trimmed_range_in_parent(
        Transition *self, OTIOErrorStatus *error_status) {
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
            reinterpret_cast<OTIO_NS::Transition *>(self)
                    ->trimmed_range_in_parent(
                            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    if (timeRangeOptional == nonstd::nullopt) return OptionalTimeRange_create_null();
    return OptionalTimeRange_create(CppTimeRange_to_CTimeRange(timeRangeOptional.value()));
}
OTIO_API otiostr Transition_name(Transition *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::Transition *>(self)->name();
    otiostr nameStr = otiostr_create(returnStr.c_str());
    return nameStr;
}
OTIO_API AnyDictionary *Transition_metadata(Transition *self) {
    OTIO_NS::AnyDictionary anyDictionary =
            reinterpret_cast<OTIO_NS::Transition *>(self)->metadata();
    return reinterpret_cast<AnyDictionary *>(
            new OTIO_NS::AnyDictionary(anyDictionary));
}
OTIO_API Composition *Transition_parent(Transition *self) {
    return reinterpret_cast<Composition *>(
            reinterpret_cast<OTIO_NS::Transition *>(self)->parent());
}
OTIO_API void Transition_set_name(Transition *self, const char *name) {
    reinterpret_cast<OTIO_NS::Transition *>(self)->set_name(name);
}
OTIO_API bool Transition_visible(Transition *self) {
    return Composable_visible((Composable *) self);
}

OTIO_API bool Transition_to_json_file(
        Transition *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return Composable_to_json_file(
            (Composable *) self, file_name, error_status, indent);
}
OTIO_API otiostr Transition_to_json_string(
        Transition *self, OTIOErrorStatus *error_status, int indent) {
    return Composable_to_json_string(
            (Composable *) self, error_status, indent);
}
OTIO_API bool
Transition_is_equivalent_to(Transition *self, OTIOSerializableObject *other) {
    return Composable_is_equivalent_to((Composable *) self, other);
}
OTIO_API Transition *
Transition_clone(Transition *self, OTIOErrorStatus *error_status) {
    return (Transition *) Composable_clone((Composable *) self, error_status);
}
OTIO_API otiostr Transition_schema_name(Transition *self) {
    return Composable_schema_name((Composable *) self);
}
OTIO_API int Transition_schema_version(Transition *self) {
    return Composable_schema_version((Composable *) self);
}
