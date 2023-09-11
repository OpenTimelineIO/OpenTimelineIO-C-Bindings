// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentime/util.h"

OTIO_API void Opentime_Free(void *ptr){
    free(ptr);
}