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

struct MediaReference;
typedef struct MediaReference MediaReference;

OTIO_API MediaReference *MediaReference_create(
        const char *name, AnyDictionary *metadata);
OTIO_API MediaReference *MediaReference_create_with_available_range(
        const char *name, TimeRange available_range, AnyDictionary *metadata);
OTIO_API bool MediaReference_available_range(MediaReference *self, TimeRange &timeRange);
OTIO_API void MediaReference_set_available_range(
        MediaReference *self, TimeRange available_range);
OTIO_API void MediaReference_set_available_range_null(
        MediaReference *self);
OTIO_API bool MediaReference_is_missing_reference(MediaReference *self);
OTIO_API const char *MediaReference_name(MediaReference *self);
OTIO_API void MediaReference_set_name(MediaReference *self, const char *name);
OTIO_API AnyDictionary *MediaReference_metadata(MediaReference *self);
OTIO_API bool MediaReference_to_json_file(
        MediaReference *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent);
OTIO_API const char *MediaReference_to_json_string(
        MediaReference *self, OTIOErrorStatus *error_status, int indent);
OTIO_API bool MediaReference_is_equivalent_to(
        MediaReference *self, OTIOSerializableObject *other);
OTIO_API MediaReference *
MediaReference_clone(MediaReference *self, OTIOErrorStatus *error_status);
OTIO_API const char *MediaReference_schema_name(MediaReference *self);
OTIO_API int MediaReference_schema_version(MediaReference *self);
