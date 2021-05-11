#include "copentimelineio/serializableObjectVector.h"
#include <opentimelineio/serializableObject.h>
#include <vector>

typedef std::vector<OTIO_NS::SerializableObject *> SerializableObjectVectorDef;
typedef std::vector<OTIO_NS::SerializableObject *>::iterator
        SerializableObjectVectorIteratorDef;

OTIO_API SerializableObjectVector *SerializableObjectVector_create() {
    return reinterpret_cast<SerializableObjectVector *>(
            new SerializableObjectVectorDef());
}
OTIO_API void SerializableObjectVector_destroy(SerializableObjectVector *self) {
    delete reinterpret_cast<SerializableObjectVectorDef *>(self);
}
OTIO_API SerializableObjectVectorIterator *
SerializableObjectVector_begin(SerializableObjectVector *self) {
    SerializableObjectVectorIteratorDef iter =
            reinterpret_cast<SerializableObjectVectorDef *>(self)->begin();
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(iter));
}
OTIO_API SerializableObjectVectorIterator *
SerializableObjectVector_end(SerializableObjectVector *self) {
    SerializableObjectVectorIteratorDef iter =
            reinterpret_cast<SerializableObjectVectorDef *>(self)->end();
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(iter));
}
OTIO_API int SerializableObjectVector_size(SerializableObjectVector *self) {
    return reinterpret_cast<SerializableObjectVectorDef *>(self)->size();
}
OTIO_API int SerializableObjectVector_max_size(SerializableObjectVector *self) {
    return reinterpret_cast<SerializableObjectVectorDef *>(self)->max_size();
}
OTIO_API int SerializableObjectVector_capacity(SerializableObjectVector *self) {
    return reinterpret_cast<SerializableObjectVectorDef *>(self)->capacity();
}
OTIO_API void SerializableObjectVector_resize(SerializableObjectVector *self, int n) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->resize(n);
}
OTIO_API bool SerializableObjectVector_empty(SerializableObjectVector *self) {
    return reinterpret_cast<SerializableObjectVectorDef *>(self)->empty();
}
OTIO_API void SerializableObjectVector_shrink_to_fit(SerializableObjectVector *self) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->shrink_to_fit();
}
OTIO_API void SerializableObjectVector_reserve(SerializableObjectVector *self, int n) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->reserve(n);
}
OTIO_API void SerializableObjectVector_swap(
        SerializableObjectVector *self, SerializableObjectVector *other) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->swap(
            *reinterpret_cast<SerializableObjectVectorDef *>(other));
}
OTIO_API OTIOSerializableObject *
SerializableObjectVector_at(SerializableObjectVector *self, int pos) {
    return reinterpret_cast<OTIOSerializableObject *>(
            reinterpret_cast<SerializableObjectVectorDef *>(self)->at(pos));
}
OTIO_API void SerializableObjectVector_push_back(
        SerializableObjectVector *self, OTIOSerializableObject *value) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->push_back(
            reinterpret_cast<OTIO_NS::SerializableObject *>(value));
}
OTIO_API void SerializableObjectVector_pop_back(SerializableObjectVector *self) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->pop_back();
}
OTIO_API SerializableObjectVectorIterator *SerializableObjectVector_insert(
        SerializableObjectVector *self,
        SerializableObjectVectorIterator *pos,
        OTIOSerializableObject *val) {
    SerializableObjectVectorIteratorDef iter =
            reinterpret_cast<SerializableObjectVectorDef *>(self)->insert(
                    *reinterpret_cast<SerializableObjectVectorIteratorDef *>(pos),
                    reinterpret_cast<OTIO_NS::SerializableObject *>(val));
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(iter));
}
OTIO_API void SerializableObjectVector_clear(SerializableObjectVector *self) {
    reinterpret_cast<SerializableObjectVectorDef *>(self)->clear();
}
OTIO_API SerializableObjectVectorIterator *SerializableObjectVector_erase(
        SerializableObjectVector *self, SerializableObjectVectorIterator *pos) {
    SerializableObjectVectorIteratorDef iter =
            reinterpret_cast<SerializableObjectVectorDef *>(self)->erase(
                    *reinterpret_cast<SerializableObjectVectorIteratorDef *>(pos));
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(iter));
}
OTIO_API SerializableObjectVectorIterator *SerializableObjectVector_erase_range(
        SerializableObjectVector *self,
        SerializableObjectVectorIterator *first,
        SerializableObjectVectorIterator *last) {
    SerializableObjectVectorIteratorDef iter =
            reinterpret_cast<SerializableObjectVectorDef *>(self)->erase(
                    *reinterpret_cast<SerializableObjectVectorIteratorDef *>(first),
                    *reinterpret_cast<SerializableObjectVectorIteratorDef *>(last));
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(iter));
}
OTIO_API void SerializableObjectVectorIterator_advance(
        SerializableObjectVectorIterator *iter, int dist) {
    std::advance(
            *reinterpret_cast<SerializableObjectVectorIteratorDef *>(iter),
            dist);
}
OTIO_API SerializableObjectVectorIterator *SerializableObjectVectorIterator_next(
        SerializableObjectVectorIterator *iter, int dist) {
    SerializableObjectVectorIteratorDef it = std::next(
            *reinterpret_cast<SerializableObjectVectorIteratorDef *>(iter),
            dist);
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(it));
}
OTIO_API SerializableObjectVectorIterator *SerializableObjectVectorIterator_prev(
        SerializableObjectVectorIterator *iter, int dist) {
    SerializableObjectVectorIteratorDef it = std::prev(
            *reinterpret_cast<SerializableObjectVectorIteratorDef *>(iter),
            dist);
    return reinterpret_cast<SerializableObjectVectorIterator *>(
            new SerializableObjectVectorIteratorDef(it));
}
OTIO_API OTIOSerializableObject *SerializableObjectVectorIterator_value(
        SerializableObjectVectorIterator *iter) {
    OTIO_NS::SerializableObject *obj =
            **reinterpret_cast<SerializableObjectVectorIteratorDef *>(iter);
    return reinterpret_cast<OTIOSerializableObject *>(obj);
}
OTIO_API bool SerializableObjectVectorIterator_equal(
        SerializableObjectVectorIterator *lhs,
        SerializableObjectVectorIterator *rhs) {
    return *reinterpret_cast<SerializableObjectVectorIteratorDef *>(lhs) ==
           *reinterpret_cast<SerializableObjectVectorIteratorDef *>(rhs);
}
OTIO_API bool SerializableObjectVectorIterator_not_equal(
        SerializableObjectVectorIterator *lhs,
        SerializableObjectVectorIterator *rhs) {
    return *reinterpret_cast<SerializableObjectVectorIteratorDef *>(lhs) !=
           *reinterpret_cast<SerializableObjectVectorIteratorDef *>(rhs);
}
OTIO_API void SerializableObjectVectorIterator_destroy(
        SerializableObjectVectorIterator *self) {
    delete reinterpret_cast<SerializableObjectVectorIteratorDef *>(self);
}
