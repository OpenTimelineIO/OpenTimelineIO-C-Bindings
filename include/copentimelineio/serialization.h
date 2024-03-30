// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#pragma once

#include "typeRegistry.h"
#include "errorStatus.h"
#include "any.h"
#include <stdbool.h>

#ifdef __cplusplus
# define OTIO_API extern "C"
#else
# define OTIO_API
#endif

OTIO_API const char *serialize_json_to_string(
        Any *value,
        OTIOSchemaVersionMap *schema_version_targets,
        OTIOErrorStatus *error_status,
        int indent);

OTIO_API bool serialize_json_to_file(
        Any *value,
        const char *file_name,
        OTIOSchemaVersionMap *schema_version_targets,
        OTIOErrorStatus *error_status,
        int indent);
