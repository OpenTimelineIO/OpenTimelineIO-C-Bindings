// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/composableVector.h"
#include <opentimelineio/composable.h>
#include <vector>

typedef std::vector<OTIO_NS::Composable*>           ComposableVectorDef;
typedef std::vector<OTIO_NS::Composable*>::iterator ComposableVectorIteratorDef;

OTIO_API ComposableVector* ComposableVector_create()
{
    return reinterpret_cast<ComposableVector*>(new ComposableVectorDef());
}
OTIO_API void ComposableVector_destroy(ComposableVector* self)
{
    delete reinterpret_cast<ComposableVectorDef*>(self);
}
OTIO_API ComposableVectorIterator* ComposableVector_begin(ComposableVector* self)
{
    ComposableVectorIteratorDef iter =
        reinterpret_cast<ComposableVectorDef*>(self)->begin();
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(iter));
}
OTIO_API ComposableVectorIterator* ComposableVector_end(ComposableVector* self)
{
    ComposableVectorIteratorDef iter =
        reinterpret_cast<ComposableVectorDef*>(self)->end();
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(iter));
}
OTIO_API int ComposableVector_size(ComposableVector* self)
{
    return reinterpret_cast<ComposableVectorDef*>(self)->size();
}
OTIO_API int ComposableVector_max_size(ComposableVector* self)
{
    return reinterpret_cast<ComposableVectorDef*>(self)->max_size();
}
OTIO_API int ComposableVector_capacity(ComposableVector* self)
{
    return reinterpret_cast<ComposableVectorDef*>(self)->capacity();
}
OTIO_API void ComposableVector_resize(ComposableVector* self, int n)
{
    reinterpret_cast<ComposableVectorDef*>(self)->resize(n);
}
OTIO_API bool ComposableVector_empty(ComposableVector* self)
{
    return reinterpret_cast<ComposableVectorDef*>(self)->empty();
}
OTIO_API void ComposableVector_shrink_to_fit(ComposableVector* self)
{
    reinterpret_cast<ComposableVectorDef*>(self)->shrink_to_fit();
}
OTIO_API void ComposableVector_reserve(ComposableVector* self, int n)
{
    reinterpret_cast<ComposableVectorDef*>(self)->reserve(n);
}
OTIO_API void ComposableVector_swap(ComposableVector* self, ComposableVector* other)
{
    reinterpret_cast<ComposableVectorDef*>(self)->swap(
        *reinterpret_cast<ComposableVectorDef*>(other));
}
OTIO_API Composable* ComposableVector_at(ComposableVector* self, int pos)
{
    return reinterpret_cast<Composable*>(
        reinterpret_cast<ComposableVectorDef*>(self)->at(pos));
}
OTIO_API void ComposableVector_push_back(ComposableVector* self, Composable* value)
{
    reinterpret_cast<ComposableVectorDef*>(self)->push_back(
        reinterpret_cast<OTIO_NS::Composable*>(value));
}
OTIO_API void ComposableVector_pop_back(ComposableVector* self)
{
    reinterpret_cast<ComposableVectorDef*>(self)->pop_back();
}
OTIO_API ComposableVectorIterator* ComposableVector_insert(
    ComposableVector* self, ComposableVectorIterator* pos, Composable* val)
{
    ComposableVectorIteratorDef iter =
        reinterpret_cast<ComposableVectorDef*>(self)->insert(
            *reinterpret_cast<ComposableVectorIteratorDef*>(pos),
            reinterpret_cast<OTIO_NS::Composable*>(val));
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(iter));
}
OTIO_API void ComposableVector_clear(ComposableVector* self)
{
    reinterpret_cast<ComposableVectorDef*>(self)->clear();
}
OTIO_API ComposableVectorIterator* ComposableVector_erase(
    ComposableVector* self, ComposableVectorIterator* pos)
{
    ComposableVectorIteratorDef iter =
        reinterpret_cast<ComposableVectorDef*>(self)->erase(
            *reinterpret_cast<ComposableVectorIteratorDef*>(pos));
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(iter));
}
OTIO_API ComposableVectorIterator* ComposableVector_erase_range(
    ComposableVector*         self,
    ComposableVectorIterator* first,
    ComposableVectorIterator* last)
{
    ComposableVectorIteratorDef iter =
        reinterpret_cast<ComposableVectorDef*>(self)->erase(
            *reinterpret_cast<ComposableVectorIteratorDef*>(first),
            *reinterpret_cast<ComposableVectorIteratorDef*>(last));
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(iter));
}
OTIO_API void
ComposableVectorIterator_advance(ComposableVectorIterator* iter, int dist)
{
    std::advance(
        *reinterpret_cast<ComposableVectorIteratorDef*>(iter), dist);
}
OTIO_API ComposableVectorIterator*
ComposableVectorIterator_next(ComposableVectorIterator* iter, int dist)
{
    ComposableVectorIteratorDef it = std::next(
        *reinterpret_cast<ComposableVectorIteratorDef*>(iter), dist);
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(it));
}
OTIO_API ComposableVectorIterator*
ComposableVectorIterator_prev(ComposableVectorIterator* iter, int dist)
{
    ComposableVectorIteratorDef it = std::prev(
        *reinterpret_cast<ComposableVectorIteratorDef*>(iter), dist);
    return reinterpret_cast<ComposableVectorIterator*>(
        new ComposableVectorIteratorDef(it));
}
OTIO_API Composable* ComposableVectorIterator_value(ComposableVectorIterator* iter)
{
    OTIO_NS::Composable* obj =
        **reinterpret_cast<ComposableVectorIteratorDef*>(iter);
    return reinterpret_cast<Composable*>(obj);
}
OTIO_API bool ComposableVectorIterator_equal(
    ComposableVectorIterator* lhs, ComposableVectorIterator* rhs)
{
    return *reinterpret_cast<ComposableVectorIteratorDef*>(lhs) ==
           *reinterpret_cast<ComposableVectorIteratorDef*>(rhs);
}
OTIO_API bool ComposableVectorIterator_not_equal(
    ComposableVectorIterator* lhs, ComposableVectorIterator* rhs)
{
    return *reinterpret_cast<ComposableVectorIteratorDef*>(lhs) !=
           *reinterpret_cast<ComposableVectorIteratorDef*>(rhs);
}
OTIO_API void ComposableVectorIterator_destroy(ComposableVectorIterator* self)
{
    delete reinterpret_cast<ComposableVectorIteratorDef*>(self);
}
