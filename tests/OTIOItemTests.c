// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
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
#include <copentimelineio/timeline.h>
#include <copentimelineio/track.h>
#include <copentimelineio/transition.h>

static void otio_gap_serialize_test(void **state) {
    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Gap *gap = Gap_create_with_duration(nullTime, NULL, NULL, NULL, NULL);
    OTIO_RETAIN(gap);
    Any *gap_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) gap);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(gap_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) gap);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) gap, decoded_object));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    OTIO_RELEASE(gap);
    gap = NULL;
    otiostr_delete(encoded);
}

static void otio_item_constructor_test(void **state) {
    RationalTime start = RationalTime_create(0, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));

    Item *it = Item_create("foo", tr, NULL, NULL, NULL);
    OTIO_RETAIN(it);

    otiostr serializableObjectName = Item_name(it);
    assert_string_equal(serializableObjectName, "foo");
    otiostr_delete(serializableObjectName);

    OptionalTimeRange it_source_range = Item_source_range(it);
    assert_true(OptionalTimeRange_valid(it_source_range));
    assert_true(TimeRange_equal(OptionalTimeRange_value(it_source_range), OptionalTimeRange_value(tr)));

    Any *item_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) it);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(item_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) it);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) it, decoded_object));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    OTIO_RELEASE(it);
    it = NULL;
    otiostr_delete(encoded);
}

static void otio_item_copy_arguments_test(void **state) {
    RationalTime start = RationalTime_create(0, 24);
    RationalTime duration = RationalTime_create(10, 24);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));

    const char *it_name = "foobar";
    MarkerVector *markerVector = MarkerVector_create();
    AnyDictionary *metadata = AnyDictionary_create();
    Item *it = Item_create(it_name, tr, metadata, NULL, markerVector);
    OTIO_RETAIN(it);

    it_name = "foobaz";
    otiostr serializableObjectName = Item_name(it);
    assert_string_not_equal(serializableObjectName, it_name);
    otiostr_delete(serializableObjectName);

    RationalTime start2 = RationalTime_create(1, RationalTime_rate(start));

    tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start2, duration));

    OptionalTimeRange it_source_range = Item_source_range(it);
    assert_true(OptionalTimeRange_valid(it_source_range));
    assert_false(TimeRange_equal(OptionalTimeRange_value(it_source_range), OptionalTimeRange_value(tr)));

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Marker *marker = Marker_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(marker);
    MarkerVector_push_back(markerVector, marker);
    MarkerRetainerVector *markerRetainerVector = Item_markers(it);
    assert_int_not_equal(MarkerRetainerVector_size(markerRetainerVector),
                         MarkerVector_size(markerVector));
    MarkerRetainerVector_destroy(markerRetainerVector);
    markerRetainerVector = NULL;
    OTIO_RELEASE(marker);
    marker = NULL;

    Any *stringAny = create_safely_typed_any_string("bar");
    AnyDictionary_insert(metadata, "foo", stringAny);
    AnyDictionary *metadataResult = Item_metadata(it);
    assert_int_not_equal(AnyDictionary_size(metadata), AnyDictionary_size(metadataResult));
    AnyDictionary_destroy(metadataResult);
    metadataResult = NULL;
    Any_destroy(stringAny);
    stringAny = NULL;

    MarkerVector_destroy(markerVector);
    markerVector = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    OTIO_RELEASE(it);
    it = NULL;
}

static void otio_item_copy_duration_test(void **state) {
    RationalTime start = RationalTime_create(0, 24);
    RationalTime duration = RationalTime_create(10, 24);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));
    Item *it = Item_create(NULL, tr, NULL, NULL, NULL);
    OTIO_RETAIN(it);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    RationalTime it_duration = Item_duration(it, errorStatus);

    assert_true(RationalTime_equal(duration, it_duration));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(it);
    it = NULL;
}

static void otio_item_copy_available_range_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(it);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    TimeRange available_range = Item_available_range(it, errorStatus);

    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 1);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(it);
    it = NULL;
}

static void otio_item_copy_duration_and_source_range_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(it);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    RationalTime it_duration = Item_duration(it, errorStatus);

    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 1);

    RationalTime start = RationalTime_create(1, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));
    Item *it2 = Item_create(NULL, tr, NULL, NULL, NULL);
    OTIO_RETAIN(it2);

    OptionalTimeRange it2_source_range = Item_source_range(it2);
    assert_true(OptionalTimeRange_valid(it2_source_range));
    assert_true(TimeRange_equal(OptionalTimeRange_value(it2_source_range), OptionalTimeRange_value(tr)));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    RationalTime it2_duration = Item_duration(it2, errorStatus);
    assert_true(RationalTime_equal(it2_duration, duration));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(it);
    it = NULL;
    OTIO_RELEASE(it2);
    it2 = NULL;
}

