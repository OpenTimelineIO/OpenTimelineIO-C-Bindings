// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include <copentimelineio/clip.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/gap.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableCollection.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>
#include <copentimelineio/stackAlgorithm.h>
#include <copentimelineio/timeline.h>
#include <copentimelineio/track.h>
#include <copentimelineio/transition.h>

#define xstr(s) str(s)
#define str(s) #s

struct StackAlgoTestState {
    const char *sample_data_dir;

    Track *trackZ;
    Track *trackABC;
    Track *trackDgE;
    Track *trackgFg;
    RetainerSerializableObject *trackZ_r;
    RetainerSerializableObject *trackABC_r;
    RetainerSerializableObject *trackDgE_r;
    RetainerSerializableObject *trackgFg_r;

    const char *trackZStr;
    const char *trackABCStr;
    const char *trackDgEStr;
    const char *trackgFgStr;
};

static int setupStackAlgoTests(void **state) {
    struct StackAlgoTestState *testState = malloc(sizeof(struct StackAlgoTestState));

    testState->trackZStr =
            "{\n"
            "            \"OTIO_SCHEMA\": \"Track.1\",\n"
            "            \"children\": [\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"Z\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 150\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "            ],\n"
            "            \"effects\": [],\n"
            "            \"kind\": \"Video\",\n"
            "            \"markers\": [],\n"
            "            \"metadata\": {},\n"
            "            \"name\": \"Sequence1\",\n"
            "            \"source_range\": null\n"
            "        }";

    testState->trackABCStr =
            "{\n"
            "            \"OTIO_SCHEMA\": \"Track.1\",\n"
            "            \"children\": [\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"A\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"B\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"C\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "            ],\n"
            "            \"effects\": [],\n"
            "            \"kind\": \"Video\",\n"
            "            \"markers\": [],\n"
            "            \"metadata\": {},\n"
            "            \"name\": \"Sequence1\",\n"
            "            \"source_range\": null\n"
            "        }";

    testState->trackDgEStr =
            "{\n"
            "            \"OTIO_SCHEMA\": \"Track.1\",\n"
            "            \"children\": [\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"D\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Gap.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"g\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"E\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "            ],\n"
            "            \"effects\": [],\n"
            "            \"kind\": \"Video\",\n"
            "            \"markers\": [],\n"
            "            \"metadata\": {},\n"
            "            \"name\": \"Sequence1\",\n"
            "            \"source_range\": null\n"
            "        }";

    testState->trackgFgStr =
            "{\n"
            "            \"OTIO_SCHEMA\": \"Track.1\",\n"
            "            \"children\": [\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Gap.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"g1\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"F\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Gap.1\",\n"
            "                    \"effects\": [],\n"
            "                    \"markers\": [],\n"
            "                    \"media_reference\": null,\n"
            "                    \"metadata\": {},\n"
            "                    \"name\": \"g2\",\n"
            "                    \"source_range\": {\n"
            "                        \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "                        \"duration\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 50\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 0.0\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "            ],\n"
            "            \"effects\": [],\n"
            "            \"kind\": \"Video\",\n"
            "            \"markers\": [],\n"
            "            \"metadata\": {},\n"
            "            \"name\": \"Sequence1\",\n"
            "            \"source_range\": null\n"
            "        }";

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

/* trackZ */
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    bool decoded_successfully =
            deserialize_json_from_string(testState->trackZStr, decoded, errorStatus);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    testState->trackZ = (Track *) decoded_object;
    testState->trackZ_r = RetainerSerializableObject_create(decoded_object);

/* trackABC */
    decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    decoded_successfully =
            deserialize_json_from_string(testState->trackABCStr, decoded, errorStatus);
    decoded_object = safely_cast_retainer_any(decoded);
    testState->trackABC = (Track *) decoded_object;
    testState->trackABC_r = RetainerSerializableObject_create(decoded_object);

/* trackDgE */
    decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    decoded_successfully =
            deserialize_json_from_string(testState->trackDgEStr, decoded, errorStatus);
    decoded_object = safely_cast_retainer_any(decoded);
    testState->trackDgE = (Track *) decoded_object;
    testState->trackDgE_r = RetainerSerializableObject_create(decoded_object);

