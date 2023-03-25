// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/serializableObject.h"
#include <opentimelineio/serializableObject.h>
#include <string.h>

typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::SerializableObject>
    SerializableObjectRetainer;

OTIO_API RetainerSerializableObject*
RetainerSerializableObject_create(OTIOSerializableObject* obj)
{
    return reinterpret_cast<RetainerSerializableObject*>(
        new SerializableObjectRetainer(
            reinterpret_cast<OTIO_NS::SerializableObject*>(obj)));
}

OTIO_API OTIOSerializableObject*
RetainerSerializableObject_take_value(RetainerSerializableObject* self)
{
    return reinterpret_cast<OTIOSerializableObject*>(
        reinterpret_cast<SerializableObjectRetainer*>(self)->take_value());
}

OTIO_API OTIOSerializableObject*
RetainerSerializableObject_value(RetainerSerializableObject* self)
{
    return reinterpret_cast<OTIOSerializableObject*>(
        reinterpret_cast<SerializableObjectRetainer*>(self)->value);
}

OTIO_API void
RetainerSerializableObject_managed_destroy(RetainerSerializableObject* self)
{
    delete reinterpret_cast<SerializableObjectRetainer*>(self);
}

OTIO_API OTIOSerializableObject*
SerializableObject_create()
{
    return reinterpret_cast<OTIOSerializableObject*>(
        new OTIO_NS::SerializableObject());
}

OTIO_API bool
SerializableObject_to_json_file(
    OTIOSerializableObject* self,
    const char*         file_name,
    OTIOErrorStatus*    error_status,
    int                 indent)
{
    return reinterpret_cast<OTIO_NS::SerializableObject*>(self)
        ->to_json_file(
            file_name,
            reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status),
            indent);
}

OTIO_API otiostr
SerializableObject_to_json_string(
    OTIOSerializableObject* self, OTIOErrorStatus* error_status, int indent)
{
    std::string returnStr =
        reinterpret_cast<OTIO_NS::SerializableObject*>(self)
            ->to_json_string(
                reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status),
                indent);
    otiostr jsonStr = otiostr_create(returnStr.c_str());
    return jsonStr;
}

OTIO_API OTIOSerializableObject*
SerializableObject_from_json_file(
    const char* file_name, OTIOErrorStatus* error_status)
{
    OTIO_NS::SerializableObject* obj =
        OTIO_NS::SerializableObject::from_json_file(
            file_name,
            reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
    return reinterpret_cast<OTIOSerializableObject*>(obj);
}

OTIO_API OTIOSerializableObject*
SerializableObject_from_json_string(
    const char* input, OTIOErrorStatus* error_status)
{
    OTIO_NS::SerializableObject* obj =
        OTIO_NS::SerializableObject::from_json_string(
            input, reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
    return reinterpret_cast<OTIOSerializableObject*>(obj);
}

OTIO_API bool
SerializableObject_is_equivalent_to(
    OTIOSerializableObject* self, OTIOSerializableObject* other)
{
    return reinterpret_cast<OTIO_NS::SerializableObject*>(self)
        ->is_equivalent_to(
            *reinterpret_cast<OTIO_NS::SerializableObject*>(other));
}

OTIO_API OTIOSerializableObject*
SerializableObject_clone(
    OTIOSerializableObject* self, OTIOErrorStatus* error_status)
{
    OTIO_NS::SerializableObject* obj =
        reinterpret_cast<OTIO_NS::SerializableObject*>(self)->clone(
            reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
    return reinterpret_cast<OTIOSerializableObject*>(obj);
}

/*OTIO_API AnyDictionary*
 SerializableObject_dynamic_fields(SerializableObject* self)
{
    return reinterpret_cast<AnyDictionary*>(
        reinterpret_cast<OTIO_NS::SerializableObject*>(self)
            ->dynamic_fields());
}*/

OTIO_API bool
SerializableObject_is_unknown_schema(OTIOSerializableObject* self)
{
    return reinterpret_cast<OTIO_NS::SerializableObject*>(self)
        ->is_unknown_schema();
}

OTIO_API otiostr
SerializableObject_schema_name(OTIOSerializableObject* self)
{
    std::string returnStr =
        reinterpret_cast<OTIO_NS::SerializableObject*>(self)->schema_name();
    otiostr schemaNameStr = otiostr_create(returnStr.c_str());
    return schemaNameStr;
}

OTIO_API int
SerializableObject_schema_version(OTIOSerializableObject* self)
{
    return reinterpret_cast<OTIO_NS::SerializableObject*>(self)
        ->schema_version();
}
