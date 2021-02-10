#include "copentimelineio/missingReference.h"
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/missingReference.h>

OTIO_API MissingReference* MissingReference_create(
    const char* name, TimeRange* available_range, AnyDictionary* metadata)
{
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
        nonstd::nullopt;
    if(available_range != NULL)
    {
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
            *reinterpret_cast<opentime::TimeRange*>(available_range));
    }
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if(metadata != NULL)
        metadataDictionary =
            *reinterpret_cast<OTIO_NS::AnyDictionary*>(metadata);

    std::string name_str = std::string();
    if(name != NULL) name_str = name;
    return reinterpret_cast<MissingReference*>(
        new OTIO_NS::MissingReference(
            name_str, timeRangeOptional, metadataDictionary));
}
OTIO_API bool MissingReference_is_missing_reference(MissingReference* self)
{
    return reinterpret_cast<OTIO_NS::MissingReference*>(self)
        ->is_missing_reference();
}
OTIO_API TimeRange* MissingReference_available_range(MissingReference* self)
{
    return MediaReference_available_range((MediaReference*) self);
}
OTIO_API void MissingReference_set_available_range(
    MissingReference* self, TimeRange* available_range)
{
    MediaReference_set_available_range(
        (MediaReference*) self, available_range);
}
OTIO_API const char* MissingReference_name(MissingReference* self)
{
    return MediaReference_name((MediaReference*) self);
}
OTIO_API void MissingReference_set_name(MissingReference* self, const char* name)
{
    SerializableObjectWithMetadata_set_name(
        (SerializableObjectWithMetadata*) self, name);
}
OTIO_API AnyDictionary* MissingReference_metadata(MissingReference* self)
{
    return SerializableObjectWithMetadata_metadata(
        (SerializableObjectWithMetadata*) self);
}

OTIO_API bool MissingReference_to_json_file(
    MissingReference* self,
    const char*       file_name,
    OTIOErrorStatus*  error_status,
    int               indent)
{
    return SerializableObject_to_json_file(
        reinterpret_cast<OTIOSerializableObject*>(self), file_name, error_status, indent);
}
OTIO_API const char* MissingReference_to_json_string(
    MissingReference* self, OTIOErrorStatus* error_status, int indent)
{
    return SerializableObject_to_json_string(
        reinterpret_cast<OTIOSerializableObject*>(self), error_status, indent);
}
OTIO_API bool MissingReference_is_equivalent_to(
    MissingReference* self, OTIOSerializableObject* other)
{
    return SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(self), other);
}
OTIO_API MissingReference* MissingReference_clone(
    MissingReference* self, OTIOErrorStatus* error_status)
{
    return (MissingReference*) SerializableObject_clone(
        reinterpret_cast<OTIOSerializableObject*>(self), error_status);
}
OTIO_API const char* MissingReference_schema_name(MissingReference* self)
{
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject*>(self));
}
OTIO_API int MissingReference_schema_version(MissingReference* self)
{
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject*>(self));
}
