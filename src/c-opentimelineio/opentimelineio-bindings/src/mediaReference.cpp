#include "copentimelineio/mediaReference.h"
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/mediaReference.h>

OTIO_API MediaReference* MediaReference_create(
    const char* name, TimeRange* available_range, AnyDictionary* metadata)
{
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
        nonstd::nullopt;
    if(available_range != NULL)
    {
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
            *reinterpret_cast<opentime::TimeRange*>(available_range));
    }

    std::string name_str = std::string();
    if(name != NULL) name_str = name;

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if(metadata != NULL)
        metadataDictionary =
            *reinterpret_cast<OTIO_NS::AnyDictionary*>(metadata);

    return reinterpret_cast<MediaReference*>(new OTIO_NS::MediaReference(
        name_str, timeRangeOptional, metadataDictionary));
}
OTIO_API TimeRange* MediaReference_available_range(MediaReference* self)
{
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
        reinterpret_cast<OTIO_NS::MediaReference*>(self)->available_range();
    if(timeRangeOptional == nonstd::nullopt) return NULL;
    return reinterpret_cast<TimeRange*>(
        new opentime::TimeRange(timeRangeOptional.value()));
}
OTIO_API void MediaReference_set_available_range(
    MediaReference* self, TimeRange* available_range)
{
    nonstd::optional<opentime::TimeRange> timeRangeOptional =
        nonstd::nullopt;
    if(available_range != NULL)
    {
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
            *reinterpret_cast<opentime::TimeRange*>(available_range));
    }
    reinterpret_cast<OTIO_NS::MediaReference*>(self)->set_available_range(
        timeRangeOptional);
}
OTIO_API bool MediaReference_is_missing_reference(MediaReference* self)
{
    return reinterpret_cast<OTIO_NS::MediaReference*>(self)
        ->is_missing_reference();
}
OTIO_API const char* MediaReference_name(MediaReference* self)
{
    return SerializableObjectWithMetadata_name(
        (SerializableObjectWithMetadata*) self);
}
OTIO_API void MediaReference_set_name(MediaReference* self, const char* name)
{
    SerializableObjectWithMetadata_set_name(
        (SerializableObjectWithMetadata*) self, name);
}
OTIO_API AnyDictionary* MediaReference_metadata(MediaReference* self)
{
    return SerializableObjectWithMetadata_metadata(
        (SerializableObjectWithMetadata*) self);
}
OTIO_API bool MediaReference_possibly_delete(MediaReference* self)
{
    return SerializableObject_possibly_delete(reinterpret_cast<OTIOSerializableObject*>(self));
}
OTIO_API bool MediaReference_to_json_file(
    MediaReference*  self,
    const char*      file_name,
    OTIOErrorStatus* error_status,
    int              indent)
{
    return SerializableObject_to_json_file(
        reinterpret_cast<OTIOSerializableObject*>(self), file_name, error_status, indent);
}
OTIO_API const char* MediaReference_to_json_string(
    MediaReference* self, OTIOErrorStatus* error_status, int indent)
{
    return SerializableObject_to_json_string(
        reinterpret_cast<OTIOSerializableObject*>(self), error_status, indent);
}
OTIO_API bool MediaReference_is_equivalent_to(
    MediaReference* self, OTIOSerializableObject* other)
{
    return SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(self), other);
}
OTIO_API MediaReference*
MediaReference_clone(MediaReference* self, OTIOErrorStatus* error_status)
{
    return (MediaReference*) SerializableObject_clone(
       reinterpret_cast<OTIOSerializableObject*>(self), error_status);
}
OTIO_API const char* MediaReference_schema_name(MediaReference* self)
{
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject*>(self));
}
OTIO_API int MediaReference_schema_version(MediaReference* self)
{
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject*>(self));
}
