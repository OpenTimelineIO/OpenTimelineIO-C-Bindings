#include "copentime/util.h"
#include "copentimelineio/track.h"
#include <copentimelineio/composition.h>
#include <opentime/rationalTime.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/composable.h>
#include <opentimelineio/errorStatus.h>
#include <opentimelineio/track.h>
#include <optional>
#include <string.h>
#include <utility>

typedef std::map<OTIO_NS::Composable *, opentime::TimeRange>::iterator
        MapComposableTimeRangeIteratorDef;
typedef std::map<OTIO_NS::Composable *, opentime::TimeRange>
        MapComposableTimeRangeDef;

typedef std::pair<
        nonstd::optional<opentime::RationalTime>,
        nonstd::optional<opentime::RationalTime>>
        OptionalPairRationalTimeDef;
typedef std::pair<
        OTIO_NS::Composable::Retainer<OTIO_NS::Composable>,
        OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>
        RetainerPairComposableDef;

typedef std::vector<OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>
        ComposableRetainerVectorDef;
typedef std::vector<OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>::
iterator ComposableRetainerVectorIteratorDef;
typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Composable>
        ComposableRetainer;

typedef std::vector<OTIO_NS::Composable *> ComposableVectorDef;
typedef std::vector<OTIO_NS::Composable *>::iterator ComposableVectorIteratorDef;


OTIO_API const char *TrackKind_Video = OTIO_NS::Track::Kind::video;
OTIO_API const char *TrackKind_Audio = OTIO_NS::Track::Kind::audio;

