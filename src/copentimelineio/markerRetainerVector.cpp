#include "copentimelineio/markerRetainerVector.h"
#include <opentimelineio/marker.h>
#include <vector>

typedef std::vector<OTIO_NS::Marker::Retainer<OTIO_NS::Marker>>
        MarkerRetainerVectorDef;
typedef std::vector<OTIO_NS::Marker::Retainer<OTIO_NS::Marker>>::iterator
        MarkerRetainerVectorIteratorDef;
typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Marker> MarkerRetainer;

OTIO_API MarkerRetainerVector *MarkerRetainerVector_create() {
    return reinterpret_cast<MarkerRetainerVector *>(
            new MarkerRetainerVectorDef());
}

OTIO_API void MarkerRetainerVector_destroy(MarkerRetainerVector *self) {
    delete reinterpret_cast<MarkerRetainerVectorDef *>(self);
}

OTIO_API MarkerRetainerVectorIterator *
MarkerRetainerVector_begin(MarkerRetainerVector *self) {
    MarkerRetainerVectorIteratorDef iter =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->begin();
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(iter));
}

OTIO_API MarkerRetainerVectorIterator *
MarkerRetainerVector_end(MarkerRetainerVector *self) {
    MarkerRetainerVectorIteratorDef iter =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->end();
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(iter));
}

OTIO_API int MarkerRetainerVector_size(MarkerRetainerVector *self) {
    return reinterpret_cast<MarkerRetainerVectorDef *>(self)->size();
}

OTIO_API int MarkerRetainerVector_max_size(MarkerRetainerVector *self) {
    return reinterpret_cast<MarkerRetainerVectorDef *>(self)->max_size();
}

OTIO_API int MarkerRetainerVector_capacity(MarkerRetainerVector *self) {
    return reinterpret_cast<MarkerRetainerVectorDef *>(self)->capacity();
}

OTIO_API void MarkerRetainerVector_resize(MarkerRetainerVector *self, int n) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->resize(n);
}

OTIO_API bool MarkerRetainerVector_empty(MarkerRetainerVector *self) {
    return reinterpret_cast<MarkerRetainerVectorDef *>(self)->empty();
}

OTIO_API void MarkerRetainerVector_shrink_to_fit(MarkerRetainerVector *self) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->shrink_to_fit();
}

OTIO_API void MarkerRetainerVector_reserve(MarkerRetainerVector *self, int n) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->reserve(n);
}

OTIO_API void MarkerRetainerVector_swap(
        MarkerRetainerVector *self, MarkerRetainerVector *other) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->swap(
            *reinterpret_cast<MarkerRetainerVectorDef *>(other));
}

OTIO_API RetainerMarker *MarkerRetainerVector_at(MarkerRetainerVector *self, int pos) {
    MarkerRetainer obj =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->at(pos);
    return reinterpret_cast<RetainerMarker *>(new MarkerRetainer(obj));
}

OTIO_API void MarkerRetainerVector_push_back(
        MarkerRetainerVector *self, RetainerMarker *value) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->push_back(
            *reinterpret_cast<MarkerRetainer *>(value));
}

OTIO_API void MarkerRetainerVector_pop_back(MarkerRetainerVector *self) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->pop_back();
}

OTIO_API MarkerRetainerVectorIterator *MarkerRetainerVector_insert(
        MarkerRetainerVector *self,
        MarkerRetainerVectorIterator *pos,
        RetainerMarker *val) {
    MarkerRetainerVectorIteratorDef iter =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->insert(
                    *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(pos),
                    *reinterpret_cast<MarkerRetainer *>(val));
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(iter));
}

OTIO_API void MarkerRetainerVector_clear(MarkerRetainerVector *self) {
    reinterpret_cast<MarkerRetainerVectorDef *>(self)->clear();
}

OTIO_API MarkerRetainerVectorIterator *MarkerRetainerVector_erase(
        MarkerRetainerVector *self, MarkerRetainerVectorIterator *pos) {
    MarkerRetainerVectorIteratorDef iter =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->erase(
                    *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(pos));
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(iter));
}

OTIO_API MarkerRetainerVectorIterator *MarkerRetainerVector_erase_range(
        MarkerRetainerVector *self,
        MarkerRetainerVectorIterator *first,
        MarkerRetainerVectorIterator *last) {
    MarkerRetainerVectorIteratorDef iter =
            reinterpret_cast<MarkerRetainerVectorDef *>(self)->erase(
                    *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(first),
                    *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(last));
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(iter));
}

OTIO_API void MarkerRetainerVectorIterator_advance(
        MarkerRetainerVectorIterator *iter, int dist) {
    std::advance(
            *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(iter), dist);
}

OTIO_API MarkerRetainerVectorIterator *MarkerRetainerVectorIterator_next(
        MarkerRetainerVectorIterator *iter, int dist) {
    MarkerRetainerVectorIteratorDef it = std::next(
            *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(it));
}

OTIO_API MarkerRetainerVectorIterator *MarkerRetainerVectorIterator_prev(
        MarkerRetainerVectorIterator *iter, int dist) {
    MarkerRetainerVectorIteratorDef it = std::prev(
            *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<MarkerRetainerVectorIterator *>(
            new MarkerRetainerVectorIteratorDef(it));
}

OTIO_API RetainerMarker *
MarkerRetainerVectorIterator_value(MarkerRetainerVectorIterator *iter) {
    MarkerRetainer obj =
            **reinterpret_cast<MarkerRetainerVectorIteratorDef *>(iter);
    return reinterpret_cast<RetainerMarker *>(new MarkerRetainer(obj));
}

OTIO_API bool MarkerRetainerVectorIterator_equal(
        MarkerRetainerVectorIterator *lhs, MarkerRetainerVectorIterator *rhs) {
    return *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(lhs) ==
           *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(rhs);
}

OTIO_API bool MarkerRetainerVectorIterator_not_equal(
        MarkerRetainerVectorIterator *lhs, MarkerRetainerVectorIterator *rhs) {
    return *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(lhs) !=
           *reinterpret_cast<MarkerRetainerVectorIteratorDef *>(rhs);
}

OTIO_API void
MarkerRetainerVectorIterator_destroy(MarkerRetainerVectorIterator *self) {
    delete reinterpret_cast<MarkerRetainerVectorIteratorDef *>(self);
}