/* trackgFg */
    decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    decoded_successfully =
            deserialize_json_from_string(testState->trackgFgStr, decoded, errorStatus);
    decoded_object = safely_cast_retainer_any(decoded);
    testState->trackgFg = (Track *) decoded_object;
    testState->trackgFg_r = RetainerSerializableObject_create(decoded_object);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;

    testState->sample_data_dir = xstr(SAMPLE_DATA_DIR);
    *state = testState;
    return 0;
}

static int teardownStackAlgoTests(void **state) {
    struct StackAlgoTestState *testState = *state;
    RetainerSerializableObject_managed_destroy(testState->trackZ_r);
    RetainerSerializableObject_managed_destroy(testState->trackABC_r);
    RetainerSerializableObject_managed_destroy(testState->trackDgE_r);
    RetainerSerializableObject_managed_destroy(testState->trackgFg_r);

    testState->trackZ = testState->trackABC =
    testState->trackDgE = testState->trackgFg = NULL;

    free(testState);
    return 0;
}

static void otio_stack_algo_flatten_single_track_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(stack);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackABC, errorStatus));

    Track *flattenedStack = flatten_stack(stack, errorStatus);
    OTIO_RETAIN(flattenedStack);
    Track_set_name(flattenedStack, "Sequence1");
    Any *s_any = create_safely_typed_any_serializable_object(
            (OTIOSerializableObject *) flattenedStack);

    assert_ptr_not_equal(flattenedStack, trackABC);

    assert_true(Track_is_equivalent_to(flattenedStack, (OTIOSerializableObject *) trackABC));

    OTIO_RELEASE(flattenedStack);
    flattenedStack = NULL;
    OTIOErrorStatus_destroy(errorStatus);

    OTIO_RELEASE(stack);
    stack = NULL;
}

static void otio_stack_algo_flatten_obscured_track_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(stack);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackABC, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackZ, errorStatus));

    Track *flattenedStack = flatten_stack(stack, errorStatus);
    OTIO_RETAIN(flattenedStack);
    Track_set_name(flattenedStack, "Sequence1");
    assert_ptr_not_equal(flattenedStack, trackZ);
    assert_true(Track_is_equivalent_to(flattenedStack, (OTIOSerializableObject *) trackZ));

    OTIO_RELEASE(flattenedStack);
    flattenedStack = NULL;
    OTIO_RELEASE(stack);
    stack = NULL;

    stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackZ, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackABC, errorStatus));
    flattenedStack = flatten_stack(stack, errorStatus);
    Track_set_name(flattenedStack, "Sequence1");
    assert_ptr_not_equal(flattenedStack, trackABC);
    assert_true(Track_is_equivalent_to(flattenedStack, (OTIOSerializableObject *) trackABC));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_stack_algo_flatten_gaps_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(stack);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackABC, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackDgE, errorStatus));

    Track *flattenedStack = flatten_stack(stack, errorStatus);
    Track_set_name(flattenedStack, "Sequence1");

    ComposableRetainerVector *flattenedStackChildren =
            Track_children(flattenedStack);
    ComposableRetainerVectorIterator *flattenedStackIt =
            ComposableRetainerVector_begin(flattenedStackChildren);
    RetainerComposable *flattenedStack_0_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_0 =
            RetainerComposable_value(flattenedStack_0_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    RetainerComposable *flattenedStack_1_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_1 =
            RetainerComposable_value(flattenedStack_1_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    RetainerComposable *flattenedStack_2_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_2 =
            RetainerComposable_value(flattenedStack_2_retainer);
    ComposableRetainerVector_destroy(flattenedStackChildren);
    flattenedStackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flattenedStackIt);
    flattenedStackIt = NULL;

    ComposableRetainerVector *trackDgEChildren = Track_children(trackDgE);
    ComposableRetainerVectorIterator *trackDgEIt =
            ComposableRetainerVector_begin(trackDgEChildren);
    RetainerComposable *trackDgE_0_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_0 = RetainerComposable_value(trackDgE_0_retainer);
    ComposableRetainerVectorIterator_advance(trackDgEIt, 2);
    RetainerComposable *trackDgE_2_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_2 = RetainerComposable_value(trackDgE_2_retainer);
    ComposableRetainerVector_destroy(trackDgEChildren);
    trackDgEChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackDgEIt);
    trackDgEIt = NULL;

    ComposableRetainerVector *trackABCChildren = Track_children(trackABC);
    ComposableRetainerVectorIterator *trackABCIt =
            ComposableRetainerVector_begin(trackABCChildren);
    RetainerComposable *trackABC_0_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_0 = RetainerComposable_value(trackABC_0_retainer);
    ComposableRetainerVectorIterator_advance(trackABCIt, 1);
    RetainerComposable *trackABC_1_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_1 = RetainerComposable_value(trackABC_1_retainer);
    ComposableRetainerVectorIterator_advance(trackABCIt, 1);
    RetainerComposable *trackABC_2_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_2 = RetainerComposable_value(trackABC_2_retainer);
    ComposableRetainerVector_destroy(trackABCChildren);
    trackABCChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackABCIt);
    trackABCIt = NULL;

    assert_ptr_not_equal(flattenedStack_0, trackDgE_0);
    assert_ptr_not_equal(flattenedStack_1, trackABC_1);
    assert_ptr_not_equal(flattenedStack_2, trackDgE_2);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_0,
            (OTIOSerializableObject *) trackDgE_0));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_1,
            (OTIOSerializableObject *) trackABC_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_2,
            (OTIOSerializableObject *) trackDgE_2));

    OTIO_RELEASE(stack);
    stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
