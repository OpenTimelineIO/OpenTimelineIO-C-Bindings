// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/gap.h"
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/gap.h>

typedef std::vector<OTIO_NS::Effect *> EffectVectorDef;
typedef std::vector<OTIO_NS::Effect *>::iterator EffectVectorIteratorDef;
typedef std::vector<OTIO_NS::Marker *> MarkerVectorDef;
typedef std::vector<OTIO_NS::Marker *>::iterator MarkerVectorIteratorDef;

OTIO_API Gap *Gap_create_with_source_range(
        OptionalTimeRange source_range,
        const char *name,
        EffectVector *effects,
        MarkerVector *markers,
        AnyDictionary *metadata) {
    OTIO_NS::TimeRange source_range_tr = opentime::TimeRange();
    if (source_range.valid)
        source_range_tr = CTimeRange_to_CppTimeRange(source_range.value);
    std::string name_str = std::string();
    if (name != NULL) name_str = name;
    EffectVectorDef effectVectorDef = EffectVectorDef();
    if (effects != NULL)
        effectVectorDef = *reinterpret_cast<EffectVectorDef *>(effects);
    MarkerVectorDef markerVectorDef = MarkerVectorDef();
    if (markers != NULL)
        markerVectorDef = *reinterpret_cast<MarkerVectorDef *>(markers);
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    return reinterpret_cast<Gap *>(new OTIO_NS::Gap(
            source_range_tr,
            name_str,
            effectVectorDef,
            markerVectorDef,
            metadataDictionary));
}

OTIO_API Gap *Gap_create_with_duration(
        OptionalRationalTime duration,
        const char *name,
        EffectVector *effects,
        MarkerVector *markers,
        AnyDictionary *metadata) {
    opentime::RationalTime duration_rt = opentime::RationalTime();
    if (duration.valid)
        duration_rt = CRationalTime_to_CppRationalTime(duration.value);
    std::string name_str = std::string();
    if (name != NULL) name_str = name;
    EffectVectorDef effectVectorDef = EffectVectorDef();
    if (effects != NULL)
        effectVectorDef = *reinterpret_cast<EffectVectorDef *>(effects);
    MarkerVectorDef markerVectorDef = MarkerVectorDef();
    if (markers != NULL)
        markerVectorDef = *reinterpret_cast<MarkerVectorDef *>(markers);
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);
    return reinterpret_cast<Gap *>(new OTIO_NS::Gap(
            duration_rt,
            name_str,
            effectVectorDef,
            markerVectorDef,
            metadataDictionary));
}

OTIO_API bool Gap_visible(Gap *self) {
    return reinterpret_cast<OTIO_NS::Gap *>(self)->visible();
}

OTIO_API bool Gap_overlapping(Gap *self) { return Item_overlapping((Item *) self); }

OTIO_API OptionalTimeRange Gap_source_range(Gap *self) {
    return Item_source_range((Item *) self);
}

OTIO_API void Gap_set_source_range(Gap *self, OptionalTimeRange source_range) {
    Item_set_source_range((Item *) self, source_range);
}

OTIO_API EffectRetainerVector *Gap_effects(Gap *self) {
    return Item_effects((Item *) self);
}

OTIO_API MarkerRetainerVector *Gap_markers(Gap *self) {
    return Item_markers((Item *) self);
}

OTIO_API RationalTime Gap_duration(Gap *self, OTIOErrorStatus *error_status) {
    return Item_duration((Item *) self, error_status);
}

OTIO_API TimeRange Gap_available_range(Gap *self, OTIOErrorStatus *error_status) {
    return Item_available_range((Item *) self, error_status);
}

OTIO_API TimeRange Gap_trimmed_range(Gap *self, OTIOErrorStatus *error_status) {
    return Item_trimmed_range((Item *) self, error_status);
}

OTIO_API TimeRange Gap_visible_range(Gap *self, OTIOErrorStatus *error_status) {
    return Item_visible_range((Item *) self, error_status);
}

OTIO_API OptionalTimeRange
Gap_trimmed_range_in_parent(Gap *self, OTIOErrorStatus *error_status) {
    return Item_trimmed_range_in_parent((Item *) self, error_status);
}

OTIO_API TimeRange Gap_range_in_parent(Gap *self, OTIOErrorStatus *error_status) {
    return Item_range_in_parent((Item *) self, error_status);
}

OTIO_API RationalTime Gap_transformed_time(
        Gap *self,
        RationalTime time,
        Item *to_item,
        OTIOErrorStatus *error_status) {
    return Item_transformed_time((Item *) self, time, to_item, error_status);
}

OTIO_API TimeRange Gap_transformed_time_range(
        Gap *self,
        TimeRange time_range,
        Item *to_item,
        OTIOErrorStatus *error_status) {
    return Item_transformed_time_range(
            (Item *) self, time_range, to_item, error_status);
}

OTIO_API Composition *Gap_parent(Gap *self) {
    return Composable_parent((Composable *) self);
}

OTIO_API const char *Gap_name(Gap *self) {
    return Composable_name((Composable *) self);
}

OTIO_API AnyDictionary *Gap_metadata(Gap *self) {
    return Composable_metadata((Composable *) self);
}

OTIO_API void Gap_set_name(Gap *self, const char *name) {
    Composable_set_name((Composable *) self, name);
}

OTIO_API bool Gap_to_json_file(
        Gap *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        OTIOSchemaVersionMap *schema_version_targets,
        int indent) {
    return Composable_to_json_file(
            (Composable *) self,
            file_name,
            error_status,
            schema_version_targets,
            indent);
}

OTIO_API const char *
Gap_to_json_string(
        Gap *self,
        OTIOErrorStatus *error_status,
        OTIOSchemaVersionMap *schema_version_targets,
        int indent) {
    return Composable_to_json_string(
            (Composable *) self,
            error_status,
            schema_version_targets,
            indent);
}

OTIO_API bool Gap_is_equivalent_to(Gap *self, OTIOSerializableObject *other) {
    return Composable_is_equivalent_to((Composable *) self, other);
}

OTIO_API Gap *Gap_clone(Gap *self, OTIOErrorStatus *error_status) {
    return (Gap *) Composable_clone((Composable *) self, error_status);
}

OTIO_API const char *Gap_schema_name(Gap *self) {
    return Composable_schema_name((Composable *) self);
}

OTIO_API int Gap_schema_version(Gap *self) {
    return Composable_schema_version((Composable *) self);
}
