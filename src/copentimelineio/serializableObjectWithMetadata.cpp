// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/serializableObjectWithMetadata.h"
#include "copentime/util.h"
#include <opentimelineio/serializableObjectWithMetadata.h>

OTIO_API SerializableObjectWithMetadata *SerializableObjectWithMetadata_create(
        const char *name, AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) { name_str = name; }
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL) {
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);
    }
    return reinterpret_cast<SerializableObjectWithMetadata *>(
            new OTIO_NS::SerializableObjectWithMetadata(
                    name_str, metadataDictionary));
}
OTIO_API const char *
SerializableObjectWithMetadata_name(SerializableObjectWithMetadata *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::SerializableObjectWithMetadata *>(self)
                    ->name();
    return CppString_to_CString(returnStr);
}
OTIO_API void SerializableObjectWithMetadata_set_name(
        SerializableObjectWithMetadata *self, const char *name) {
    reinterpret_cast<OTIO_NS::SerializableObjectWithMetadata *>(self)
            ->set_name(name);
}
OTIO_API AnyDictionary *SerializableObjectWithMetadata_metadata(
        SerializableObjectWithMetadata *self) {
    OTIO_NS::AnyDictionary anyDictionary =
            reinterpret_cast<OTIO_NS::SerializableObjectWithMetadata *>(self)
                    ->metadata();
    return reinterpret_cast<AnyDictionary *>(
            new OTIO_NS::AnyDictionary(anyDictionary));
}

OTIO_API bool SerializableObjectWithMetadata_to_json_file(
        SerializableObjectWithMetadata *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}
OTIO_API const char *SerializableObjectWithMetadata_to_json_string(
        SerializableObjectWithMetadata *self,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}
OTIO_API bool SerializableObjectWithMetadata_is_equivalent_to(
        SerializableObjectWithMetadata *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}
OTIO_API SerializableObjectWithMetadata *SerializableObjectWithMetadata_clone(
        SerializableObjectWithMetadata *self, OTIOErrorStatus *error_status) {
    return (SerializableObjectWithMetadata *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}
OTIO_API bool SerializableObjectWithMetadata_is_unknown_schema(
        SerializableObjectWithMetadata *self) {
    return SerializableObject_is_unknown_schema(reinterpret_cast<OTIOSerializableObject *>(self));
}
OTIO_API const char *SerializableObjectWithMetadata_schema_name(
        SerializableObjectWithMetadata *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}
OTIO_API int SerializableObjectWithMetadata_schema_version(
        SerializableObjectWithMetadata *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}

