// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/unknownSchema.h"
#include <copentimelineio/serializableObject.h>
#include <opentimelineio/unknownSchema.h>
#include <string.h>

OTIO_API UnknownSchema *UnknownSchema_create(
        const char *original_schema_name, int original_schema_version) {
    return reinterpret_cast<UnknownSchema *>(new OTIO_NS::UnknownSchema(
            original_schema_name, original_schema_version));
}

OTIO_API otiostr UnknownSchema_original_schema_name(UnknownSchema *self) {
    std::string returnStr = reinterpret_cast<OTIO_NS::UnknownSchema *>(self)
            ->original_schema_name();
    otiostr schemaNameStr = otiostr_create(returnStr.c_str());
    return schemaNameStr;
}

OTIO_API int UnknownSchema_original_schema_version(UnknownSchema *self) {
    return reinterpret_cast<OTIO_NS::UnknownSchema *>(self)
            ->original_schema_version();
}

OTIO_API bool UnknownSchema_is_unknown_schema(UnknownSchema *self) {
    return reinterpret_cast<OTIO_NS::UnknownSchema *>(self)
            ->is_unknown_schema();
}

OTIO_API bool UnknownSchema_to_json_file(
        UnknownSchema *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}

OTIO_API otiostr UnknownSchema_to_json_string(
        UnknownSchema *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}

OTIO_API bool UnknownSchema_is_equivalent_to(
        UnknownSchema *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}

OTIO_API UnknownSchema *
UnknownSchema_clone(UnknownSchema *self, OTIOErrorStatus *error_status) {
    return (UnknownSchema *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}

OTIO_API otiostr UnknownSchema_schema_name(UnknownSchema *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}

OTIO_API int UnknownSchema_schema_version(UnknownSchema *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
