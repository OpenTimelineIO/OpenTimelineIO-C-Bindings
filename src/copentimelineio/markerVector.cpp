#include "copentimelineio/markerVector.h"
#include <opentimelineio/marker.h>
#include <vector>

typedef std::vector<OTIO_NS::Marker *> MarkerVectorDef;
typedef std::vector<OTIO_NS::Marker *>::iterator MarkerVectorIteratorDef;

OTIO_API MarkerVector *MarkerVector_create() {
    return reinterpret_cast<MarkerVector *>(new MarkerVectorDef());
}

OTIO_API void MarkerVector_destroy(MarkerVector *self) {
    delete reinterpret_cast<MarkerVectorDef *>(self);
}

OTIO_API MarkerVectorIterator *MarkerVector_begin(MarkerVector *self) {
    MarkerVectorIteratorDef iter =
            reinterpret_cast<MarkerVectorDef *>(self)->begin();
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(iter));
}

OTIO_API MarkerVectorIterator *MarkerVector_end(MarkerVector *self) {
    MarkerVectorIteratorDef iter =
            reinterpret_cast<MarkerVectorDef *>(self)->end();
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(iter));
}

OTIO_API int MarkerVector_size(MarkerVector *self) {
    return reinterpret_cast<MarkerVectorDef *>(self)->size();
}

OTIO_API int MarkerVector_max_size(MarkerVector *self) {
    return reinterpret_cast<MarkerVectorDef *>(self)->max_size();
}

OTIO_API int MarkerVector_capacity(MarkerVector *self) {
    return reinterpret_cast<MarkerVectorDef *>(self)->capacity();
}

OTIO_API void MarkerVector_resize(MarkerVector *self, int n) {
    reinterpret_cast<MarkerVectorDef *>(self)->resize(n);
}

OTIO_API bool MarkerVector_empty(MarkerVector *self) {
    return reinterpret_cast<MarkerVectorDef *>(self)->empty();
}

OTIO_API void MarkerVector_shrink_to_fit(MarkerVector *self) {
    reinterpret_cast<MarkerVectorDef *>(self)->shrink_to_fit();
}

OTIO_API void MarkerVector_reserve(MarkerVector *self, int n) {
    reinterpret_cast<MarkerVectorDef *>(self)->reserve(n);
}

OTIO_API void MarkerVector_swap(MarkerVector *self, MarkerVector *other) {
    reinterpret_cast<MarkerVectorDef *>(self)->swap(
            *reinterpret_cast<MarkerVectorDef *>(other));
}

OTIO_API Marker *MarkerVector_at(MarkerVector *self, int pos) {
    return reinterpret_cast<Marker *>(
            reinterpret_cast<MarkerVectorDef *>(self)->at(pos));
}

OTIO_API void MarkerVector_push_back(MarkerVector *self, Marker *value) {
    reinterpret_cast<MarkerVectorDef *>(self)->push_back(
            reinterpret_cast<OTIO_NS::Marker *>(value));
}

OTIO_API void MarkerVector_pop_back(MarkerVector *self) {
    reinterpret_cast<MarkerVectorDef *>(self)->pop_back();
}

OTIO_API MarkerVectorIterator *MarkerVector_insert(
        MarkerVector *self, MarkerVectorIterator *pos, Marker *val) {
    MarkerVectorIteratorDef iter =
            reinterpret_cast<MarkerVectorDef *>(self)->insert(
                    *reinterpret_cast<MarkerVectorIteratorDef *>(pos),
                    reinterpret_cast<OTIO_NS::Marker *>(val));
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(iter));
}

OTIO_API void MarkerVector_clear(MarkerVector *self) {
    reinterpret_cast<MarkerVectorDef *>(self)->clear();
}

OTIO_API MarkerVectorIterator *
MarkerVector_erase(MarkerVector *self, MarkerVectorIterator *pos) {
    MarkerVectorIteratorDef iter =
            reinterpret_cast<MarkerVectorDef *>(self)->erase(
                    *reinterpret_cast<MarkerVectorIteratorDef *>(pos));
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(iter));
}

OTIO_API MarkerVectorIterator *MarkerVector_erase_range(
        MarkerVector *self,
        MarkerVectorIterator *first,
        MarkerVectorIterator *last) {
    MarkerVectorIteratorDef iter =
            reinterpret_cast<MarkerVectorDef *>(self)->erase(
                    *reinterpret_cast<MarkerVectorIteratorDef *>(first),
                    *reinterpret_cast<MarkerVectorIteratorDef *>(last));
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(iter));
}

OTIO_API void MarkerVectorIterator_advance(MarkerVectorIterator *iter, int dist) {
    std::advance(*reinterpret_cast<MarkerVectorIteratorDef *>(iter), dist);
}

OTIO_API MarkerVectorIterator *
MarkerVectorIterator_next(MarkerVectorIterator *iter, int dist) {
    MarkerVectorIteratorDef it =
            std::next(*reinterpret_cast<MarkerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(it));
}

OTIO_API MarkerVectorIterator *
MarkerVectorIterator_prev(MarkerVectorIterator *iter, int dist) {
    MarkerVectorIteratorDef it =
            std::prev(*reinterpret_cast<MarkerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<MarkerVectorIterator *>(
            new MarkerVectorIteratorDef(it));
}

OTIO_API Marker *MarkerVectorIterator_value(MarkerVectorIterator *iter) {
    OTIO_NS::Marker *obj =
            **reinterpret_cast<MarkerVectorIteratorDef *>(iter);
    return reinterpret_cast<Marker *>(obj);
}

OTIO_API bool MarkerVectorIterator_equal(
        MarkerVectorIterator *lhs, MarkerVectorIterator *rhs) {
    return *reinterpret_cast<MarkerVectorIteratorDef *>(lhs) ==
           *reinterpret_cast<MarkerVectorIteratorDef *>(rhs);
}

OTIO_API bool MarkerVectorIterator_not_equal(
        MarkerVectorIterator *lhs, MarkerVectorIterator *rhs) {
    return *reinterpret_cast<MarkerVectorIteratorDef *>(lhs) !=
           *reinterpret_cast<MarkerVectorIteratorDef *>(rhs);
}

OTIO_API void MarkerVectorIterator_destroy(MarkerVectorIterator *self) {
    delete reinterpret_cast<MarkerVectorIteratorDef *>(self);
}