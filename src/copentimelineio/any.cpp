// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/any.h"
#include <opentimelineio/any.h>
#include <opentimelineio/version.h>

OTIO_API void Any_destroy(Any *self) {
    delete reinterpret_cast<OTIO_NS::any *>(self);
}
