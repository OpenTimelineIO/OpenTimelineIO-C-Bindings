#include "copentime/util.h"
#include "copentimelineio/timeline.h"
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/stack.h>
#include <opentimelineio/timeline.h>
#include <opentimelineio/track.h>
#include <vector>

typedef std::vector<OTIO_NS::Track *> TrackVectorDef;
typedef std::vector<OTIO_NS::Track *>::iterator TrackVectorIteratorDef;

OTIO_API Timeline *Timeline_create(
        const char *name,
        OptionalRationalTime global_start_time,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    nonstd::optional<opentime::RationalTime> rationalTimeOptional = nonstd::nullopt;
    if (global_start_time.valid)
        rationalTimeOptional = nonstd::optional<opentime::RationalTime>(
                _COTRationalTime_to_OTRationalTime(global_start_time.value));
    return reinterpret_cast<Timeline *>(new OTIO_NS::Timeline(
            name_str, rationalTimeOptional, metadataDictionary));
}

OTIO_API Stack *Timeline_tracks(Timeline *self) {
    return reinterpret_cast<Stack *>(
            reinterpret_cast<OTIO_NS::Timeline *>(self)->tracks());
}

OTIO_API void Timeline_set_tracks(Timeline *self, Stack *stack) {
    reinterpret_cast<OTIO_NS::Timeline *>(self)->set_tracks(
            reinterpret_cast<OTIO_NS::Stack *>(stack));
}

OTIO_API OptionalRationalTime Timeline_global_start_time(Timeline *self) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional =
            reinterpret_cast<OTIO_NS::Timeline *>(self)->global_start_time();
    if (rationalTimeOptional == nonstd::nullopt) return OptionalRationalTime_create_null();
    return OptionalRationalTime_create(_OTRationalTime_to_COTRationalTime(rationalTimeOptional.value()));
}

OTIO_API void Timeline_set_global_start_time(
        Timeline *self, OptionalRationalTime global_start_time) {
    nonstd::optional<opentime::RationalTime> rationalTimeOptional = nonstd::nullopt;
    if (global_start_time.valid)
        rationalTimeOptional = nonstd::optional<opentime::RationalTime>(
                _COTRationalTime_to_OTRationalTime(global_start_time.value));
    reinterpret_cast<OTIO_NS::Timeline *>(self)->set_global_start_time(
            rationalTimeOptional);
}

OTIO_API RationalTime
Timeline_duration(Timeline *self, OTIOErrorStatus *error_status) {
    opentime::RationalTime rationalTime =
            reinterpret_cast<OTIO_NS::Timeline *>(self)->duration(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return _OTRationalTime_to_COTRationalTime(rationalTime);
}

OTIO_API TimeRange Timeline_range_of_child(
        Timeline *self, Composable *child, OTIOErrorStatus *error_status) {
    opentime::TimeRange timeRange =
            reinterpret_cast<OTIO_NS::Timeline *>(self)->range_of_child(
                    reinterpret_cast<OTIO_NS::Composable *>(child),
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return _OTTimeRange_to_COTTimeRange(timeRange);
}

OTIO_API TrackVector *Timeline_audio_tracks(Timeline *self) {
    TrackVectorDef trackVector =
            reinterpret_cast<OTIO_NS::Timeline *>(self)->audio_tracks();
    return reinterpret_cast<TrackVector *>(new TrackVectorDef(trackVector));
}

OTIO_API TrackVector *Timeline_video_tracks(Timeline *self) {
    TrackVectorDef trackVector =
            reinterpret_cast<OTIO_NS::Timeline *>(self)->video_tracks();
    return reinterpret_cast<TrackVector *>(new TrackVectorDef(trackVector));
}

OTIO_API const char *Timeline_name(Timeline *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API void Timeline_set_name(Timeline *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}

OTIO_API AnyDictionary *Timeline_metadata(Timeline *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool Timeline_to_json_file(
        Timeline *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}

OTIO_API const char *Timeline_to_json_string(
        Timeline *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}

OTIO_API bool Timeline_is_equivalent_to(Timeline *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}

OTIO_API Timeline *Timeline_clone(Timeline *self, OTIOErrorStatus *error_status) {
    return (Timeline *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}

OTIO_API const char *Timeline_schema_name(Timeline *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}

OTIO_API int Timeline_schema_version(Timeline *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}