#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>

#include <copentimelineio/clip.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/externalReference.h>
#include <copentimelineio/gap.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableCollection.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>
#include <copentimelineio/timeline.h>
#include <copentimelineio/track.h>
#include <copentimelineio/trackAlgorithm.h>
#include <copentimelineio/transition.h>

struct TrackAlgoTestState {
    Track *sample_track;
    RetainerSerializableObject *sample_track_r;
    const char *sample_track_str;
};

static int setupTrackAlgoTests(void **state) {
    struct TrackAlgoTestState *testState = malloc(sizeof(struct TrackAlgoTestState));

    testState->sample_track_str =
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

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    /* sample_track */
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    bool decoded_successfully = deserialize_json_from_string(
            testState->sample_track_str, decoded, errorStatus);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    testState->sample_track = (Track *) decoded_object;
    testState->sample_track_r = RetainerSerializableObject_create(decoded_object);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;

    *state = testState;

    return 0;
}

static int teardownTrackAlgoTests(void **state) {
    struct TrackAlgoTestState *testState = *state;

    if (testState->sample_track_r) {
        RetainerSerializableObject_managed_destroy(testState->sample_track_r);
        testState->sample_track = NULL;
        testState->sample_track_r = NULL;
    }

    free(testState);

    return 0;
}

static void otio_track_algo_trim_to_existing_range_test(void **state) {
    struct TrackAlgoTestState *testState = *state;
    Track *sample_track = testState->sample_track;
    RetainerSerializableObject *sample_track_r = testState->sample_track_r;
    const char *sample_track_str = testState->sample_track_str;

    RationalTime start = RationalTime_create(0, 24);
    RationalTime duration = RationalTime_create(150, 24);
    TimeRange trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    TimeRange sample_track_trimmed_range =
            Track_trimmed_range(sample_track, errorStatus);
    assert_true(TimeRange_equal(trimmed_range, sample_track_trimmed_range));

    OTIOSerializableObject *trimmed_track = (OTIOSerializableObject *) (
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus));
    RetainerSerializableObject *trimmed_track_r = RetainerSerializableObject_create(trimmed_track);

    /* It shouldn't have changed at all */
    assert_true(Track_is_equivalent_to(sample_track, trimmed_track));
}

static void otio_track_algo_trim_to_longer_range_test(void **state) {
    struct TrackAlgoTestState *testState = *state;
    Track *sample_track = testState->sample_track;
    RetainerSerializableObject *sample_track_r = testState->sample_track_r;
    const char *sample_track_str = testState->sample_track_str;

    RationalTime start = RationalTime_create(-10, 24);
    RationalTime duration = RationalTime_create(160, 24);
    TimeRange trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Track *trimmed_track = track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    RetainerSerializableObject *trimmed_track_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) trimmed_track);

    /* It shouldn't have changed at all */
    assert_true(Track_is_equivalent_to(
            sample_track, (OTIOSerializableObject *) trimmed_track));
}

static void otio_track_algo_trim_front_test(void **state) {
    struct TrackAlgoTestState *testState = *state;
    Track *sample_track = testState->sample_track;
    RetainerSerializableObject *sample_track_r = testState->sample_track_r;
    const char *sample_track_str = testState->sample_track_str;

    RationalTime start = RationalTime_create(60, 24);
    RationalTime duration = RationalTime_create(90, 24);
    TimeRange trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    /* trim off the front (clip A and part of B) */
    Track *trimmed_track =
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    RetainerSerializableObject *trimmed_track_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) trimmed_track);

    ComposableRetainerVector *trimmed_track_children =
            Track_children(trimmed_track);
    assert_int_equal(ComposableRetainerVector_size(trimmed_track_children), 2);

    TimeRange trimmed_track_trimmed_range =
            Track_trimmed_range(trimmed_track, errorStatus);

    start = RationalTime_create(0, 24);
    duration = RationalTime_create(90, 24);
    TimeRange trimmed_track_trimmed_range_compare =
            TimeRange_create_with_start_time_and_duration(start, duration);
    assert_true(TimeRange_equal(
            trimmed_track_trimmed_range, trimmed_track_trimmed_range_compare));

    /* did clip B get trimmed? */
    ComposableRetainerVectorIterator *it =
            ComposableRetainerVector_begin(trimmed_track_children);
    RetainerComposable *clipB_retainer =
            ComposableRetainerVectorIterator_value(it);
    Clip *B = (Clip *) RetainerComposable_value(clipB_retainer);
    ComposableRetainerVectorIterator_advance(it, 1);
    RetainerComposable *clipC_retainer =
            ComposableRetainerVectorIterator_value(it);
    Clip *C = (Clip *) RetainerComposable_value(clipC_retainer);
    otiostr serializableObjectName = Clip_name(B);
    assert_string_equal(serializableObjectName, "B");
    otiostr_delete(serializableObjectName);
    TimeRange clipB_trimmed_range = Clip_trimmed_range(B, errorStatus);
    start = RationalTime_create(10, 24);
    duration = RationalTime_create(40, 24);
    TimeRange clipB_trimmed_range_compare =
            TimeRange_create_with_start_time_and_duration(start, duration);
    assert_true(TimeRange_equal(clipB_trimmed_range, clipB_trimmed_range_compare));
    ComposableRetainerVectorIterator_destroy(it);
    it = NULL;

    ComposableRetainerVector *sample_track_children =
            Track_children(sample_track);
    ComposableRetainerVectorIterator *sample_track_it =
            ComposableRetainerVector_begin(sample_track_children);
    ComposableRetainerVectorIterator_advance(sample_track_it, 2);
    RetainerComposable *original_clipC_retainer =
            ComposableRetainerVectorIterator_value(sample_track_it);
    Clip *original_ClipC =
            (Clip *) RetainerComposable_value(original_clipC_retainer);

    /* clip C should have been left alone */
    assert_true(Clip_is_equivalent_to(C, (OTIOSerializableObject *) original_ClipC));

    ComposableRetainerVectorIterator_destroy(sample_track_it);
    sample_track_it = NULL;
    ComposableRetainerVector_destroy(sample_track_children);
    sample_track_children = NULL;

    assert_false(Track_is_equivalent_to(
            sample_track, (OTIOSerializableObject *) trimmed_track));

    ComposableRetainerVector_destroy(trimmed_track_children);
    trimmed_track_children = NULL;
    RetainerSerializableObject_managed_destroy(trimmed_track_r);
}

