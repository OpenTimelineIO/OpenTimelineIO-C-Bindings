#pragma once

#include "copentime/rationalTime.h"

#ifdef __cplusplus
# define OTIO_API extern "C"
#else
# define OTIO_API
#endif

struct OptionalPairRationalTime;
typedef struct OptionalPairRationalTime OptionalPairRationalTime;

OTIO_API OptionalPairRationalTime *
OptionalPairRationalTime_create(RationalTime *first, RationalTime *second);

OTIO_API bool
OptionalPairRationalTime_first(OptionalPairRationalTime *self, RationalTime &first);

OTIO_API bool
OptionalPairRationalTime_second(OptionalPairRationalTime *self, RationalTime &second);

OTIO_API void OptionalPairRationalTime_destroy(OptionalPairRationalTime *self);