//    OTIO_RETAIN(stack); TODO: Fix error: redefinition of ‘stack_r’
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackABC, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackgFg, errorStatus));
    flattenedStack = flatten_stack(stack, errorStatus);
    Track_set_name(flattenedStack, "Sequence1");

    flattenedStackChildren = Track_children(flattenedStack);
    flattenedStackIt = ComposableRetainerVector_begin(flattenedStackChildren);
    flattenedStack_0_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_0 = RetainerComposable_value(flattenedStack_0_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    flattenedStack_1_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_1 = RetainerComposable_value(flattenedStack_1_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    flattenedStack_2_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_2 = RetainerComposable_value(flattenedStack_2_retainer);
    ComposableRetainerVector_destroy(flattenedStackChildren);
    flattenedStackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flattenedStackIt);
    flattenedStackIt = NULL;

    ComposableRetainerVector *trackgFgChildren = Track_children(trackgFg);
    ComposableRetainerVectorIterator *trackgFgIt =
            ComposableRetainerVector_begin(trackgFgChildren);
    ComposableRetainerVectorIterator_advance(trackgFgIt, 1);
    RetainerComposable *trackgFg_1_retainer =
            ComposableRetainerVectorIterator_value(trackgFgIt);
    Composable *trackgFg_1 = RetainerComposable_value(trackgFg_1_retainer);
    ComposableRetainerVector_destroy(trackgFgChildren);
    trackgFgChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackgFgIt);
    trackgFgIt = NULL;

    assert_ptr_not_equal(flattenedStack_0, trackABC_0);
    assert_ptr_not_equal(flattenedStack_1, trackgFg_1);
    assert_ptr_not_equal(flattenedStack_2, trackABC_2);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_0,
            (OTIOSerializableObject *) trackABC_0));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_1,
            (OTIOSerializableObject *) trackgFg_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_2,
            (OTIOSerializableObject *) trackABC_2));

