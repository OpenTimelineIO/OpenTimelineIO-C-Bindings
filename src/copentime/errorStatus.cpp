// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/errorStatus.h"
#include <opentime/errorStatus.h>
#include <string.h>

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
        OpenTime_ErrorStatus_Outcome var1) {
    std::string returnStr = opentime::ErrorStatus::outcome_to_string(
            static_cast<opentime::ErrorStatus::Outcome>(var1));
    return strdup(returnStr.c_str());
}
OTIO_API OpenTime_ErrorStatus_Outcome
OpenTimeErrorStatus_get_outcome(OpenTimeErrorStatus* self)
{
    return static_cast<OpenTime_ErrorStatus_Outcome>(
        reinterpret_cast<opentime::ErrorStatus*>(self)->outcome);
}
OTIO_API void OpenTimeErrorStatus_destroy(OpenTimeErrorStatus *self) {
    delete reinterpret_cast<opentime::ErrorStatus *>(self);
}
