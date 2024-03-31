// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/deserialization.h"
#include <opentimelineio/deserialization.h>
#include <any>

OTIO_API bool deserialize_json_from_string(
        const char *input, Any *destination, OTIOErrorStatus *error_status) {
    std::string str = input;
    return OTIO_NS::deserialize_json_from_string(
            str,
            reinterpret_cast<std::any *>(destination),
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
}

OTIO_API bool deserialize_json_from_file(
        const char *file_name, Any *destination, OTIOErrorStatus *error_status) {
    return OTIO_NS::deserialize_json_from_file(
            file_name,
            reinterpret_cast<std::any *>(destination),
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
}