//    OTIO_RELEASE(stack);
//    stack = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_stack_algo_flatten_gaps_with_trims_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(stack);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackZ, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackDgE, errorStatus));

    Track *flattenedStack = flatten_stack(stack, errorStatus);
    OTIO_RETAIN(flattenedStack);
    Track_set_name(flattenedStack, "Sequence1");

    ComposableRetainerVector *flattenedStackChildren =
            Track_children(flattenedStack);
    ComposableRetainerVectorIterator *flattenedStackIt =
            ComposableRetainerVector_begin(flattenedStackChildren);
    RetainerComposable *flattenedStack_0_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_0 =
            RetainerComposable_value(flattenedStack_0_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    RetainerComposable *flattenedStack_1_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_1 =
            RetainerComposable_value(flattenedStack_1_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    RetainerComposable *flattenedStack_2_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    Composable *flattenedStack_2 =
            RetainerComposable_value(flattenedStack_2_retainer);
    ComposableRetainerVector_destroy(flattenedStackChildren);
    flattenedStackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flattenedStackIt);
    flattenedStackIt = NULL;

    ComposableRetainerVector *trackDgEChildren = Track_children(trackDgE);
    ComposableRetainerVectorIterator *trackDgEIt =
            ComposableRetainerVector_begin(trackDgEChildren);
    RetainerComposable *trackDgE_0_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_0 = RetainerComposable_value(trackDgE_0_retainer);
    ComposableRetainerVectorIterator_advance(trackDgEIt, 2);
    RetainerComposable *trackDgE_2_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_2 = RetainerComposable_value(trackDgE_2_retainer);
    ComposableRetainerVector_destroy(trackDgEChildren);
    trackDgEChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackDgEIt);
    trackDgEIt = NULL;
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_0,
            (OTIOSerializableObject *) trackDgE_0));
    assert_string_equal(Composable_name(flattenedStack_1), "Z");
    RationalTime start = RationalTime_create(50, 24);
    RationalTime duration = RationalTime_create(50, 24);
    TimeRange tr =
            TimeRange_create_with_start_time_and_duration(start, duration);
    OptionalTimeRange flattenedStack_1_source_range =
            Clip_source_range((Clip *) flattenedStack_1);
    assert_true(OptionalTimeRange_valid(flattenedStack_1_source_range));
    assert_true(TimeRange_equal(tr, OptionalTimeRange_value(flattenedStack_1_source_range)));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_2,
            (OTIOSerializableObject *) trackDgE_2));

    OTIO_RELEASE(stack);
    stack = NULL;
    OTIO_RELEASE(flattenedStack);
    flattenedStack = NULL;

    stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
//    OTIO_RETAIN(stack); TODO: fix error: redefinition of ‘stack_r’
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackZ, errorStatus));
    assert_true(Stack_insert_child(stack, 1, (Composable *) trackgFg, errorStatus));
    flattenedStack = flatten_stack(stack, errorStatus);
//    OTIO_RETAIN(flattenedStack); TODO: fix error: redefinition of ‘flattenedStack_r’
    Track_set_name(flattenedStack, "Sequence1");

    flattenedStackChildren = Track_children(flattenedStack);
    flattenedStackIt = ComposableRetainerVector_begin(flattenedStackChildren);
    flattenedStack_0_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_0 = RetainerComposable_value(flattenedStack_0_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    flattenedStack_1_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_1 = RetainerComposable_value(flattenedStack_1_retainer);
    ComposableRetainerVectorIterator_advance(flattenedStackIt, 1);
    flattenedStack_2_retainer =
            ComposableRetainerVectorIterator_value(flattenedStackIt);
    flattenedStack_2 = RetainerComposable_value(flattenedStack_2_retainer);
    ComposableRetainerVector_destroy(flattenedStackChildren);
    flattenedStackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flattenedStackIt);
    flattenedStackIt = NULL;

    ComposableRetainerVector *trackgFgChildren = Track_children(trackgFg);
    ComposableRetainerVectorIterator *trackgFgIt =
            ComposableRetainerVector_begin(trackgFgChildren);
    ComposableRetainerVectorIterator_advance(trackgFgIt, 1);
    RetainerComposable *trackgFg_1_retainer =
            ComposableRetainerVectorIterator_value(trackgFgIt);
    Composable *trackgFg_1 = RetainerComposable_value(trackgFg_1_retainer);
    ComposableRetainerVector_destroy(trackDgEChildren);
    trackDgEChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackDgEIt);
    trackDgEIt = NULL;

    assert_string_equal(Composable_name(flattenedStack_0), "Z");
    start = RationalTime_create(0, 24);
    duration = RationalTime_create(50, 24);
    tr = TimeRange_create_with_start_time_and_duration(start, duration);
    OptionalTimeRange flattenedStack_0_source_range =
            Clip_source_range((Clip *) flattenedStack_0);
    assert_true(OptionalTimeRange_valid(flattenedStack_0_source_range));
    assert_true(TimeRange_equal(tr, OptionalTimeRange_value(flattenedStack_0_source_range)));

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flattenedStack_1,
            (OTIOSerializableObject *) trackgFg_1));
    assert_string_equal(Composable_name(flattenedStack_2), "Z");
    start = RationalTime_create(100, 24);
    tr = TimeRange_create_with_start_time_and_duration(start, duration);
    OptionalTimeRange flattenedStack_2_source_range =
            Clip_source_range((Clip *) flattenedStack_2);
    assert_true(OptionalTimeRange_valid(flattenedStack_2_source_range));
    assert_true(TimeRange_equal(tr, OptionalTimeRange_value(flattenedStack_2_source_range)));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
