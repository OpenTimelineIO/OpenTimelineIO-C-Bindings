// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/composable.h"
#include "copentimelineio/serializableObjectWithMetadata.h"
#include "copentime/util.h"
#include <opentime/rationalTime.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/composable.h>
#include <opentimelineio/composition.h>
#include <opentimelineio/errorStatus.h>

typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Composable>
        ComposableRetainer;

OTIO_API RetainerComposable *
RetainerComposable_create(Composable *obj) {
    return reinterpret_cast<RetainerComposable *>(new ComposableRetainer(
            reinterpret_cast<OTIO_NS::Composable *>(obj)));
}

OTIO_API Composable *
RetainerComposable_take_value(RetainerComposable *self) {
    return reinterpret_cast<Composable *>(
            reinterpret_cast<ComposableRetainer *>(self)->take_value());
}

OTIO_API Composable *
RetainerComposable_value(RetainerComposable *self) {
    return reinterpret_cast<Composable *>(
            reinterpret_cast<ComposableRetainer *>(self)->value);
}

OTIO_API void
RetainerComposable_managed_destroy(RetainerComposable *self) {
    delete reinterpret_cast<ComposableRetainer *>(self);
}

OTIO_API Composable *
Composable_create() {
    return reinterpret_cast<Composable *>(new OTIO_NS::Composable());
}

OTIO_API Composable *
Composable_create_with_name_and_metadata(
        const char *name, AnyDictionary *metadata) {
    return reinterpret_cast<Composable *>(new OTIO_NS::Composable(
            name, *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata)));
}

OTIO_API bool
Composable_visible(Composable *self) {
    return reinterpret_cast<OTIO_NS::Composable *>(self)->visible();
}

OTIO_API bool
Composable_overlapping(Composable *self) {
    return reinterpret_cast<OTIO_NS::Composable *>(self)->overlapping();
}

OTIO_API Composition *
Composable_parent(Composable *self) {
    return reinterpret_cast<Composition *>(
            reinterpret_cast<OTIO_NS::Composable *>(self)->parent());
}

OTIO_API RationalTime
Composable_duration(Composable *self, OTIOErrorStatus *error_status) {
    opentime::RationalTime rationalTime =
            reinterpret_cast<OTIO_NS::Composable *>(self)->duration(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return CppRationalTime_to_CRationalTime(rationalTime);
}

OTIO_API const char *
Composable_name(Composable *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}
OTIO_API AnyDictionary *
Composable_metadata(Composable *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API void
Composable_set_name(Composable *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}

OTIO_API bool
Composable_to_json_file(
        Composable *self,
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

OTIO_API const char *
Composable_to_json_string(
        Composable *self,
        OTIOErrorStatus *error_status,
        OTIOSchemaVersionMap *schema_version_targets,
        int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self),
            error_status,
            schema_version_targets,
            indent);
}

OTIO_API bool
Composable_is_equivalent_to(Composable *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}

OTIO_API Composable *
Composable_clone(Composable *self, OTIOErrorStatus *error_status) {
    return (Composable *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}

OTIO_API const char *
Composable_schema_name(Composable *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}

OTIO_API int
Composable_schema_version(Composable *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