static void otio_track_algo_trim_end_test(void **state) {
    struct TrackAlgoTestState *testState = *state;
    Track *sample_track = testState->sample_track;
    RetainerSerializableObject *sample_track_r = testState->sample_track_r;
    const char *sample_track_str = testState->sample_track_str;

    RationalTime start = RationalTime_create(0, 24);
    RationalTime duration = RationalTime_create(90, 24);
    TimeRange trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    /* trim off the front (clip C and part of B) */
    Track *trimmed_track =
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    RetainerSerializableObject *trimmed_track_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) trimmed_track);

    ComposableRetainerVector *trimmed_track_children =
            Track_children(trimmed_track);
    assert_int_equal(ComposableRetainerVector_size(trimmed_track_children), 2);

    TimeRange trimmed_track_trimmed_range =
            Track_trimmed_range(trimmed_track, errorStatus);

    start = RationalTime_create(0, 24);
    duration = RationalTime_create(90, 24);
    TimeRange trimmed_track_trimmed_range_compare =
            TimeRange_create_with_start_time_and_duration(start, duration);
    assert_true(TimeRange_equal(
            trimmed_track_trimmed_range, trimmed_track_trimmed_range_compare));

    ComposableRetainerVectorIterator *it =
            ComposableRetainerVector_begin(trimmed_track_children);
    RetainerComposable *trimmed_0_retainer =
            ComposableRetainerVectorIterator_value(it);
    Composable *trimmed_0 = RetainerComposable_value(trimmed_0_retainer);
    ComposableRetainerVectorIterator_advance(it, 1);
    RetainerComposable *trimmed_1_retainer =
            ComposableRetainerVectorIterator_value(it);
    Composable *trimmed_1 = RetainerComposable_value(trimmed_1_retainer);
    ComposableRetainerVectorIterator_destroy(it);
    it = NULL;
    ComposableRetainerVector_destroy(trimmed_track_children);
    trimmed_track_children = NULL;

    ComposableRetainerVector *sample_track_children =
            Track_children(sample_track);
    ComposableRetainerVectorIterator *sample_track_it =
            ComposableRetainerVector_begin(sample_track_children);
    RetainerComposable *sample_track_0_retainer =
            ComposableRetainerVectorIterator_value(sample_track_it);
    Composable *sample_track_0 =
            RetainerComposable_value(sample_track_0_retainer);
    ComposableRetainerVectorIterator_destroy(sample_track_it);
    sample_track_it = NULL;
    ComposableRetainerVector_destroy(sample_track_children);
    sample_track_children = NULL;

    /* clip A should have been left alone */
    assert_true(Composable_is_equivalent_to(
            trimmed_0, (OTIOSerializableObject *) sample_track_0));
    /* did clip B get trimmed? */
    otiostr serializableObjectName = Composable_name(trimmed_1);
    assert_string_equal(serializableObjectName, "B");
    otiostr_delete(serializableObjectName);

    TimeRange trimmed_1_track_trimmed_range =
            Clip_trimmed_range((Clip *) trimmed_1, errorStatus);
    start = RationalTime_create(0, 24);
    duration = RationalTime_create(40, 24);
    TimeRange trimmed_1_track_trimmed_range_compare =
            TimeRange_create_with_start_time_and_duration(start, duration);
    assert_true(TimeRange_equal(
            trimmed_1_track_trimmed_range, trimmed_1_track_trimmed_range_compare));

    assert_false(Track_is_equivalent_to(
            sample_track, (OTIOSerializableObject *) (trimmed_track)));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_track_algo_trim_with_transition_test(void **state) {
    struct TrackAlgoTestState *testState = *state;
    Track *sample_track = testState->sample_track;
    RetainerSerializableObject *sample_track_r = testState->sample_track_r;
    const char *sample_track_str = testState->sample_track_str;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    RationalTime sample_track_duration_compare = RationalTime_create(150, 24);
    RationalTime sample_track_duration =
            Track_duration(sample_track, errorStatus);
    assert_true(RationalTime_equal(
            sample_track_duration, sample_track_duration_compare));

    ComposableRetainerVector *sample_track_children =
            Track_children(sample_track);
    assert_int_equal(ComposableRetainerVector_size(sample_track_children), 3);
    ComposableRetainerVector_destroy(sample_track_children);
    sample_track_children = NULL;
    OptionalRationalTime in_offset = OptionalRationalTime_create(RationalTime_create(12, 24));
    OptionalRationalTime out_offset = OptionalRationalTime_create(RationalTime_create(20, 24));
    Transition *transition = Transition_create(NULL, NULL, in_offset, out_offset, NULL);
    RetainerSerializableObject *transition_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) (transition));
    /* add a transition */
    assert_true(Track_insert_child(
            sample_track, 1, (Composable *) transition, errorStatus));
    sample_track_children = Track_children(sample_track);
    assert_int_equal(ComposableRetainerVector_size(sample_track_children), 4);
    ComposableRetainerVector_destroy(sample_track_children);
    sample_track_children = NULL;

    sample_track_duration_compare = RationalTime_create(150, 24);
    sample_track_duration = Track_duration(sample_track, errorStatus);
    assert_true(RationalTime_equal(
            sample_track_duration, sample_track_duration_compare));

    RationalTime start = RationalTime_create(5, 24);
    RationalTime duration = RationalTime_create(50, 24);
    TimeRange trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);
    Track *trimmed_track =
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    assert_ptr_equal(trimmed_track, (Track *) NULL);
    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 23);
    OTIOErrorStatus_destroy(errorStatus);

    errorStatus = OTIOErrorStatus_create();
    start = RationalTime_create(45, 24);
    duration = RationalTime_create(50, 24);
    trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);
    trimmed_track =
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    assert_ptr_equal(trimmed_track, (Track *) NULL);
    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 23);
    OTIOErrorStatus_destroy(errorStatus);

    errorStatus = OTIOErrorStatus_create();
    start = RationalTime_create(25, 24);
    duration = RationalTime_create(50, 24);
    trimmed_range =
            TimeRange_create_with_start_time_and_duration(start, duration);
    trimmed_track =
            track_trimmed_to_range(sample_track, trimmed_range, errorStatus);
    RetainerSerializableObject *trimmed_track_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) trimmed_track);
    assert_false(Track_is_equivalent_to(
            sample_track, (OTIOSerializableObject *) trimmed_track));

    const char *expected_str =
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
            "                            \"value\": 25\n"
            "                        },\n"
            "                        \"start_time\": {\n"
            "                            \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                            \"rate\": 24,\n"
            "                            \"value\": 25.0\n"
            "                        }\n"
            "                    }\n"
            "                },\n"
            "                {\n"
            "                    \"OTIO_SCHEMA\": \"Transition.1\",\n"
            "                    \"name\": \"\",\n"
            "                    \"metadata\": {},\n"
            "                    \"transition_type\": \"\",\n"
            "                    \"in_offset\": {\n"
            "                        \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                        \"rate\": 24,\n"
            "                        \"value\": 12\n"
            "                    },\n"
            "                    \"out_offset\": {\n"
            "                        \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                        \"rate\": 24,\n"
            "                        \"value\": 20\n"
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
            "                            \"value\": 25\n"
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

    Any *decodedAny = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    SerializableObject_create());
    assert_true(deserialize_json_from_string(expected_str, decodedAny, errorStatus));
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decodedAny);
    RetainerSerializableObject *decoded_object_r = RetainerSerializableObject_create(decoded_object);

    assert_true(Track_is_equivalent_to(trimmed_track, decoded_object));

    RetainerSerializableObject_managed_destroy(decoded_object_r);
    decoded_object = NULL;
    RetainerSerializableObject_managed_destroy(trimmed_track_r);
    trimmed_track = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    RetainerSerializableObject_managed_destroy(transition_r);
    transition = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(otio_track_algo_trim_to_existing_range_test, setupTrackAlgoTests,
                                            teardownTrackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_track_algo_trim_to_longer_range_test, setupTrackAlgoTests,
                                            teardownTrackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_track_algo_trim_front_test, setupTrackAlgoTests,
                                            teardownTrackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_track_algo_trim_end_test, setupTrackAlgoTests,
                                            teardownTrackAlgoTests),
            cmocka_unit_test_setup_teardown(otio_track_algo_trim_with_transition_test, setupTrackAlgoTests,
                                            teardownTrackAlgoTests),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
