// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/generatorReference.h"
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/generatorReference.h>

OTIO_API GeneratorReference *GeneratorReference_create(
        const char *name,
        const char *generator_kind,
        OptionalTimeRange available_range,
        AnyDictionary *parameters,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    std::string generator_kind_str = std::string();
    if (generator_kind != NULL) generator_kind_str = generator_kind;

    OTIO_NS::AnyDictionary parametersDictionary = OTIO_NS::AnyDictionary();
    if (parameters != NULL)
        parametersDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(parameters);

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    nonstd::optional<opentime::TimeRange> timeRangeOptional = nonstd::nullopt;
    if (available_range.valid)
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
                CTimeRange_to_CppTimeRange(available_range.value));
    return reinterpret_cast<GeneratorReference *>(
            new OTIO_NS::GeneratorReference(
                    name,
                    generator_kind,
                    timeRangeOptional,
                    parametersDictionary,
                    metadataDictionary));
}
OTIO_API const char *GeneratorReference_generator_kind(GeneratorReference *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::GeneratorReference *>(self)
                    ->generator_kind();
    return _strdup(returnStr.c_str());
}
OTIO_API void GeneratorReference_set_generator_kind(
        GeneratorReference *self, const char *generator_kind) {
    reinterpret_cast<OTIO_NS::GeneratorReference *>(self)
            ->set_generator_kind(generator_kind);
}
OTIO_API AnyDictionary *GeneratorReference_parameters(GeneratorReference *self) {
    OTIO_NS::AnyDictionary anyDictionary =
            reinterpret_cast<OTIO_NS::GeneratorReference *>(self)->parameters();
    return reinterpret_cast<AnyDictionary *>(
            new OTIO_NS::AnyDictionary(anyDictionary));
}
OTIO_API OptionalTimeRange GeneratorReference_available_range(GeneratorReference *self) {
    return MediaReference_available_range((MediaReference *) self);
}
OTIO_API void GeneratorReference_set_available_range(
        GeneratorReference *self, OptionalTimeRange available_range) {
    MediaReference_set_available_range(
            (MediaReference *) self, available_range);
}
OTIO_API bool GeneratorReference_is_missing_reference(GeneratorReference *self) {
    return MediaReference_is_missing_reference((MediaReference *) self);
}
OTIO_API const char *GeneratorReference_name(GeneratorReference *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}
OTIO_API void GeneratorReference_set_name(GeneratorReference *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}
OTIO_API AnyDictionary *GeneratorReference_metadata(GeneratorReference *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool GeneratorReference_to_json_file(
        GeneratorReference *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}
OTIO_API const char *GeneratorReference_to_json_string(
        GeneratorReference *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}
OTIO_API bool GeneratorReference_is_equivalent_to(
        GeneratorReference *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}
OTIO_API GeneratorReference *GeneratorReference_clone(
        GeneratorReference *self, OTIOErrorStatus *error_status) {
    return (GeneratorReference *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}
OTIO_API const char *GeneratorReference_schema_name(GeneratorReference *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}
OTIO_API int GeneratorReference_schema_version(GeneratorReference *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}

