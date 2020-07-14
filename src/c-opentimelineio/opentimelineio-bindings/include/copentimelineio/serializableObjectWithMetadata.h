#pragma once

#include "anyDictionary.h"
#include "errorStatus.h"
#include "serializableObject.h"

#ifdef __cplusplus
extern "C"
{
#endif
    struct SerializableObjectWithMetadata;
    typedef struct SerializableObjectWithMetadata
                                    SerializableObjectWithMetadata;
    SerializableObjectWithMetadata* SerializableObjectWithMetadata_create(
        const char* name, AnyDictionary* metadata);
    const char*
         SerializableObjectWithMetadata_name(SerializableObjectWithMetadata* self);
    void SerializableObjectWithMetadata_set_name(
        SerializableObjectWithMetadata* self, const char* name);
    AnyDictionary* SerializableObjectWithMetadata_metadata(
        SerializableObjectWithMetadata* self);
    bool SerializableObjectWithMetadata_possibly_delete(
        SerializableObjectWithMetadata* self);
    bool SerializableObjectWithMetadata_to_json_file(
        SerializableObjectWithMetadata* self,
        const char*                     file_name,
        OTIOErrorStatus*                error_status,
        int                             indent);
    const char* SerializableObjectWithMetadata_to_json_string(
        SerializableObjectWithMetadata* self,
        OTIOErrorStatus*                error_status,
        int                             indent);
    bool SerializableObjectWithMetadata_is_equivalent_to(
        SerializableObjectWithMetadata* self, SerializableObject* other);
    SerializableObjectWithMetadata* SerializableObjectWithMetadata_clone(
        SerializableObjectWithMetadata* self, OTIOErrorStatus* error_status);
    bool SerializableObjectWithMetadata_is_unknown_schema(
        SerializableObjectWithMetadata* self);
    const char* SerializableObjectWithMetadata_schema_name(
        SerializableObjectWithMetadata* self);
    int SerializableObjectWithMetadata_schema_version(
        SerializableObjectWithMetadata* self);
#ifdef __cplusplus
}
#endif
