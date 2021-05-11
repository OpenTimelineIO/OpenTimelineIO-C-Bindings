#include "copentimelineio/retainerPairComposable.h"
#include <opentimelineio/composable.h>
#include <utility>

typedef std::pair<
        OTIO_NS::Composable::Retainer<OTIO_NS::Composable>,
        OTIO_NS::Composable::Retainer<OTIO_NS::Composable>>
        PairDef;
typedef OTIO_NS::SerializableObject::Retainer<OTIO_NS::Composable>
        ComposableRetainer;

OTIO_API RetainerPairComposable *RetainerPairComposable_create(
        RetainerComposable *first, RetainerComposable *second) {
    ComposableRetainer firstComposableRetainer =
            *reinterpret_cast<ComposableRetainer *>(first);
    ComposableRetainer secondComposableRetainer =
            *reinterpret_cast<ComposableRetainer *>(second);
    return reinterpret_cast<RetainerPairComposable *>(
            new PairDef(firstComposableRetainer, secondComposableRetainer));
}

OTIO_API RetainerComposable *
RetainerPairComposable_first(RetainerPairComposable *self) {
    ComposableRetainer composableRetainer =
            reinterpret_cast<PairDef *>(self)->first;
    return reinterpret_cast<RetainerComposable *>(
            new ComposableRetainer(composableRetainer));
}

OTIO_API RetainerComposable *
RetainerPairComposable_second(RetainerPairComposable *self) {
    ComposableRetainer composableRetainer =
            reinterpret_cast<PairDef *>(self)->second;
    return reinterpret_cast<RetainerComposable *>(
            new ComposableRetainer(composableRetainer));
}

OTIO_API void RetainerPairComposable_destroy(RetainerPairComposable *self) {
    delete reinterpret_cast<PairDef *>(self);
}