// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/missingReference.h"
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/missingReference.h>
#include <optional>

OTIO_API MissingReference *MissingReference_create(
        const char *name, OptionalTimeRange available_range, AnyDictionary *metadata) {
    std::optional<opentime::TimeRange> timeRangeOptional = std::nullopt;
    if (available_range.valid)
        timeRangeOptional = std::optional<opentime::TimeRange>(
                CTimeRange_to_CppTimeRange(available_range.value));
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    std::string name_str = std::string();
    if (name != NULL) name_str = name;
    return reinterpret_cast<MissingReference *>(
            new OTIO_NS::MissingReference(
                    name_str, timeRangeOptional, metadataDictionary));
}
OTIO_API bool MissingReference_is_missing_reference(MissingReference *self) {
    return reinterpret_cast<OTIO_NS::MissingReference *>(self)
            ->is_missing_reference();
}
OTIO_API OptionalTimeRange MissingReference_available_range(MissingReference *self) {
    return MediaReference_available_range((MediaReference *) self);
}
OTIO_API void MissingReference_set_available_range(
        MissingReference *self, OptionalTimeRange available_range) {
    MediaReference_set_available_range(
            (MediaReference *) self, available_range);
}
OTIO_API const char *MissingReference_name(MissingReference *self) {
    return MediaReference_name((MediaReference *) self);
}
OTIO_API void MissingReference_set_name(MissingReference *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}
OTIO_API AnyDictionary *MissingReference_metadata(MissingReference *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool MissingReference_to_json_file(
        MissingReference *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        OTIOSchemaVersionMap *schema_version_targets,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self),
            file_name,
            error_status,
            schema_version_targets,
            indent);
}
OTIO_API const char *MissingReference_to_json_string(
        MissingReference *self,
        OTIOErrorStatus *error_status,
        OTIOSchemaVersionMap *schema_version_targets,
        int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self),
            error_status,
            schema_version_targets,
            indent);
}
OTIO_API bool MissingReference_is_equivalent_to(
        MissingReference *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}
OTIO_API MissingReference *MissingReference_clone(
        MissingReference *self, OTIOErrorStatus *error_status) {
    return (MissingReference *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}
OTIO_API const char *MissingReference_schema_name(MissingReference *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}
OTIO_API int MissingReference_schema_version(MissingReference *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
