#include "copentimelineio/effectRetainerVector.h"
#include <opentimelineio/effect.h>
#include <vector>

typedef std::vector<OTIO_NS::Effect::Retainer<OTIO_NS::Effect>>
        EffectRetainerVectorDef;
typedef std::vector<OTIO_NS::Effect::Retainer<OTIO_NS::Effect>>::iterator
        EffectRetainerVectorIteratorDef;
typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Effect> EffectRetainer;

OTIO_API EffectRetainerVector *EffectRetainerVector_create() {
    return reinterpret_cast<EffectRetainerVector *>(
            new EffectRetainerVectorDef());
}

OTIO_API void EffectRetainerVector_destroy(EffectRetainerVector *self) {
    delete reinterpret_cast<EffectRetainerVectorDef *>(self);
}

OTIO_API EffectRetainerVectorIterator *
EffectRetainerVector_begin(EffectRetainerVector *self) {
    EffectRetainerVectorIteratorDef iter =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->begin();
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(iter));
}

OTIO_API EffectRetainerVectorIterator *
EffectRetainerVector_end(EffectRetainerVector *self) {
    EffectRetainerVectorIteratorDef iter =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->end();
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(iter));
}

OTIO_API int EffectRetainerVector_size(EffectRetainerVector *self) {
    return reinterpret_cast<EffectRetainerVectorDef *>(self)->size();
}

OTIO_API int EffectRetainerVector_max_size(EffectRetainerVector *self) {
    return reinterpret_cast<EffectRetainerVectorDef *>(self)->max_size();
}

OTIO_API int EffectRetainerVector_capacity(EffectRetainerVector *self) {
    return reinterpret_cast<EffectRetainerVectorDef *>(self)->capacity();
}

OTIO_API void EffectRetainerVector_resize(EffectRetainerVector *self, int n) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->resize(n);
}

OTIO_API bool EffectRetainerVector_empty(EffectRetainerVector *self) {
    return reinterpret_cast<EffectRetainerVectorDef *>(self)->empty();
}

OTIO_API void EffectRetainerVector_shrink_to_fit(EffectRetainerVector *self) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->shrink_to_fit();
}

OTIO_API void EffectRetainerVector_reserve(EffectRetainerVector *self, int n) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->reserve(n);
}

OTIO_API void EffectRetainerVector_swap(
        EffectRetainerVector *self, EffectRetainerVector *other) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->swap(
            *reinterpret_cast<EffectRetainerVectorDef *>(other));
}

OTIO_API RetainerEffect *EffectRetainerVector_at(EffectRetainerVector *self, int pos) {
    EffectRetainer obj =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->at(pos);
    return reinterpret_cast<RetainerEffect *>(new EffectRetainer(obj));
}

OTIO_API void EffectRetainerVector_push_back(
        EffectRetainerVector *self, RetainerEffect *value) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->push_back(
            *reinterpret_cast<EffectRetainer *>(value));
}

OTIO_API void EffectRetainerVector_pop_back(EffectRetainerVector *self) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->pop_back();
}

OTIO_API EffectRetainerVectorIterator *EffectRetainerVector_insert(
        EffectRetainerVector *self,
        EffectRetainerVectorIterator *pos,
        RetainerEffect *val) {
    EffectRetainerVectorIteratorDef iter =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->insert(
                    *reinterpret_cast<EffectRetainerVectorIteratorDef *>(pos),
                    *reinterpret_cast<EffectRetainer *>(val));
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(iter));
}

OTIO_API void EffectRetainerVector_clear(EffectRetainerVector *self) {
    reinterpret_cast<EffectRetainerVectorDef *>(self)->clear();
}

OTIO_API EffectRetainerVectorIterator *EffectRetainerVector_erase(
        EffectRetainerVector *self, EffectRetainerVectorIterator *pos) {
    EffectRetainerVectorIteratorDef iter =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->erase(
                    *reinterpret_cast<EffectRetainerVectorIteratorDef *>(pos));
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(iter));
}

OTIO_API EffectRetainerVectorIterator *EffectRetainerVector_erase_range(
        EffectRetainerVector *self,
        EffectRetainerVectorIterator *first,
        EffectRetainerVectorIterator *last) {
    EffectRetainerVectorIteratorDef iter =
            reinterpret_cast<EffectRetainerVectorDef *>(self)->erase(
                    *reinterpret_cast<EffectRetainerVectorIteratorDef *>(first),
                    *reinterpret_cast<EffectRetainerVectorIteratorDef *>(last));
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(iter));
}

OTIO_API void EffectRetainerVectorIterator_advance(
        EffectRetainerVectorIterator *iter, int dist) {
    std::advance(
            *reinterpret_cast<EffectRetainerVectorIteratorDef *>(iter), dist);
}

OTIO_API EffectRetainerVectorIterator *EffectRetainerVectorIterator_next(
        EffectRetainerVectorIterator *iter, int dist) {
    EffectRetainerVectorIteratorDef it = std::next(
            *reinterpret_cast<EffectRetainerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(it));
}

OTIO_API EffectRetainerVectorIterator *EffectRetainerVectorIterator_prev(
        EffectRetainerVectorIterator *iter, int dist) {
    EffectRetainerVectorIteratorDef it = std::prev(
            *reinterpret_cast<EffectRetainerVectorIteratorDef *>(iter), dist);
    return reinterpret_cast<EffectRetainerVectorIterator *>(
            new EffectRetainerVectorIteratorDef(it));
}

OTIO_API RetainerEffect *
EffectRetainerVectorIterator_value(EffectRetainerVectorIterator *iter) {
    EffectRetainer obj =
            **reinterpret_cast<EffectRetainerVectorIteratorDef *>(iter);
    return reinterpret_cast<RetainerEffect *>(new EffectRetainer(obj));
}

OTIO_API bool EffectRetainerVectorIterator_equal(
        EffectRetainerVectorIterator *lhs, EffectRetainerVectorIterator *rhs) {
    return *reinterpret_cast<EffectRetainerVectorIteratorDef *>(lhs) ==
           *reinterpret_cast<EffectRetainerVectorIteratorDef *>(rhs);
}

OTIO_API bool EffectRetainerVectorIterator_not_equal(
        EffectRetainerVectorIterator *lhs, EffectRetainerVectorIterator *rhs) {
    return *reinterpret_cast<EffectRetainerVectorIteratorDef *>(lhs) !=
           *reinterpret_cast<EffectRetainerVectorIteratorDef *>(rhs);
}

OTIO_API void
EffectRetainerVectorIterator_destroy(EffectRetainerVectorIterator *self) {
    delete reinterpret_cast<EffectRetainerVectorIteratorDef *>(self);
}
