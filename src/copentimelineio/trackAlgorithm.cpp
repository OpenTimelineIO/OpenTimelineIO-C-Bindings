#include "copentime/util.h"
#include "copentimelineio/trackAlgorithm.h"
#include <opentimelineio/errorStatus.h>
#include <opentimelineio/track.h>
#include <opentimelineio/trackAlgorithm.h>

OTIO_API Track *track_trimmed_to_range(
        Track *in_track, TimeRange trim_range, OTIOErrorStatus *error_status) {
    return reinterpret_cast<Track *>(OTIO_NS::track_trimmed_to_range(
            reinterpret_cast<OTIO_NS::Track *>(in_track),
            _COTTimeRange_to_OTTimeRange(trim_range),
            reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status)));
}