// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/timeEffect.h"
#include <copentimelineio/effect.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/timeEffect.h>

OTIO_API TimeEffect* TimeEffect_create(
    const char* name, const char* effect_name, AnyDictionary* metadata)
{
    return reinterpret_cast<TimeEffect*>(new OTIO_NS::TimeEffect(
        name,
        effect_name,
        *reinterpret_cast<OTIO_NS::AnyDictionary*>(metadata)));
}
OTIO_API const char* TimeEffect_effect_name(TimeEffect* self)
{
    return Effect_effect_name((Effect*) self);
}
OTIO_API void TimeEffect_set_effect_name(TimeEffect* self, const char* effect_name)
{
    Effect_set_effect_name((Effect*) self, effect_name);
}
OTIO_API const char* TimeEffect_name(TimeEffect* self)
{
    return Effect_name((Effect*) self);
}
OTIO_API void TimeEffect_set_name(TimeEffect* self, const char* name)
{
    Effect_set_name((Effect*) self, name);
}
OTIO_API AnyDictionary* TimeEffect_metadata(TimeEffect* self)
{
    return Effect_metadata((Effect*) self);
}

OTIO_API bool TimeEffect_to_json_file(
    TimeEffect*      self,
    const char*      file_name,
    OTIOErrorStatus* error_status,
    int              indent)
{
    return Effect_to_json_file(
        (Effect*) self, file_name, error_status, indent);
}
OTIO_API const char* TimeEffect_to_json_string(
    TimeEffect* self, OTIOErrorStatus* error_status, int indent)
{
    return Effect_to_json_string((Effect*) self, error_status, indent);
}
OTIO_API bool
TimeEffect_is_equivalent_to(TimeEffect* self, OTIOSerializableObject* other)
{
    return Effect_is_equivalent_to((Effect*) self, other);
}
OTIO_API TimeEffect*
TimeEffect_clone(TimeEffect* self, OTIOErrorStatus* error_status)
{
    return (TimeEffect*) Effect_clone((Effect*) self, error_status);
}
OTIO_API const char* TimeEffect_schema_name(TimeEffect* self)
{
    return Effect_schema_name((Effect*) self);
}
OTIO_API int TimeEffect_schema_version(TimeEffect* self)
{
    return Effect_schema_version((Effect*) self);
}