//    OTIO_RELEASE(stack);
//    stack = NULL;
//    OTIO_RELEASE(flattenedStack);
//    flattenedStack = NULL;
}

static void otio_stack_algo_flatten_vector_of_tracks_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    TrackVector *tracks = TrackVector_create();
    TrackVector_push_back(tracks, trackABC);
    TrackVector_push_back(tracks, trackDgE);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Track *flatTrack = flatten_stack_track_vector(tracks, errorStatus);
    OTIO_RETAIN(flatTrack);

    ComposableRetainerVector *trackABCChildren = Track_children(trackABC);
    ComposableRetainerVectorIterator *trackABCIt =
            ComposableRetainerVector_begin(trackABCChildren);
    RetainerComposable *trackABC_0_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_0 = RetainerComposable_value(trackABC_0_retainer);
    ComposableRetainerVectorIterator_advance(trackABCIt, 1);
    RetainerComposable *trackABC_1_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_1 = RetainerComposable_value(trackABC_1_retainer);
    ComposableRetainerVectorIterator_advance(trackABCIt, 1);
    RetainerComposable *trackABC_2_retainer =
            ComposableRetainerVectorIterator_value(trackABCIt);
    Composable *trackABC_2 = RetainerComposable_value(trackABC_2_retainer);
    ComposableRetainerVector_destroy(trackABCChildren);
    trackABCChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackABCIt);
    trackABCIt = NULL;

    ComposableRetainerVector *trackDgEChildren = Track_children(trackDgE);
    ComposableRetainerVectorIterator *trackDgEIt =
            ComposableRetainerVector_begin(trackDgEChildren);
    RetainerComposable *trackDgE_0_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_0 = RetainerComposable_value(trackDgE_0_retainer);
    ComposableRetainerVectorIterator_advance(trackDgEIt, 2);
    RetainerComposable *trackDgE_2_retainer =
            ComposableRetainerVectorIterator_value(trackDgEIt);
    Composable *trackDgE_2 = RetainerComposable_value(trackDgE_2_retainer);
    ComposableRetainerVector_destroy(trackDgEChildren);
    trackDgEChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackDgEIt);
    trackDgEIt = NULL;

    ComposableRetainerVector *trackgFgChildren = Track_children(trackgFg);
    ComposableRetainerVectorIterator *trackgFgIt =
            ComposableRetainerVector_begin(trackgFgChildren);
    ComposableRetainerVectorIterator_advance(trackgFgIt, 1);
    RetainerComposable *trackgFg_1_retainer =
            ComposableRetainerVectorIterator_value(trackgFgIt);
    Composable *trackgFg_1 = RetainerComposable_value(trackgFg_1_retainer);
    ComposableRetainerVector_destroy(trackDgEChildren);
    trackDgEChildren = NULL;
    ComposableRetainerVectorIterator_destroy(trackDgEIt);
    trackDgEIt = NULL;

    ComposableRetainerVector *flatTrackChildren = Track_children(flatTrack);
    ComposableRetainerVectorIterator *flatTrackIt =
            ComposableRetainerVector_begin(flatTrackChildren);
    RetainerComposable *flatTrack_0_retainer =
            ComposableRetainerVectorIterator_value(flatTrackIt);
    Composable *flatTrack_0 = RetainerComposable_value(flatTrack_0_retainer);
    ComposableRetainerVectorIterator_advance(flatTrackIt, 1);
    RetainerComposable *flatTrack_1_retainer =
            ComposableRetainerVectorIterator_value(flatTrackIt);
    Composable *flatTrack_1 = RetainerComposable_value(flatTrack_1_retainer);
    ComposableRetainerVectorIterator_advance(flatTrackIt, 1);
    RetainerComposable *flatTrack_2_retainer =
            ComposableRetainerVectorIterator_value(flatTrackIt);
    Composable *flatTrack_2 = RetainerComposable_value(flatTrack_2_retainer);
    ComposableRetainerVector_destroy(flatTrackChildren);
    flatTrackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flatTrackIt);
    flatTrackIt = NULL;

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_0,
            (OTIOSerializableObject *) trackDgE_0));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_1,
            (OTIOSerializableObject *) trackABC_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_2,
            (OTIOSerializableObject *) trackDgE_2));

    TrackVector_destroy(tracks);
    tracks = NULL;
    OTIO_RELEASE(flatTrack);
    flatTrack = NULL;

    tracks = TrackVector_create();
    TrackVector_push_back(tracks, trackABC);
    TrackVector_push_back(tracks, trackgFg);

    flatTrack = flatten_stack_track_vector(tracks, errorStatus);
