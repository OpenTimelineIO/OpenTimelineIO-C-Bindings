#pragma once

#include "anyDictionary.h"
#include "copentime/timeRange.h"
#include "errorStatus.h"
#include <stdbool.h>

#ifdef __cplusplus
# define OTIO_API extern "C"
#else
# define OTIO_API
#endif

struct ExternalReference;
typedef struct ExternalReference ExternalReference;

OTIO_API ExternalReference *ExternalReference_create(
        const char *target_url,
        AnyDictionary *metadata);

OTIO_API ExternalReference *ExternalReference_create_with_available_range(
        const char *target_url,
        TimeRange available_range,
        AnyDictionary *metadata);

OTIO_API const char *ExternalReference_target_url(ExternalReference *self);

OTIO_API void ExternalReference_set_target_url(
        ExternalReference *self, const char *target_url);

OTIO_API bool ExternalReference_available_range(ExternalReference *self,
                                                TimeRange &availableRange);

OTIO_API void ExternalReference_set_available_range(
        ExternalReference *self, TimeRange available_range);

OTIO_API void ExternalReference_set_available_range_null(ExternalReference *self);

OTIO_API bool ExternalReference_is_missing_reference(ExternalReference *self);

OTIO_API const char *ExternalReference_name(ExternalReference *self);

OTIO_API void ExternalReference_set_name(ExternalReference *self, const char *name);

OTIO_API AnyDictionary *ExternalReference_metadata(ExternalReference *self);

OTIO_API bool ExternalReference_to_json_file(
        ExternalReference *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent);

OTIO_API const char *ExternalReference_to_json_string(
        ExternalReference *self, OTIOErrorStatus *error_status, int indent);

OTIO_API bool ExternalReference_is_equivalent_to(
        ExternalReference *self, OTIOSerializableObject *other);

OTIO_API ExternalReference *ExternalReference_clone(
        ExternalReference *self, OTIOErrorStatus *error_status);

OTIO_API const char *ExternalReference_schema_name(ExternalReference *self);

OTIO_API int ExternalReference_schema_version(ExternalReference *self);
