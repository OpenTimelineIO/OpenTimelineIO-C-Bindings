#include "copentime/util.h"
#include "copentimelineio/mapComposableTimeRange.h"
#include <map>
#include <opentime/timeRange.h>
#include <opentimelineio/composable.h>
#include <opentimelineio/version.h>

typedef std::map<OTIO_NS::Composable *, opentime::TimeRange>::iterator
        MapIterator;
typedef std::map<OTIO_NS::Composable *, opentime::TimeRange> MapDef;

OTIO_API MapComposableTimeRange *MapComposableTimeRange_create() {
    return reinterpret_cast<MapComposableTimeRange *>(new MapDef());
}

OTIO_API void MapComposableTimeRange_destroy(MapComposableTimeRange *self) {
    delete reinterpret_cast<MapDef *>(self);
}

OTIO_API void MapComposableTimeRange_clear(MapComposableTimeRange *self) {
    reinterpret_cast<MapDef *>(self)->clear();
}

OTIO_API MapComposableTimeRangeIterator *
MapComposableTimeRange_begin(MapComposableTimeRange *self) {
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(reinterpret_cast<MapDef *>(self)->begin()));
}

OTIO_API MapComposableTimeRangeIterator *
MapComposableTimeRange_end(MapComposableTimeRange *self) {
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(reinterpret_cast<MapDef *>(self)->end()));
}

OTIO_API void MapComposableTimeRange_swap(
        MapComposableTimeRange *self, MapComposableTimeRange *other) {
    reinterpret_cast<MapDef *>(self)->swap(
            *reinterpret_cast<MapDef *>(other));
}

OTIO_API MapComposableTimeRangeIterator *MapComposableTimeRange_erase(
        MapComposableTimeRange *self, MapComposableTimeRangeIterator *pos) {
    MapIterator it = reinterpret_cast<MapDef *>(self)->erase(
            *reinterpret_cast<MapIterator *>(pos));
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(it));
}

OTIO_API MapComposableTimeRangeIterator *MapComposableTimeRange_erase_range(
        MapComposableTimeRange *self,
        MapComposableTimeRangeIterator *first,
        MapComposableTimeRangeIterator *last) {
    MapIterator it = reinterpret_cast<MapDef *>(self)->erase(
            *reinterpret_cast<MapIterator *>(first),
            *reinterpret_cast<MapIterator *>(last));
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(it));
}

OTIO_API int MapComposableTimeRange_erase_key(
        MapComposableTimeRange *self, Composable *key) {
    return reinterpret_cast<MapDef *>(self)->erase(
            reinterpret_cast<OTIO_NS::Composable *>(key));
}

OTIO_API int MapComposableTimeRange_size(MapComposableTimeRange *self) {
    return reinterpret_cast<MapDef *>(self)->size();
}

OTIO_API int MapComposableTimeRange_max_size(MapComposableTimeRange *self) {
    return reinterpret_cast<MapDef *>(self)->max_size();
}

OTIO_API bool MapComposableTimeRange_empty(MapComposableTimeRange *self) {
    return reinterpret_cast<MapDef *>(self)->empty();
}

OTIO_API MapComposableTimeRangeIterator *
MapComposableTimeRange_find(MapComposableTimeRange *self, Composable *key) {
    MapIterator iter = reinterpret_cast<MapDef *>(self)->find(
            reinterpret_cast<OTIO_NS::Composable *>(key));
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(iter));
}

OTIO_API MapComposableTimeRangeIterator *MapComposableTimeRange_insert(
        MapComposableTimeRange *self, Composable *key, TimeRange anyObj) {
    MapIterator it =
            reinterpret_cast<MapDef *>(self)
                    ->insert({reinterpret_cast<OTIO_NS::Composable *>(key),
                              CTimeRange_to_CppTimeRange(anyObj)})
                    .first;
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(it));
}

OTIO_API void MapComposableTimeRangeIterator_advance(
        MapComposableTimeRangeIterator *iter, int dist) {
    std::advance(*reinterpret_cast<MapIterator *>(iter), dist);
}

OTIO_API MapComposableTimeRangeIterator *MapComposableTimeRangeIterator_next(
        MapComposableTimeRangeIterator *iter, int dist) {
    MapIterator it = std::next(*reinterpret_cast<MapIterator *>(iter), dist);
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(it));
}

OTIO_API MapComposableTimeRangeIterator *MapComposableTimeRangeIterator_prev(
        MapComposableTimeRangeIterator *iter, int dist) {
    MapIterator it = std::prev(*reinterpret_cast<MapIterator *>(iter), dist);
    return reinterpret_cast<MapComposableTimeRangeIterator *>(
            new MapIterator(it));
}

OTIO_API TimeRange
MapComposableTimeRangeIterator_value(MapComposableTimeRangeIterator *iter) {
    opentime::TimeRange timeRange =
            (*reinterpret_cast<MapIterator *>(iter))->second;
    return CppTimeRange_to_CTimeRange(timeRange);
}

OTIO_API bool MapComposableTimeRangeIterator_equal(
        MapComposableTimeRangeIterator *lhs,
        MapComposableTimeRangeIterator *rhs) {
    return *reinterpret_cast<MapIterator *>(lhs) ==
           *reinterpret_cast<MapIterator *>(rhs);
}

OTIO_API bool MapComposableTimeRangeIterator_not_equal(
        MapComposableTimeRangeIterator *lhs,
        MapComposableTimeRangeIterator *rhs) {
    return *reinterpret_cast<MapIterator *>(lhs) !=
           *reinterpret_cast<MapIterator *>(rhs);
}

OTIO_API void
MapComposableTimeRangeIterator_destroy(MapComposableTimeRangeIterator *self) {
    delete reinterpret_cast<MapIterator *>(self);
}
