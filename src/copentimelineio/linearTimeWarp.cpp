// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/linearTimeWarp.h"
#include <copentimelineio/effect.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/linearTimeWarp.h>

OTIO_API LinearTimeWarp* LinearTimeWarp_create(
    const char*    name,
    const char*    effect_name,
    double         time_scalar,
    AnyDictionary* metadata)
{
    std::string name_str = std::string();
    if(name != NULL) name_str = name;

    std::string effect_name_str = "LinearTimeWarp";
    if(effect_name != NULL) effect_name_str = effect_name;

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if(metadata != NULL)
        metadataDictionary =
            *reinterpret_cast<OTIO_NS::AnyDictionary*>(metadata);

    return reinterpret_cast<LinearTimeWarp*>(new OTIO_NS::LinearTimeWarp(
        name_str, effect_name_str, time_scalar, metadataDictionary));
}
OTIO_API double LinearTimeWarp_time_scalar(LinearTimeWarp* self)
{
    return reinterpret_cast<OTIO_NS::LinearTimeWarp*>(self)->time_scalar();
}
OTIO_API void
LinearTimeWarp_set_time_scalar(LinearTimeWarp* self, double time_scalar)
{
    reinterpret_cast<OTIO_NS::LinearTimeWarp*>(self)->set_time_scalar(
        time_scalar);
}
OTIO_API otiostr LinearTimeWarp_effect_name(LinearTimeWarp* self)
{
    return Effect_effect_name((Effect*) self);
}
OTIO_API void LinearTimeWarp_set_effect_name(
    LinearTimeWarp* self, const char* effect_name)
{
    Effect_set_name((Effect*) self, effect_name);
}
OTIO_API otiostr LinearTimeWarp_name(LinearTimeWarp* self)
{
    return Effect_name((Effect*) self);
}
OTIO_API void LinearTimeWarp_set_name(LinearTimeWarp* self, const char* name)
{
    Effect_set_name((Effect*) self, name);
}
OTIO_API AnyDictionary* LinearTimeWarp_metadata(LinearTimeWarp* self)
{
    return Effect_metadata((Effect*) self);
}

OTIO_API bool LinearTimeWarp_to_json_file(
    LinearTimeWarp*  self,
    const char*      file_name,
    OTIOErrorStatus* error_status,
    int              indent)
{
    return Effect_to_json_file(
        (Effect*) self, file_name, error_status, indent);
}
OTIO_API otiostr LinearTimeWarp_to_json_string(
    LinearTimeWarp* self, OTIOErrorStatus* error_status, int indent)
{
    return Effect_to_json_string((Effect*) self, error_status, indent);
}
OTIO_API bool LinearTimeWarp_is_equivalent_to(
    LinearTimeWarp* self, OTIOSerializableObject* other)
{
    return Effect_is_equivalent_to((Effect*) self, other);
}
OTIO_API LinearTimeWarp*
LinearTimeWarp_clone(LinearTimeWarp* self, OTIOErrorStatus* error_status)
{
    return (LinearTimeWarp*) Effect_clone((Effect*) self, error_status);
}
OTIO_API otiostr LinearTimeWarp_schema_name(LinearTimeWarp* self)
{
    return Effect_schema_name((Effect*) self);
}
OTIO_API int LinearTimeWarp_schema_version(LinearTimeWarp* self)
{
    return Effect_schema_version((Effect*) self);
}