static void otio_item_copy_trimmed_range_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(it);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    TimeRange it_trimmed_range = Item_trimmed_range(it, errorStatus);

    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 1);

    RationalTime start = RationalTime_create(1, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));
    Item *it2 = Item_create(NULL, tr, NULL, NULL, NULL);
    OTIO_RETAIN(it2);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    TimeRange it2_trimmed_range = Item_trimmed_range(it2, errorStatus);

    assert_true(TimeRange_equal(it2_trimmed_range, OptionalTimeRange_value(tr)));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(it);
    it = NULL;
    OTIO_RELEASE(it2);
    it2 = NULL;
}

static void otio_item_metadata_test_test(void **state) {
    RationalTime start = RationalTime_create(1, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));

    AnyDictionary *metadata = AnyDictionary_create();

    Any *stringAny = create_safely_typed_any_string("bar");

    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", stringAny);

    Item *item = Item_create(NULL, tr, metadata, NULL, NULL);
    OTIO_RETAIN(item);

    Any *item_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(item_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) item, decoded_object));

    AnyDictionary *metadataResult = Item_metadata((Item *) decoded_object);
    AnyDictionaryIterator *metadataResultFooIt =
            AnyDictionary_find(metadataResult, "foo");
    Any *metadataFooValue = AnyDictionaryIterator_value(metadataResultFooIt);
    otiostr metadataFooStr = safely_cast_string_any(metadataFooValue);

    assert_string_equal(metadataFooStr, "bar");

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    AnyDictionary_destroy(metadataResult);
    metadataResult = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    AnyDictionaryIterator_destroy(metadataResultFooIt);
    metadataResultFooIt = NULL;
    Any_destroy(metadataFooValue);
    metadataFooValue = NULL;
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    Any_destroy(stringAny);
    stringAny = NULL;
    OTIO_RELEASE(item);
    item = NULL;
    otiostr_delete(encoded);
    otiostr_delete(metadataFooStr);
}

static void otio_item_markers_test(void **state) {
    RationalTime start = RationalTime_create(1, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr =
            OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));

    Marker *marker = Marker_create("test_marker", tr, NULL, NULL);
    MarkerVector *markerVector = MarkerVector_create();
    MarkerVector_push_back(markerVector, marker);

    Item *item = Item_create(NULL, tr, NULL, NULL, markerVector);
    OTIO_RETAIN(item);

    Any *item_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(item_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) item, decoded_object));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    OTIO_RELEASE(item);
    item = NULL;
    MarkerVector_destroy(markerVector);
    markerVector = NULL;
    otiostr_delete(encoded);
}

static void otio_item_effects_test(void **state) {
    RationalTime start = RationalTime_create(1, 1);
    RationalTime duration = RationalTime_create(10, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start, duration));

    Effect *effect = Effect_create(NULL, "blur", NULL);
    EffectVector *effectVector = EffectVector_create();
    EffectVector_push_back(effectVector, effect);

    Item *item = Item_create(NULL, tr, NULL, effectVector, NULL);
    OTIO_RETAIN(item);

    Any *item_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(item_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) item);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) item, decoded_object));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    OTIO_RELEASE(item);
    item = NULL;
    EffectVector_destroy(effectVector);
    effectVector = NULL;
    otiostr_delete(encoded);
}