//    OTIO_RETAIN(flatTrack); TODO: fix error: redefinition of ‘flattenedTrack_r’

    flatTrackChildren = Track_children(flatTrack);
    flatTrackIt = ComposableRetainerVector_begin(flatTrackChildren);
    flatTrack_0_retainer = ComposableRetainerVectorIterator_value(flatTrackIt);
    flatTrack_0 = RetainerComposable_value(flatTrack_0_retainer);
    ComposableRetainerVectorIterator_advance(flatTrackIt, 1);
    flatTrack_1_retainer = ComposableRetainerVectorIterator_value(flatTrackIt);
    flatTrack_1 = RetainerComposable_value(flatTrack_1_retainer);
    ComposableRetainerVectorIterator_advance(flatTrackIt, 1);
    flatTrack_2_retainer = ComposableRetainerVectorIterator_value(flatTrackIt);
    flatTrack_2 = RetainerComposable_value(flatTrack_2_retainer);
    ComposableRetainerVector_destroy(flatTrackChildren);
    flatTrackChildren = NULL;
    ComposableRetainerVectorIterator_destroy(flatTrackIt);
    flatTrackIt = NULL;

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_0,
            (OTIOSerializableObject *) trackABC_0));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_1,
            (OTIOSerializableObject *) trackgFg_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) flatTrack_2,
            (OTIOSerializableObject *) trackABC_2));

    TrackVector_destroy(tracks);
    tracks = NULL;
