#include "copentimelineio/anyDictionary.h"
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/version.h>
#include <string.h>

typedef std::map<std::string, OTIO_NS::any>::iterator DictionaryIterator;

OTIO_API AnyDictionary* AnyDictionary_create()
{
    return reinterpret_cast<AnyDictionary*>(new OTIO_NS::AnyDictionary());
}
OTIO_API void AnyDictionary_destroy(AnyDictionary* self)
{
    delete reinterpret_cast<OTIO_NS::AnyDictionary*>(self);
}
OTIO_API void AnyDictionary_clear(AnyDictionary* self)
{
    reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->clear();
}
OTIO_API AnyDictionaryIterator* AnyDictionary_begin(AnyDictionary* self)
{
    return reinterpret_cast<AnyDictionaryIterator*>(new DictionaryIterator(
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->begin()));
}
OTIO_API AnyDictionaryIterator* AnyDictionary_end(AnyDictionary* self)
{
    return reinterpret_cast<AnyDictionaryIterator*>(new DictionaryIterator(
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->end()));
}
OTIO_API void AnyDictionary_swap(AnyDictionary* self, AnyDictionary* other)
{
    reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->swap(
        *reinterpret_cast<OTIO_NS::AnyDictionary*>(other));
}
OTIO_API AnyDictionaryIterator*
AnyDictionary_erase(AnyDictionary* self, AnyDictionaryIterator* pos)
{
    DictionaryIterator it =
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->erase(
            *reinterpret_cast<DictionaryIterator*>(pos));
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(it));
}
OTIO_API AnyDictionaryIterator* AnyDictionary_erase_range(
    AnyDictionary*         self,
    AnyDictionaryIterator* first,
    AnyDictionaryIterator* last)
{
    DictionaryIterator it =
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->erase(
            *reinterpret_cast<DictionaryIterator*>(first),
            *reinterpret_cast<DictionaryIterator*>(last));
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(it));
}
OTIO_API int AnyDictionary_erase_key(AnyDictionary* self, const char* key)
{
    return reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->erase(key);
}
OTIO_API int AnyDictionary_size(AnyDictionary* self)
{
    return reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->size();
}
OTIO_API int AnyDictionary_max_size(AnyDictionary* self)
{
    return reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->max_size();
}
OTIO_API bool AnyDictionary_empty(AnyDictionary* self)
{
    return reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->empty();
}
OTIO_API AnyDictionaryIterator*
AnyDictionary_find(AnyDictionary* self, const char* key)
{
    DictionaryIterator iter =
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)->find(key);
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(iter));
}
OTIO_API AnyDictionaryIterator*
AnyDictionary_insert(AnyDictionary* self, const char* key, Any* anyObj)
{
    DictionaryIterator it =
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)
            ->insert({ key, *reinterpret_cast<OTIO_NS::any*>(anyObj) })
            .first;
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(it));
}
OTIO_API void AnyDictionaryIterator_advance(AnyDictionaryIterator* iter, int dist)
{
    std::advance(*reinterpret_cast<DictionaryIterator*>(iter), dist);
}
OTIO_API AnyDictionaryIterator*
AnyDictionaryIterator_next(AnyDictionaryIterator* iter, int dist)
{
    DictionaryIterator it =
        std::next(*reinterpret_cast<DictionaryIterator*>(iter), dist);
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(it));
}
OTIO_API AnyDictionaryIterator*
AnyDictionaryIterator_prev(AnyDictionaryIterator* iter, int dist)
{
    DictionaryIterator it =
        std::prev(*reinterpret_cast<DictionaryIterator*>(iter), dist);
    return reinterpret_cast<AnyDictionaryIterator*>(
        new DictionaryIterator(it));
}
OTIO_API otiostr AnyDictionaryIterator_key(AnyDictionaryIterator* iter)
{
    std::string returnStr =
        (*reinterpret_cast<DictionaryIterator*>(iter))->first;
    otiostr keyStr = otiostr_create(returnStr.c_str());
    return keyStr;
}
OTIO_API Any* AnyDictionaryIterator_value(AnyDictionaryIterator* iter)
{
    OTIO_NS::any value =
        (*reinterpret_cast<DictionaryIterator*>(iter))->second;
    return reinterpret_cast<Any*>(new OTIO_NS::any(value));
}
OTIO_API bool AnyDictionaryIterator_equal(
    AnyDictionaryIterator* lhs, AnyDictionaryIterator* rhs)
{
    return *reinterpret_cast<DictionaryIterator*>(lhs) ==
           *reinterpret_cast<DictionaryIterator*>(rhs);
}
OTIO_API bool AnyDictionaryIterator_not_equal(
    AnyDictionaryIterator* lhs, AnyDictionaryIterator* rhs)
{
    return *reinterpret_cast<DictionaryIterator*>(lhs) !=
           *reinterpret_cast<DictionaryIterator*>(rhs);
}
OTIO_API void AnyDictionaryIterator_destroy(AnyDictionaryIterator* self)
{
    delete reinterpret_cast<DictionaryIterator*>(self);
}
OTIO_API AnyDictionaryMutationStamp*
AnyDictionaryMutationStamp_create(AnyDictionary* d)
{
    return reinterpret_cast<AnyDictionaryMutationStamp*>(
        new OTIO_NS::AnyDictionary::MutationStamp(
            reinterpret_cast<OTIO_NS::AnyDictionary*>(d)));
}
OTIO_API void AnyDictionaryMutationStamp_destroy(AnyDictionaryMutationStamp* self)
{
    delete reinterpret_cast<OTIO_NS::AnyDictionary::MutationStamp*>(self);
}
OTIO_API AnyDictionaryMutationStamp*
AnyDictionary_get_or_create_mutation_stamp(AnyDictionary* self)
{
    return reinterpret_cast<AnyDictionaryMutationStamp*>(
        reinterpret_cast<OTIO_NS::AnyDictionary*>(self)
            ->get_or_create_mutation_stamp());
}
