// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/stackAlgorithm.h"
#include <opentimelineio/errorStatus.h>
#include <opentimelineio/stack.h>
#include <opentimelineio/stackAlgorithm.h>
#include <opentimelineio/track.h>
#include <vector>

typedef std::vector<OTIO_NS::Track*> TrackVectorDef;


OTIO_API Track* flatten_stack(Stack* in_stack, OTIOErrorStatus* error_status)
{
    OTIO_NS::Track* track = OTIO_NS::flatten_stack(
        reinterpret_cast<OTIO_NS::Stack*>(in_stack),
        reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
    return reinterpret_cast<Track*>(track);
}
OTIO_API Track* flatten_stack_track_vector(
    TrackVector* tracks, OTIOErrorStatus* error_status)
{
    OTIO_NS::Track* track = OTIO_NS::flatten_stack(
        *reinterpret_cast<TrackVectorDef*>(tracks),
        reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
    return reinterpret_cast<Track*>(track);
}
