// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/errorStatus.h"
#include "copentime/util.h"
#include <opentimelineio/errorStatus.h>

OTIO_API OTIOErrorStatus*
OTIOErrorStatus_create()
{
    return reinterpret_cast<OTIOErrorStatus*>(new OTIO_NS::ErrorStatus());
}

OTIO_API OTIOErrorStatus*
OTIOErrorStatus_create_with_outcome(OTIO_ErrorStatus_Outcome in_outcome)
{
    return reinterpret_cast<OTIOErrorStatus*>(new OTIO_NS::ErrorStatus(
        static_cast<OTIO_NS::ErrorStatus::Outcome>(in_outcome)));
}

OTIO_API OTIOErrorStatus*
OTIOErrorStatus_create_with_outcome_details_serializable_object(
    OTIO_ErrorStatus_Outcome in_outcome,
    const char*              in_details,
    OTIOSerializableObject*  object)
{
    return reinterpret_cast<OTIOErrorStatus*>(new OTIO_NS::ErrorStatus(
        static_cast<OTIO_NS::ErrorStatus::Outcome>(in_outcome),
        in_details,
        reinterpret_cast<OTIO_NS::SerializableObject*>(object)));
}

OTIO_API const char*
OTIOErrorStatus_outcome_to_string(OTIO_ErrorStatus_Outcome var1)
{
    std::string returnStr = OTIO_NS::ErrorStatus::outcome_to_string(
        static_cast<OTIO_NS::ErrorStatus::Outcome>(var1));
    return CppString_to_CString(returnStr);
}

OTIO_API OTIO_ErrorStatus_Outcome
OTIOErrorStatus_get_outcome(OTIOErrorStatus* self)
{
    return static_cast<OTIO_ErrorStatus_Outcome>(
        reinterpret_cast<OTIO_NS::ErrorStatus*>(self)->outcome);
}

OTIO_API void
OTIOErrorStatus_destroy(OTIOErrorStatus* self)
{
    delete reinterpret_cast<OTIO_NS::ErrorStatus*>(self);
}
