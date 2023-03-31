// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/serialization.h"
#include "copentime/util.h"
#include <opentimelineio/serialization.h>

OTIO_API const char *serialize_json_to_string(
        Any *value, OTIOErrorStatus *error_status, int indent) {
    std::string returnStr = OTIO_NS::serialize_json_to_string(
            *reinterpret_cast<OTIO_NS::any *>(value),
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status),
            indent);
    return CppString_to_CString(returnStr);
}

OTIO_API bool serialize_json_to_file(
        Any *value,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return OTIO_NS::serialize_json_to_file(
            reinterpret_cast<OTIO_NS::any *>(value),
            file_name,
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status),
            indent);
}
