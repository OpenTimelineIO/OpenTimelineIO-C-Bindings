#pragma once

#include "any.h"
#include "errorStatus.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    bool deserialize_json_from_string(
        const char* input, Any* destination, OTIOErrorStatus* error_status);
    bool deserialize_json_from_file(
        const char* file_name, Any* destination, OTIOErrorStatus* error_status);
#ifdef __cplusplus
}
#endif