static void otio_item_visible_range_test(void **state) {
    RationalTime start = RationalTime_create(1, 30);
    RationalTime duration = RationalTime_create(50, 30);
    OptionalTimeRange source_rangeA = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    Clip *A = Clip_create("A", NULL, source_rangeA, NULL);

    start = RationalTime_create(100, 30);
    duration = RationalTime_create(50, 30);
    OptionalTimeRange source_rangeB = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    Clip *B = Clip_create("B", NULL, source_rangeB, NULL);

    start = RationalTime_create(50, 30);
    duration = RationalTime_create(50, 30);
    OptionalTimeRange source_rangeC = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    Clip *C = Clip_create("C", NULL, source_rangeC, NULL);

    start = RationalTime_create(1, 30);
    duration = RationalTime_create(50, 30);
    OptionalTimeRange source_rangeD = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    Clip *D = Clip_create("D", NULL, source_rangeD, NULL);

    OptionalRationalTime in_offset = OptionalRationalTime_create(RationalTime_create(7, 30));
    OptionalRationalTime out_offset = OptionalRationalTime_create(RationalTime_create(10, 30));
    Transition *transition1 =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);

    in_offset = OptionalRationalTime_create(RationalTime_create(17, 30));
    out_offset = OptionalRationalTime_create(RationalTime_create(15, 30));
    Transition *transition2 =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *track = Track_create(NULL, nullRange, NULL, NULL);
    assert_true(Track_insert_child(track, 0, (Composable *) A, errorStatus));
    assert_true(Track_insert_child(track, 1, (Composable *) transition1, errorStatus));
    assert_true(Track_insert_child(track, 2, (Composable *) B, errorStatus));
    assert_true(Track_insert_child(track, 3, (Composable *) transition2, errorStatus));
    assert_true(Track_insert_child(track, 4, (Composable *) C, errorStatus));
    assert_true(Track_insert_child(track, 5, (Composable *) D, errorStatus));

    Stack *stack = Stack_create(NULL, nullRange, NULL, NULL, NULL);
    assert_true(Stack_insert_child(stack, 0, (Composable *) track, errorStatus));

    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIO_RETAIN(timeline);
    Timeline_set_tracks(timeline, stack);

    const char *name_list[] = {"A", "B", "C", "D"};
    OptionalTimeRange trimmed_ranges[] = {
            source_rangeA, source_rangeB, source_rangeC, source_rangeD
    };

    start = RationalTime_create(1, 30);
    duration = RationalTime_create(50 + 10, 30);
    TimeRange visible_range_1 =
            TimeRange_create_with_start_time_and_duration(start, duration);

    start = RationalTime_create(100 - 7, 30);
    duration = RationalTime_create(50 + 15 + 7, 30);
    TimeRange visible_range_2 =
            TimeRange_create_with_start_time_and_duration(start, duration);

    start = RationalTime_create(33, 30);
    duration = RationalTime_create(50 + 17, 30);
    TimeRange visible_range_3 =
            TimeRange_create_with_start_time_and_duration(start, duration);

    start = RationalTime_create(1, 30);
    duration = RationalTime_create(50, 30);
    TimeRange visible_range_4 =
            TimeRange_create_with_start_time_and_duration(start, duration);
    TimeRange visible_ranges[] = {
            visible_range_1, visible_range_2, visible_range_3, visible_range_4
    };

    ComposableVector *clipVector = Track_each_clip(track);
    ComposableVectorIterator *it = ComposableVector_begin(clipVector);
    ComposableVectorIterator *itEnd = ComposableVector_end(clipVector);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;

    for (int i = 0; ComposableVectorIterator_not_equal(it, itEnd);
         ComposableVectorIterator_advance(it, 1), i++) {
        Composable *clipComposable = ComposableVectorIterator_value(it);
        Clip *clipValue = (Clip *) clipComposable;
        otiostr serializableObjectName = Clip_name(clipValue);
        assert_string_equal(serializableObjectName, name_list[i]);
        otiostr_delete(serializableObjectName);
        errorStatus = OTIOErrorStatus_create();

        TimeRange clip_trimmed_range =
                Clip_trimmed_range(clipValue, errorStatus);
        assert_true(TimeRange_equal(clip_trimmed_range, OptionalTimeRange_value(trimmed_ranges[i])));

        TimeRange clip_visible_range =
                Clip_visible_range(clipValue, errorStatus);
        assert_true(TimeRange_equal(clip_visible_range, visible_ranges[i]));

        OTIOErrorStatus_destroy(errorStatus);
    }

    ComposableVectorIterator_destroy(it);
    it = NULL;
    ComposableVectorIterator_destroy(itEnd);
    itEnd = NULL;
    ComposableVector_destroy(clipVector);
    clipVector = NULL;
    OTIO_RELEASE(timeline);
    timeline = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_gap_serialize_test),
            cmocka_unit_test(otio_item_constructor_test),
            cmocka_unit_test(otio_item_copy_arguments_test),
            cmocka_unit_test(otio_item_copy_duration_test),
            cmocka_unit_test(otio_item_copy_available_range_test),
            cmocka_unit_test(otio_item_copy_duration_and_source_range_test),
            cmocka_unit_test(otio_item_copy_trimmed_range_test),
            cmocka_unit_test(otio_item_metadata_test_test),
            cmocka_unit_test(otio_item_markers_test),
            cmocka_unit_test(otio_item_effects_test),
            cmocka_unit_test(otio_item_visible_range_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