OTIO_API Track *Track_create(
        const char *name,
        OptionalTimeRange source_range,
        const char *kind,
        AnyDictionary *metadata) {
    nonstd::optional<opentime::TimeRange> timeRangeOptional = nonstd::nullopt;
    if (source_range.valid)
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
                CTimeRange_to_CppTimeRange(source_range.value));

    std::string name_str = std::string();
    if (name != NULL) name_str = name;

    std::string kind_str = TrackKind_Video;
    if (kind != NULL) kind_str = kind;

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL) {
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);
    }

    return reinterpret_cast<Track *>(new OTIO_NS::Track(
            name_str, timeRangeOptional, kind_str, metadataDictionary));
}
OTIO_API otiostr Track_kind(Track *self) {
    std::string returnStr = reinterpret_cast<OTIO_NS::Track *>(self)->kind();
    otiostr trackKindStr = otiostr_create(returnStr.c_str());
    return trackKindStr;
}
OTIO_API void Track_set_kind(Track *self, const char *kind) {
    reinterpret_cast<OTIO_NS::Track *>(self)->set_kind(kind);
}
OTIO_API TimeRange Track_range_of_child_at_index(
        Track *self, int index, OTIOErrorStatus *error_status) {
    opentime::TimeRange timeRange =
            reinterpret_cast<OTIO_NS::Track *>(self)->range_of_child_at_index(
                    index, reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return CppTimeRange_to_CTimeRange(timeRange);
}
OTIO_API TimeRange Track_trimmed_range_of_child_at_index(
        Track *self, int index, OTIOErrorStatus *error_status) {
    opentime::TimeRange timeRange =
            reinterpret_cast<OTIO_NS::Track *>(self)
                    ->trimmed_range_of_child_at_index(
                            index,
                            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return CppTimeRange_to_CTimeRange(timeRange);
}
OTIO_API TimeRange Track_available_range(Track *self, OTIOErrorStatus *error_status) {
    opentime::TimeRange timeRange =
            reinterpret_cast<OTIO_NS::Track *>(self)->available_range(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return CppTimeRange_to_CTimeRange(timeRange);
}
OTIO_API OptionalPairRationalTime *Track_handles_of_child(
        Track *self, Composable *child, OTIOErrorStatus *error_status) {
    OptionalPairRationalTimeDef optionalPairRationalTime =
            reinterpret_cast<OTIO_NS::Track *>(self)->handles_of_child(
                    reinterpret_cast<OTIO_NS::Composable *>(child),
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return reinterpret_cast<OptionalPairRationalTime *>(
            new OptionalPairRationalTimeDef(optionalPairRationalTime));
}
OTIO_API RetainerPairComposable *Track_neighbors_of(
        Track *self,
        Composable *item,
        OTIOErrorStatus *error_status,
        OTIO_Track_NeighbourGapPolicy insert_gap) {
    RetainerPairComposableDef retainerPairComposable =
            reinterpret_cast<OTIO_NS::Track *>(self)->neighbors_of(
                    reinterpret_cast<OTIO_NS::Composable *>(item),
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status),
                    static_cast<OTIO_NS::Track::NeighborGapPolicy>(insert_gap));
    return reinterpret_cast<RetainerPairComposable *>(
            new RetainerPairComposableDef(retainerPairComposable));
}
OTIO_API ComposableVector *Track_each_clip(Track *self) {
    ComposableRetainerVectorDef composableRetainerVector =
            reinterpret_cast<OTIO_NS::Track *>(self)->children();
    auto *composableVector = new ComposableVectorDef();

    for (int i = 0; i < composableRetainerVector.size(); i++) {
        if (composableRetainerVector[i].value->schema_name() == "Clip") {
            composableVector->push_back(composableRetainerVector[i].value);
        }
    }
    return reinterpret_cast<ComposableVector *>(composableVector);
}
OTIO_API MapComposableTimeRange *
Track_range_of_all_children(Track *self, OTIOErrorStatus *error_status) {
    MapComposableTimeRangeDef mapComposableTimeRange =
            reinterpret_cast<OTIO_NS::Track *>(self)->range_of_all_children(
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
    return reinterpret_cast<MapComposableTimeRange *>(
            new MapComposableTimeRangeDef(mapComposableTimeRange));
}
OTIO_API otiostr Track_composition_kind(Track *self) {
    return Composition_composition_kind((Composition *) self);
}
OTIO_API ComposableRetainerVector *Track_children(Track *self) {
    return Composition_children((Composition *) self);
}
OTIO_API void Track_clear_children(Track *self) {
    Composition_clear_children((Composition *) self);
}
OTIO_API bool Track_set_children(
        Track *self, ComposableVector *children, OTIOErrorStatus *error_status) {
    return Composition_set_children(
            (Composition *) self, children, error_status);
}
OTIO_API bool Track_insert_child(
        Track *self,
        int index,
        Composable *child,
        OTIOErrorStatus *error_status) {
    return Composition_insert_child(
            (Composition *) self, index, child, error_status);
}
OTIO_API bool Track_set_child(
        Track *self,
        int index,
        Composable *child,
        OTIOErrorStatus *error_status) {
    return Composition_set_child(
            (Composition *) self, index, child, error_status);
}
OTIO_API bool
Track_remove_child(Track *self, int index, OTIOErrorStatus *error_status) {
    return Composition_remove_child(
            (Composition *) self, index, error_status);
}
OTIO_API bool Track_append_child(
        Track *self, Composable *child, OTIOErrorStatus *error_status) {
    return Composition_append_child(
            (Composition *) self, child, error_status);
}
OTIO_API bool Track_is_parent_of(Track *self, Composable *other) {
    return Composition_is_parent_of((Composition *) self, other);
}
OTIO_API TimeRange Track_range_of_child(
        Track *self, Composable *child, OTIOErrorStatus *error_status) {
    return Composition_range_of_child(
            (Composition *) self, child, error_status);
}
OTIO_API OptionalTimeRange Track_trimmed_range_of_child(
        Track *self, Composable *child, OTIOErrorStatus *error_status) {
    return Composition_trimmed_range_of_child(
            (Composition *) self, child, error_status);
}
OTIO_API OptionalTimeRange Track_trim_child_range(Track *self, TimeRange child_range) {
    return Composition_trim_child_range((Composition *) self, child_range);
}
OTIO_API bool Track_has_child(Track *self, Composable *child) {
    return Composition_has_child((Composition *) self, child);
}
OTIO_API bool Track_visible(Track *self) {
    return Composition_visible((Composition *) self);
}
OTIO_API bool Track_overlapping(Track *self) {
    return Composition_overlapping((Composition *) self);
}
OTIO_API OptionalTimeRange Track_source_range(Track *self) {
    return Composition_source_range((Composition *) self);
}
OTIO_API void Track_set_source_range(Track *self, OptionalTimeRange source_range) {
    Composition_set_source_range((Composition *) self, source_range);
}
OTIO_API EffectRetainerVector *Track_effects(Track *self) {
    return Composition_effects((Composition *) self);
}
OTIO_API MarkerRetainerVector *Track_markers(Track *self) {
    return Composition_markers((Composition *) self);
}
OTIO_API RationalTime Track_duration(Track *self, OTIOErrorStatus *error_status) {
    return Composition_duration((Composition *) self, error_status);
}
OTIO_API TimeRange Track_trimmed_range(Track *self, OTIOErrorStatus *error_status) {
    return Composition_trimmed_range((Composition *) self, error_status);
}
OTIO_API TimeRange Track_visible_range(Track *self, OTIOErrorStatus *error_status) {
    return Composition_visible_range((Composition *) self, error_status);
}
OTIO_API OptionalTimeRange
Track_trimmed_range_in_parent(Track *self, OTIOErrorStatus *error_status) {
    return Composition_trimmed_range_in_parent(
            (Composition *) self, error_status);
}
OTIO_API TimeRange Track_range_in_parent(Track *self, OTIOErrorStatus *error_status) {
    return Composition_range_in_parent((Composition *) self, error_status);
}
OTIO_API RationalTime Track_transformed_time(
        Track *self,
        RationalTime time,
        Item *to_item,
        OTIOErrorStatus *error_status) {
    return Composition_transformed_time(
            (Composition *) self, time, to_item, error_status);
}
OTIO_API TimeRange Track_transformed_time_range(
        Track *self,
        TimeRange time_range,
        Item *to_item,
        OTIOErrorStatus *error_status) {
    return Composition_transformed_time_range(
            (Composition *) self, time_range, to_item, error_status);
}
OTIO_API Composition *Track_parent(Track *self) {
    return Composition_parent((Composition *) self);
}
OTIO_API otiostr Track_name(Track *self) {
    return Composition_name((Composition *) self);
}
OTIO_API AnyDictionary *Track_metadata(Track *self) {
    return Composition_metadata((Composition *) self);
}
OTIO_API void Track_set_name(Track *self, const char *name) {
    return Composition_set_name((Composition *) self, name);
}
OTIO_API bool Track_to_json_file(
        Track *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return Composition_to_json_file(
            (Composition *) self, file_name, error_status, indent);
}
OTIO_API otiostr
Track_to_json_string(Track *self, OTIOErrorStatus *error_status, int indent) {
    return Composition_to_json_string(
            (Composition *) self, error_status, indent);
}
OTIO_API bool Track_is_equivalent_to(Track *self, OTIOSerializableObject *other) {
    return Composition_is_equivalent_to((Composition *) self, other);
}
OTIO_API Track *Track_clone(Track *self, OTIOErrorStatus *error_status) {
    return (Track *) Composition_clone((Composition *) self, error_status);
}
OTIO_API otiostr Track_schema_name(Track *self) {
    return Composition_schema_name((Composition *) self);
}
OTIO_API int Track_schema_version(Track *self) {
    return Composition_schema_version((Composition *) self);
}
