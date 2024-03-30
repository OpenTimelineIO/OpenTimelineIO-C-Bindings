// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/anyVector.h"
#include <opentimelineio/anyVector.h>
#include <opentimelineio/version.h>
#include <any>

typedef std::vector<std::any>::iterator VectorIterator;

OTIO_API AnyVector *AnyVector_create() {
    return reinterpret_cast<AnyVector *>(new OTIO_NS::AnyVector());
}

OTIO_API void AnyVector_destroy(AnyVector *self) {
    delete reinterpret_cast<OTIO_NS::AnyVector *>(self);
}

OTIO_API AnyVectorIterator *AnyVector_begin(AnyVector *self) {
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(
            reinterpret_cast<OTIO_NS::AnyVector *>(self)->begin()));
}

OTIO_API AnyVectorIterator *AnyVector_end(AnyVector *self) {
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(
            reinterpret_cast<OTIO_NS::AnyVector *>(self)->end()));
}

OTIO_API int AnyVector_size(AnyVector *self) {
    return static_cast<int>(reinterpret_cast<OTIO_NS::AnyVector *>(self)->size());
}

OTIO_API int AnyVector_max_size(AnyVector *self) {
    return static_cast<int>(reinterpret_cast<OTIO_NS::AnyVector *>(self)->max_size());
}

OTIO_API int AnyVector_capacity(AnyVector *self) {
    return static_cast<int>(reinterpret_cast<OTIO_NS::AnyVector *>(self)->capacity());
}

OTIO_API void AnyVector_resize(AnyVector *self, int n) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->resize(n);
}

OTIO_API bool AnyVector_empty(AnyVector *self) {
    return reinterpret_cast<OTIO_NS::AnyVector *>(self)->empty();
}

OTIO_API void AnyVector_shrink_to_fit(AnyVector *self) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->shrink_to_fit();
}

OTIO_API void AnyVector_reserve(AnyVector *self, int n) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->reserve(n);
}

OTIO_API void AnyVector_swap(AnyVector *self, AnyVector *other) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->swap(
            *reinterpret_cast<OTIO_NS::AnyVector *>(other));
}

OTIO_API Any *AnyVector_at(AnyVector *self, int pos) {
    std::any value =
            reinterpret_cast<OTIO_NS::AnyVector *>(self)->at(pos);
    return reinterpret_cast<Any *>(new std::any(value));
}

OTIO_API void AnyVector_push_back(AnyVector *self, Any *value) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->push_back(
            *reinterpret_cast<std::any *>(value));
}

OTIO_API void AnyVector_pop_back(AnyVector *self) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->pop_back();
}

OTIO_API AnyVectorIterator *
AnyVector_insert(AnyVector *self, AnyVectorIterator *pos, Any *val) {
    VectorIterator it = reinterpret_cast<OTIO_NS::AnyVector *>(self)->insert(
            *reinterpret_cast<VectorIterator *>(pos),
            *reinterpret_cast<std::any *>(val));
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(it));
}

OTIO_API void AnyVector_clear(AnyVector *self) {
    reinterpret_cast<OTIO_NS::AnyVector *>(self)->clear();
}

OTIO_API AnyVectorIterator *AnyVector_erase(AnyVector *self, AnyVectorIterator *pos) {
    VectorIterator it = reinterpret_cast<OTIO_NS::AnyVector *>(self)->erase(
            *reinterpret_cast<VectorIterator *>(pos));
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(it));
}

OTIO_API AnyVectorIterator *AnyVector_erase_range(
        AnyVector *self, AnyVectorIterator *first, AnyVectorIterator *last) {
    VectorIterator it = reinterpret_cast<OTIO_NS::AnyVector *>(self)->erase(
            *reinterpret_cast<VectorIterator *>(first),
            *reinterpret_cast<VectorIterator *>(last));
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(it));
}

OTIO_API void AnyVectorIterator_advance(AnyVectorIterator *iter, int dist) {
    std::advance(*reinterpret_cast<VectorIterator *>(iter), dist);
}

OTIO_API AnyVectorIterator *AnyVectorIterator_next(AnyVectorIterator *iter, int dist) {
    VectorIterator it =
            std::next(*reinterpret_cast<VectorIterator *>(iter), dist);
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(it));
}

OTIO_API AnyVectorIterator *AnyVectorIterator_prev(AnyVectorIterator *iter, int dist) {
    VectorIterator it =
            std::prev(*reinterpret_cast<VectorIterator *>(iter), dist);
    return reinterpret_cast<AnyVectorIterator *>(new VectorIterator(it));
}

OTIO_API Any *AnyVectorIterator_value(AnyVectorIterator *iter) {
    std::any value = *reinterpret_cast<VectorIterator *>(iter);
    return reinterpret_cast<Any *>(new std::any(value));
}

OTIO_API bool
AnyVectorIterator_equal(AnyVectorIterator *lhs, AnyVectorIterator *rhs) {
    return *reinterpret_cast<VectorIterator *>(lhs) ==
           *reinterpret_cast<VectorIterator *>(rhs);
}

OTIO_API bool
AnyVectorIterator_not_equal(AnyVectorIterator *lhs, AnyVectorIterator *rhs) {
    return *reinterpret_cast<VectorIterator *>(lhs) ==
           *reinterpret_cast<VectorIterator *>(rhs);
}

OTIO_API void AnyVectorIterator_destroy(AnyVectorIterator *self) {
    delete reinterpret_cast<VectorIterator *>(self);
}

OTIO_API AnyVectorMutationStamp *AnyVectorMutationStamp_create(AnyVector *v) {
    return reinterpret_cast<AnyVectorMutationStamp *>(
            new OTIO_NS::AnyVector::MutationStamp(
                    reinterpret_cast<OTIO_NS::AnyVector *>(v)));
}

OTIO_API void AnyVectorMutationStamp_destroy(AnyVectorMutationStamp *self) {
    delete reinterpret_cast<OTIO_NS::AnyVector::MutationStamp *>(self);
}

OTIO_API AnyVectorMutationStamp *
AnyVector_get_or_create_mutation_stamp(AnyVector *self) {
    return reinterpret_cast<AnyVectorMutationStamp *>(
            reinterpret_cast<OTIO_NS::AnyVector *>(self)
                    ->get_or_create_mutation_stamp());
}