//    OTIO_RELEASE(flatTrack);
//    flatTrack = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_stack_algo_flatten_example_code_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    const char *multitrack_file = "multitrack.otio";
    const size_t multitrack_path_size =
        strlen(sample_data_dir) + strlen(multitrack_file) + 1;
    char *multitrack_path = (char *) calloc(multitrack_path_size, sizeof(char));
    strcpy_s(multitrack_path, multitrack_path_size, sample_data_dir);
    strcat_s(multitrack_path, multitrack_path_size, multitrack_file);

    const char *preflattened_file = "preflattened.otio";
    const size_t preflattened_path_size =
        strlen(sample_data_dir) + strlen(multitrack_file) + 1;
    char *preflattened_path = (char *) calloc(preflattened_path_size, sizeof(char));
    strcpy_s(preflattened_path, preflattened_path_size, sample_data_dir);
    strcat_s(preflattened_path, preflattened_path_size, preflattened_file);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Any *tlAny = create_safely_typed_any_serializable_object(
            SerializableObject_create());
    assert_true(deserialize_json_from_file(multitrack_path, tlAny, errorStatus));
    Timeline *timeline = (Timeline *) safely_cast_retainer_any(tlAny);

    Any *preflattenedAny = create_safely_typed_any_serializable_object(
            SerializableObject_create());
    assert_true(deserialize_json_from_file(
            preflattened_path, preflattenedAny, errorStatus));
    Timeline *preflattened =
            (Timeline *) safely_cast_retainer_any(preflattenedAny);

    Stack *preflattened_stack = Timeline_tracks(preflattened);
    ComposableRetainerVector *preflattened_tracks_vector =
            Stack_children(preflattened_stack);
    ComposableRetainerVectorIterator *preflattened_tracks_vector_it =
            ComposableRetainerVector_begin(preflattened_tracks_vector);
    RetainerComposable *preflattened_tracks_vector_0 =
            ComposableRetainerVectorIterator_value(preflattened_tracks_vector_it);
    Track *preflattened_track =
            (Track *) RetainerComposable_value(preflattened_tracks_vector_0);
    ComposableRetainerVector_destroy(preflattened_tracks_vector);
    preflattened_tracks_vector = NULL;
    ComposableRetainerVectorIterator_destroy(preflattened_tracks_vector_it);
    preflattened_tracks_vector_it = NULL;

    TrackVector *timeline_video_tracks = Timeline_video_tracks(timeline);
    Track *flattened_track =
            flatten_stack_track_vector(timeline_video_tracks, errorStatus);

    Track_set_name(preflattened_track, "");
    Track_set_name(flattened_track, "");

    assert_true(Track_is_equivalent_to(
            preflattened_track, (OTIOSerializableObject *) flattened_track));

    TrackVector_destroy(timeline_video_tracks);
    timeline_video_tracks = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_stack_algo_flatten_with_transition_test(void **state) {
    struct StackAlgoTestState *testState = *state;
    const char *sample_data_dir = testState->sample_data_dir;
    Track *trackZ = testState->trackZ;
    Track *trackABC = testState->trackABC;
    Track *trackDgE = testState->trackDgE;
    Track *trackgFg = testState->trackgFg;
    RetainerSerializableObject *trackZ_r = testState->trackZ_r;
    RetainerSerializableObject *trackABC_r = testState->trackABC_r;
    RetainerSerializableObject *trackDgE_r = testState->trackDgE_r;
    RetainerSerializableObject *trackgFg_r = testState->trackgFg_r;
    const char *trackZStr = testState->trackZStr;
    const char *trackABCStr = testState->trackABCStr;
    const char *trackDgEStr = testState->trackDgEStr;
    const char *trackgFgStr = testState->trackgFgStr;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    OptionalRationalTime in_offset = OptionalRationalTime_create(RationalTime_create(10, 24));
    OptionalRationalTime out_offset = OptionalRationalTime_create(RationalTime_create(15, 24));

    Transition *transition =
            Transition_create("test_transition", NULL, in_offset, out_offset, NULL);
    OTIO_RETAIN(transition);

    assert_true(Track_insert_child(trackDgE, 1, (Composable *) transition, errorStatus));


    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(stack);
    assert_true(Stack_insert_child(stack, 0, (Composable *) trackABC, errorStatus));
    assert_true(Stack_insert_child(stack, 2, (Composable *) trackDgE, errorStatus));

    Track *flat_track = flatten_stack(stack, errorStatus);

    ComposableRetainerVector *trackABC_childen = Track_children(trackABC);
    assert_int_equal(ComposableRetainerVector_size(trackABC_childen), 3);
    ComposableRetainerVector_destroy(trackABC_childen);
    trackABC_childen = NULL;

    ComposableRetainerVector *trackDgE_childen = Track_children(trackDgE);
    assert_int_equal(ComposableRetainerVector_size(trackDgE_childen), 4);
    ComposableRetainerVector_destroy(trackDgE_childen);
    trackDgE_childen = NULL;

    ComposableRetainerVector *flat_track_childen = Track_children(flat_track);
    assert_int_equal(ComposableRetainerVector_size(flat_track_childen), 4);

    ComposableRetainerVectorIterator *flat_track_it =
            ComposableRetainerVector_begin(flat_track_childen);
    ComposableRetainerVectorIterator_advance(flat_track_it, 1);
    RetainerComposable *flat_track_1_retainer =
            ComposableRetainerVectorIterator_value(flat_track_it);
    Composable *flat_track_1 = RetainerComposable_value(flat_track_1_retainer);
    assert_string_equal(Composable_name(flat_track_1), "test_transition");

    ComposableRetainerVectorIterator_destroy(flat_track_it);
    flat_track_1 = NULL;
    ComposableRetainerVector_destroy(flat_track_childen);
    flat_track_childen = NULL;
    OTIO_RELEASE(transition);
    transition = NULL;
    OTIO_RELEASE(stack);
    stack = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_single_track_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_obscured_track_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_gaps_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_gaps_with_trims_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_vector_of_tracks_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_example_code_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_stack_algo_flatten_with_transition_test, setupStackAlgoTests,
                                            teardownStackAlgoTests),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
