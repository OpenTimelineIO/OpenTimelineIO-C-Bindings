// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/serialization.h"
#include <opentimelineio/serialization.h>
#include <string.h>
#include <any>

OTIO_API const char *serialize_json_to_string(
        Any *value, OTIOErrorStatus *error_status, int indent) {
    std::string returnStr = OTIO_NS::serialize_json_to_string(
            *reinterpret_cast<std::any *>(value),
            {},
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status),
            indent);
    return strdup(returnStr.c_str());
}

OTIO_API bool serialize_json_to_file(
        Any *value,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return OTIO_NS::serialize_json_to_file(
            reinterpret_cast<std::any *>(value),
            file_name,
            {},
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status),
            indent);
}
