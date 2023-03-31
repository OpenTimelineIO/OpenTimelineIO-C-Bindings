// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/errorStatus.h"
#include "copentime/util.h"
#include <opentime/errorStatus.h>

OTIO_API OpenTimeErrorStatus *OpenTimeErrorStatus_create() {
    return reinterpret_cast<OpenTimeErrorStatus *>(
            new opentime::ErrorStatus());
}
OTIO_API OpenTimeErrorStatus *OpenTimeErrorStatus_create_with_outcome(
        OpenTime_ErrorStatus_Outcome in_outcome) {
    return reinterpret_cast<OpenTimeErrorStatus *>(new opentime::ErrorStatus(
            static_cast<opentime::ErrorStatus::Outcome>(in_outcome)));
}
OTIO_API OpenTimeErrorStatus *OpenTimeErrorStatus_create_with_outcome_and_details(
        OpenTime_ErrorStatus_Outcome in_outcome, const char *in_details) {
    return reinterpret_cast<OpenTimeErrorStatus *>(new opentime::ErrorStatus(
            static_cast<opentime::ErrorStatus::Outcome>(in_outcome),
            in_details));
}
OTIO_API const char *OpenTimeErrorStatus_outcome_to_string(
        OpenTimeErrorStatus *self, OpenTime_ErrorStatus_Outcome var1) {
    std::string returnStr = opentime::ErrorStatus::outcome_to_string(
            static_cast<opentime::ErrorStatus::Outcome>(var1));
    return CppString_to_CString(returnStr);
}
OTIO_API void OpenTimeErrorStatus_destroy(OpenTimeErrorStatus *self) {
    delete reinterpret_cast<opentime::ErrorStatus *>(self);
}
