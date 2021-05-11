#include "copentime/util.h"
#include "copentimelineio/marker.h"
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/marker.h>
#include <opentimelineio/serializableObject.h>
#include <opentimelineio/version.h>
#include <string.h>

typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Marker> MarkerRetainer;

const char *MarkerColor_pink = "PINK";
const char *MarkerColor_red = "RED";
const char *MarkerColor_orange = "ORANGE";
const char *MarkerColor_yellow = "YELLOW";
const char *MarkerColor_green = "GREEN";
const char *MarkerColor_cyan = "CYAN";
const char *MarkerColor_blue = "BLUE";
const char *MarkerColor_purple = "PURPLE";
const char *MarkerColor_magenta = "MAGENTA";
const char *MarkerColor_black = "BLACK";
const char *MarkerColor_white = "WHITE";
OTIO_API RetainerMarker *RetainerMarker_create(Marker *obj) {
    return reinterpret_cast<RetainerMarker *>(
            new MarkerRetainer(reinterpret_cast<OTIO_NS::Marker *>(obj)));
}
OTIO_API Marker *RetainerMarker_take_value(RetainerMarker *self) {
    return reinterpret_cast<Marker *>(
            reinterpret_cast<MarkerRetainer *>(self)->take_value());
}
OTIO_API Marker *RetainerMarker_value(RetainerMarker *self) {
    return reinterpret_cast<Marker *>(
            reinterpret_cast<MarkerRetainer *>(self)->value);
}
OTIO_API void RetainerMarker_managed_destroy(RetainerMarker *self) {
    delete reinterpret_cast<MarkerRetainer *>(self);
}

OTIO_API Marker *Marker_create(
        const char *name,
        const char *color,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    opentime::TimeRange marked_range_tr = opentime::TimeRange();

    std::string color_str = OTIO_NS::Marker::Color::green;
    if (color != NULL) color_str = color;

    OTIO_NS::AnyDictionary metdata_dictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metdata_dictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    return reinterpret_cast<Marker *>(new OTIO_NS::Marker(
            name_str, marked_range_tr, color_str, metdata_dictionary));
}
OTIO_API Marker *Marker_create_with_marked_range(
        const char *name,
        TimeRange marked_range,
        const char *color,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    opentime::TimeRange marked_range_tr =
            _COTTimeRange_to_OTTimeRange(marked_range);

    std::string color_str = OTIO_NS::Marker::Color::green;
    if (color != NULL) color_str = color;

    OTIO_NS::AnyDictionary metdata_dictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metdata_dictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    return reinterpret_cast<Marker *>(new OTIO_NS::Marker(
            name_str, marked_range_tr, color_str, metdata_dictionary));
}
OTIO_API const char *Marker_color(Marker *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::Marker *>(self)->color();
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}
OTIO_API void Marker_set_color(Marker *self, const char *color) {
    reinterpret_cast<OTIO_NS::Marker *>(self)->set_color(color);
}
OTIO_API TimeRange Marker_marked_range(Marker *self) {
    opentime::TimeRange timeRange =
            reinterpret_cast<OTIO_NS::Marker *>(self)->marked_range();
    return _OTTimeRange_to_COTTimeRange(timeRange);
}
OTIO_API void Marker_set_marked_range(Marker *self, TimeRange marked_range) {
    reinterpret_cast<OTIO_NS::Marker *>(self)->set_marked_range(
            _COTTimeRange_to_OTTimeRange(marked_range));
}
OTIO_API const char *Marker_name(Marker *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}
OTIO_API void Marker_set_name(Marker *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}

AnyDictionary *Marker_metadata(Marker *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool Marker_to_json_file(
        Marker *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}
OTIO_API const char *Marker_to_json_string(
        Marker *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}
OTIO_API bool Marker_is_equivalent_to(Marker *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}
OTIO_API Marker *Marker_clone(Marker *self, OTIOErrorStatus *error_status) {
    return (Marker *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}
OTIO_API const char *Marker_schema_name(Marker *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}
OTIO_API int Marker_schema_version(Marker *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
