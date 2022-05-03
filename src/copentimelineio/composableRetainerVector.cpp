// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/composableRetainerVector.h"
#include <opentimelineio/composable.h>
#include <vector>

typedef std::vector<OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>
    ComposableRetainerVectorDef;
typedef std::vector<OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>::
    iterator ComposableRetainerVectorIteratorDef;
typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Composable>
    ComposableRetainer;

OTIO_API ComposableRetainerVector* ComposableRetainerVector_create()
{
    return reinterpret_cast<ComposableRetainerVector*>(
        new ComposableRetainerVectorDef());
}
OTIO_API void ComposableRetainerVector_destroy(ComposableRetainerVector* self)
{
    delete reinterpret_cast<ComposableRetainerVectorDef*>(self);
}
OTIO_API ComposableRetainerVectorIterator*
ComposableRetainerVector_begin(ComposableRetainerVector* self)
{
    ComposableRetainerVectorIteratorDef iter =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->begin();
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(iter));
}
OTIO_API ComposableRetainerVectorIterator*
ComposableRetainerVector_end(ComposableRetainerVector* self)
{
    ComposableRetainerVectorIteratorDef iter =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->end();
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(iter));
}
OTIO_API int ComposableRetainerVector_size(ComposableRetainerVector* self)
{
    return reinterpret_cast<ComposableRetainerVectorDef*>(self)->size();
}
OTIO_API int ComposableRetainerVector_max_size(ComposableRetainerVector* self)
{
    return reinterpret_cast<ComposableRetainerVectorDef*>(self)->max_size();
}
OTIO_API int ComposableRetainerVector_capacity(ComposableRetainerVector* self)
{
    return reinterpret_cast<ComposableRetainerVectorDef*>(self)->capacity();
}
OTIO_API void ComposableRetainerVector_resize(ComposableRetainerVector* self, int n)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->resize(n);
}
OTIO_API bool ComposableRetainerVector_empty(ComposableRetainerVector* self)
{
    return reinterpret_cast<ComposableRetainerVectorDef*>(self)->empty();
}
OTIO_API void ComposableRetainerVector_shrink_to_fit(ComposableRetainerVector* self)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->shrink_to_fit();
}
OTIO_API void ComposableRetainerVector_reserve(ComposableRetainerVector* self, int n)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->reserve(n);
}
OTIO_API void ComposableRetainerVector_swap(
    ComposableRetainerVector* self, ComposableRetainerVector* other)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->swap(
        *reinterpret_cast<ComposableRetainerVectorDef*>(other));
}
OTIO_API RetainerComposable*
ComposableRetainerVector_at(ComposableRetainerVector* self, int pos)
{
    ComposableRetainer obj =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->at(pos);
    return reinterpret_cast<RetainerComposable*>(
        new ComposableRetainer(obj));
}
OTIO_API void ComposableRetainerVector_push_back(
    ComposableRetainerVector* self, RetainerComposable* value)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->push_back(
        *reinterpret_cast<ComposableRetainer*>(value));
}
OTIO_API void ComposableRetainerVector_pop_back(ComposableRetainerVector* self)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->pop_back();
}
OTIO_API ComposableRetainerVectorIterator* ComposableRetainerVector_insert(
    ComposableRetainerVector*         self,
    ComposableRetainerVectorIterator* pos,
    RetainerComposable*               val)
{
    ComposableRetainerVectorIteratorDef iter =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->insert(
            *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(pos),
            *reinterpret_cast<ComposableRetainer*>(val));
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(iter));
}
OTIO_API void ComposableRetainerVector_clear(ComposableRetainerVector* self)
{
    reinterpret_cast<ComposableRetainerVectorDef*>(self)->clear();
}
OTIO_API ComposableRetainerVectorIterator* ComposableRetainerVector_erase(
    ComposableRetainerVector* self, ComposableRetainerVectorIterator* pos)
{
    ComposableRetainerVectorIteratorDef iter =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->erase(
            *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(pos));
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(iter));
}
OTIO_API ComposableRetainerVectorIterator* ComposableRetainerVector_erase_range(
    ComposableRetainerVector*         self,
    ComposableRetainerVectorIterator* first,
    ComposableRetainerVectorIterator* last)
{
    ComposableRetainerVectorIteratorDef iter =
        reinterpret_cast<ComposableRetainerVectorDef*>(self)->erase(
            *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(first),
            *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(last));
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(iter));
}
OTIO_API void ComposableRetainerVectorIterator_advance(
    ComposableRetainerVectorIterator* iter, int dist)
{
    std::advance(
        *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(iter),
        dist);
}
OTIO_API ComposableRetainerVectorIterator* ComposableRetainerVectorIterator_next(
    ComposableRetainerVectorIterator* iter, int dist)
{
    ComposableRetainerVectorIteratorDef it = std::next(
        *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(iter),
        dist);
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(it));
}
OTIO_API ComposableRetainerVectorIterator* ComposableRetainerVectorIterator_prev(
    ComposableRetainerVectorIterator* iter, int dist)
{
    ComposableRetainerVectorIteratorDef it = std::prev(
        *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(iter),
        dist);
    return reinterpret_cast<ComposableRetainerVectorIterator*>(
        new ComposableRetainerVectorIteratorDef(it));
}
OTIO_API RetainerComposable* ComposableRetainerVectorIterator_value(
    ComposableRetainerVectorIterator* iter)
{
    ComposableRetainer obj =
        **reinterpret_cast<ComposableRetainerVectorIteratorDef*>(iter);
    return reinterpret_cast<RetainerComposable*>(
        new ComposableRetainer(obj));
}
OTIO_API bool ComposableRetainerVectorIterator_equal(
    ComposableRetainerVectorIterator* lhs,
    ComposableRetainerVectorIterator* rhs)
{
    return *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(lhs) ==
           *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(rhs);
}
OTIO_API bool ComposableRetainerVectorIterator_not_equal(
    ComposableRetainerVectorIterator* lhs,
    ComposableRetainerVectorIterator* rhs)
{
    return *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(lhs) !=
           *reinterpret_cast<ComposableRetainerVectorIteratorDef*>(rhs);
}
OTIO_API void ComposableRetainerVectorIterator_destroy(
    ComposableRetainerVectorIterator* self)
{
    delete reinterpret_cast<ComposableRetainerVectorIteratorDef*>(self);
}
