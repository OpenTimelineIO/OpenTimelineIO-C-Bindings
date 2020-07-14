#pragma once

#include "errorStatus.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    struct UnknownSchema;
    typedef struct UnknownSchema UnknownSchema;
    UnknownSchema*               UnknownSchema_create(
                      const char* original_schema_name, int original_schema_version);
    const char* UnknownSchema_original_schema_name(UnknownSchema* self);
    int         UnknownSchema_original_schema_version(UnknownSchema* self);
    bool       UnknownSchema_is_unknown_schema(UnknownSchema* self);
    bool       UnknownSchema_possibly_delete(UnknownSchema* self);
    bool       UnknownSchema_to_json_file(
              UnknownSchema*   self,
              const char*      file_name,
              OTIOErrorStatus* error_status,
              int              indent);
    const char* UnknownSchema_to_json_string(
        UnknownSchema* self, OTIOErrorStatus* error_status, int indent);
    bool UnknownSchema_is_equivalent_to(
        UnknownSchema* self, SerializableObject* other);
    UnknownSchema*
                UnknownSchema_clone(UnknownSchema* self, OTIOErrorStatus* error_status);
    const char* UnknownSchema_schema_name(UnknownSchema* self);
    int         UnknownSchema_schema_version(UnknownSchema* self);
#ifdef __cplusplus
}
#endif
