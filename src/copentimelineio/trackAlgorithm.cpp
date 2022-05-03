// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"
#include "copentimelineio/trackAlgorithm.h"
#include <opentimelineio/errorStatus.h>
#include <opentimelineio/track.h>
#include <opentimelineio/trackAlgorithm.h>

OTIO_API Track *track_trimmed_to_range(
        Track *in_track, TimeRange trim_range, OTIOErrorStatus *error_status) {
    return reinterpret_cast<Track *>(OTIO_NS::track_trimmed_to_range(
            reinterpret_cast<OTIO_NS::Track *>(in_track),
            CTimeRange_to_CppTimeRange(trim_range),
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status)));
}
