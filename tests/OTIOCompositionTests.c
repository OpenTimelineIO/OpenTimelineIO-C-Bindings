#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <string.h>

#include <copentime/rationalTime.h>
#include <copentime/timeRange.h>
#include <copentimelineio/clip.h>
#include <copentimelineio/composable.h>
#include <copentimelineio/composableVector.h>
#include <copentimelineio/composition.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/gap.h>
#include <copentimelineio/item.h>
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>
#include <copentimelineio/stack.h>
#include <copentimelineio/timeline.h>
#include <copentimelineio/track.h>
#include <copentimelineio/transition.h>
#include <stdlib.h>
#include "epsilon.h"

#define xstr(s) str(s)
#define str(s) #s

struct ClipWrapperPair {
    Clip *clip;
    Stack *wrapper;
};

struct ClipWrapperPair _nest(Clip *item, int index) {
    struct ClipWrapperPair clipWrapperPair;
    clipWrapperPair.clip = NULL;
    clipWrapperPair.wrapper = NULL;

    if (item == NULL) return clipWrapperPair;

    Composition *parent = Composable_parent((Composable *) item);

    if (parent == NULL) { return clipWrapperPair; }
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *wrapper = Stack_create(NULL, nullRange, NULL, NULL, NULL);

    Clip *clip = (Clip *) SerializableObject_clone(
            (OTIOSerializableObject *) item, errorStatus);

    /* now put the item inside the wrapper */
    bool appendOK = Composition_append_child(
            (Composition *) wrapper, (Composable *) clip, errorStatus);

    /* swap out the item for the wrapper */
    bool setOK = Composition_set_child(
            (Composition *) parent, index, (Composable *) wrapper, errorStatus);

    clipWrapperPair.clip = clip;
    clipWrapperPair.wrapper = wrapper;
    return clipWrapperPair;
}

static void otio_composition_constructor_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(it);
    Composition *co = Composition_create("test", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co);
    ComposableVector *composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) it);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool resultOK = Composition_set_children(co, composableVector, errorStatus);

    assert_string_equal(SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) co),
                        "test");

    otiostr compositionKind = Composition_composition_kind(co);
    assert_string_equal(compositionKind, "Composition");
    otiostr_delete(compositionKind);

    assert_true(resultOK);

    ComposableRetainerVector *composableRetainerVector =
            Composition_children(co);

    assert_int_equal(ComposableVector_size(composableVector),
                     ComposableRetainerVector_size(composableRetainerVector));

    ComposableRetainerVectorIterator *retainerIt =
            ComposableRetainerVector_begin(composableRetainerVector);
    ComposableRetainerVectorIterator *retainerItEnd =
            ComposableRetainerVector_end(composableRetainerVector);
    ComposableVectorIterator *vectorIt =
            ComposableVector_begin(composableVector);

    for (; ComposableRetainerVectorIterator_not_equal(retainerIt, retainerItEnd);
           ComposableRetainerVectorIterator_advance(retainerIt, 1),
                   ComposableVectorIterator_advance(vectorIt, 1)) {
        Composable *composableVectorElement = ComposableVectorIterator_value(vectorIt);
        OTIO_RETAIN(composableVectorElement);
        RetainerComposable *retainerVectorElement =
                ComposableRetainerVectorIterator_value(retainerIt);
        Composable *retainerComposableValue =
                RetainerComposable_take_value(retainerVectorElement);

        assert_true(SerializableObject_is_equivalent_to(
                (OTIOSerializableObject *) composableVectorElement,
                (OTIOSerializableObject *) retainerComposableValue));

        RetainerComposable_managed_destroy(retainerVectorElement);
        OTIO_RELEASE(composableVectorElement);
        retainerVectorElement = NULL;
        composableVectorElement = NULL;
    }

    OTIO_RELEASE(it);
    it = NULL;
    OTIO_RELEASE(co);
    co = NULL;
    ComposableVector_destroy(composableVector);
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;
    ComposableRetainerVectorIterator_destroy(retainerIt);
    retainerIt = NULL;
    ComposableRetainerVectorIterator_destroy(retainerItEnd);
    retainerItEnd = NULL;
    ComposableVectorIterator_destroy(vectorIt);
    vectorIt = NULL;
}

static void otio_composition_equality_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Composition *co0 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co0);
    Composition *co00 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co00);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) co0, (OTIOSerializableObject *) co00));

    Item *a = Item_create("A", nullRange, NULL, NULL, NULL);
    Item *b = Item_create("B", nullRange, NULL, NULL, NULL);
    Item *c = Item_create("C", nullRange, NULL, NULL, NULL);
    Composition *co1 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co1);
    ComposableVector *composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) a);
    ComposableVector_push_back(composableVector, (Composable *) b);
    ComposableVector_push_back(composableVector, (Composable *) c);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool resultOK =
            Composition_set_children(co1, composableVector, errorStatus);
    assert_true(resultOK);
    ComposableVector_destroy(composableVector);
    composableVector = NULL;

    Item *x = Item_create("X", nullRange, NULL, NULL, NULL);
    Item *y = Item_create("Y", nullRange, NULL, NULL, NULL);
    Item *z = Item_create("Z", nullRange, NULL, NULL, NULL);
    Composition *co2 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co2);
    composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) x);
    ComposableVector_push_back(composableVector, (Composable *) y);
    ComposableVector_push_back(composableVector, (Composable *) z);
    resultOK = Composition_set_children(co2, composableVector, errorStatus);
    assert_true(resultOK);
    ComposableVector_destroy(composableVector);
    composableVector = NULL;

    Item *a2 = Item_create("A", nullRange, NULL, NULL, NULL);
    Item *b2 = Item_create("B", nullRange, NULL, NULL, NULL);
    Item *c2 = Item_create("C", nullRange, NULL, NULL, NULL);
    Composition *co3 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co3);
    composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) a2);
    ComposableVector_push_back(composableVector, (Composable *) b2);
    ComposableVector_push_back(composableVector, (Composable *) c2);
    resultOK = Composition_set_children(co3, composableVector, errorStatus);
    assert_true(resultOK);
    ComposableVector_destroy(composableVector);
    composableVector = NULL;

    assert_false(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) co1, (OTIOSerializableObject *) co2));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) co1, (OTIOSerializableObject *) co3));

    OTIO_RELEASE(co0);
    OTIO_RELEASE(co00);
    OTIO_RELEASE(co1);
    OTIO_RELEASE(co2);
    OTIO_RELEASE(co3);
    co3 = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_composition_is_parent_of_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Composition *co = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co);
    Composition *co_2 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co_2);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    assert_false(Composition_is_parent_of(co, (Composable *) co_2));
    bool appendOK =
            Composition_append_child(co, (Composable *) co_2, errorStatus);
    assert_true(Composition_is_parent_of(co, (Composable *) co_2));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(co);
    OTIO_RELEASE(co_2);
    co = NULL;
}

static void otio_composition_parent_manip_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    Composition *co = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co);
    ComposableVector *composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) it);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool resultOK = Composition_set_children(co, composableVector, errorStatus);
    Composition *parent = Composable_parent((Composable *) it);
    assert_ptr_equal(parent, co);

    OTIO_RELEASE(co);
    co = NULL;
    ComposableVector_destroy(composableVector);
    composableVector = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_composition_parent_move_child_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(it);
    Composition *co = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co);
    ComposableVector *composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) it);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool resultOK = Composition_set_children(co, composableVector, errorStatus);
    Composition *parent = Composable_parent((Composable *) it);
    assert_ptr_equal(parent, co);

    Composition *co2 = Composition_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(co2);

    Composition_remove_child(co, 0, errorStatus);

    resultOK = Composition_set_children(co2, composableVector, errorStatus);

    parent = Composable_parent((Composable *) it);
    assert_ptr_equal(parent, co2);

    OTIO_RELEASE(it);
    OTIO_RELEASE(co);
    co = NULL;
    OTIO_RELEASE(co2);
    co2 = NULL;
    ComposableVector_destroy(composableVector);
    composableVector = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_composition_remove_actually_removes_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *track = Track_create(NULL, nullRange, NULL, NULL);
    Clip *clip = Clip_create(NULL, NULL, nullRange, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Composition_append_child(
            (Composition *) track, (Composable *) clip, errorStatus);

    ComposableRetainerVector *children =
            Composition_children((Composition *) track);
    RetainerComposable *child = ComposableRetainerVector_at(children, 0);
    Composable *child_val = RetainerComposable_take_value(child);

    assert_ptr_equal((Composable *) clip, child_val);

    Composition_remove_child((Composition *) track, 0, errorStatus);

    ComposableRetainerVector_destroy(children);
    children = NULL;
    RetainerComposable_managed_destroy(child);

    children = Composition_children((Composition *) track);
    assert_ptr_equal(ComposableRetainerVector_size(children), 0);
    ComposableRetainerVector_destroy(children);
}

static void otio_stack_constructor_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *st = Stack_create("test", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);
    assert_string_equal(SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) st),
                        "test");
    OTIO_RELEASE(st);
    st = NULL;
}

static void otio_stack_serialize_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *st = Stack_create("test", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);
    Clip *clip = Clip_create("testClip", NULL, nullRange, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool insertOK = Composition_insert_child(
            (Composition *) st, 0, (Composable *) clip, errorStatus);

    assert_true(insertOK);
    Any *stack_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) st);
    otiostr encoded = serialize_json_to_string(stack_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) st);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) st, decoded_object));

    OTIO_RELEASE(st);
    st = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    otiostr_delete(encoded);
}

static void otio_stack_trim_child_range_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *tr = Track_create("foo", nullRange, NULL, NULL);
    OTIO_RETAIN(tr);
    Stack *st = Stack_create("foo", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);

    Composition *comp[] = {(Composition *) tr, (Composition *) st};

    for (int i = 0; i < sizeof(comp) / sizeof(comp[0]); i++) {
        Composition *co = comp[i];

        RationalTime start_time = RationalTime_create(100, 24);
        RationalTime duration = RationalTime_create(50, 24);
        OptionalTimeRange tr = OptionalTimeRange_create(
                TimeRange_create_with_start_time_and_duration(start_time, duration));
        Item_set_source_range((Item *) co, tr);

        start_time = RationalTime_create(110, 24);
        duration = RationalTime_create(30, 24);
        TimeRange r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        OptionalTimeRange st_trim_child_range = Composition_trim_child_range(co, r);
        assert_true(OptionalTimeRange_valid(st_trim_child_range));
        assert_true(TimeRange_equal(OptionalTimeRange_value(st_trim_child_range), r));

        start_time = RationalTime_create(0, 24);
        duration = RationalTime_create(30, 24);
        r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        st_trim_child_range = Composition_trim_child_range(co, r);
        assert_false(OptionalTimeRange_valid(st_trim_child_range));

        start_time = RationalTime_create(1000, 24);
        duration = RationalTime_create(30, 24);
        r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        st_trim_child_range = Composition_trim_child_range(co, r);
        assert_false(OptionalTimeRange_valid(st_trim_child_range));

        start_time = RationalTime_create(90, 24);
        duration = RationalTime_create(30, 24);
        r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        st_trim_child_range = Composition_trim_child_range(co, r);

        start_time = RationalTime_create(100, 24);
        duration = RationalTime_create(20, 24);
        tr = OptionalTimeRange_create(
                TimeRange_create_with_start_time_and_duration(start_time, duration));
        assert_true(TimeRange_equal(OptionalTimeRange_value(tr),
                                    OptionalTimeRange_value(st_trim_child_range)));

        start_time = RationalTime_create(110, 24);
        duration = RationalTime_create(50, 24);
        r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        st_trim_child_range = Composition_trim_child_range(co, r);

        start_time = RationalTime_create(110, 24);
        duration = RationalTime_create(40, 24);
        tr = OptionalTimeRange_create(
                TimeRange_create_with_start_time_and_duration(start_time, duration));
        assert_true(TimeRange_equal(OptionalTimeRange_value(tr),
                                    OptionalTimeRange_value(st_trim_child_range)));

        start_time = RationalTime_create(90, 24);
        duration = RationalTime_create(1000, 24);
        r = TimeRange_create_with_start_time_and_duration(start_time, duration);
        st_trim_child_range = Composition_trim_child_range(co, r);

        OptionalTimeRange co_source_range = Item_source_range((Item *) co);
        assert_true(OptionalTimeRange_valid(co_source_range));
        assert_true(TimeRange_equal(OptionalTimeRange_value(co_source_range),
                                    OptionalTimeRange_value(st_trim_child_range)));
    }
    OTIO_RELEASE(tr);
    tr = NULL;
    OTIO_RELEASE(st);
    st = NULL;
}

static void otio_stack_range_of_child_test(void **state) {
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, source_range, NULL);
    OTIO_RETAIN(clip1);
    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, source_range, NULL);
    OTIO_RETAIN(clip2);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, source_range, NULL);
    OTIO_RETAIN(clip3);

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *st = Stack_create("foo", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Composition_insert_child(
            (Composition *) st, 0, (Composable *) clip1, errorStatus);
    Composition_insert_child(
            (Composition *) st, 1, (Composable *) clip2, errorStatus);
    Composition_insert_child(
            (Composition *) st, 2, (Composable *) clip3, errorStatus);

    /** stack should be as long as longest child */
    RationalTime length = RationalTime_create(50, 24);
    RationalTime st_duration = Composable_duration((Composable *) st, errorStatus);
    assert_true(RationalTime_equal(length, st_duration));

    RationalTime zero_time = RationalTime_create(0, 24);
    /** stacked items should all start at time zero */
    TimeRange range_at_0 = Stack_range_of_child_at_index(st, 0, errorStatus);
    TimeRange range_at_1 = Stack_range_of_child_at_index(st, 1, errorStatus);
    TimeRange range_at_2 = Stack_range_of_child_at_index(st, 2, errorStatus);
    RationalTime start0 = TimeRange_start_time(range_at_0);
    RationalTime start1 = TimeRange_start_time(range_at_1);
    RationalTime start2 = TimeRange_start_time(range_at_2);
    assert_true(RationalTime_equal(start0, zero_time));
    assert_true(RationalTime_equal(start1, zero_time));
    assert_true(RationalTime_equal(start2, zero_time));

    RationalTime duration0 = TimeRange_duration(range_at_0);
    RationalTime duration1 = TimeRange_duration(range_at_1);
    RationalTime duration2 = TimeRange_duration(range_at_2);
    RationalTime duration_time = RationalTime_create(50, 24);
    assert_true(RationalTime_equal(duration0, duration_time));
    assert_true(RationalTime_equal(duration1, duration_time));
    assert_true(RationalTime_equal(duration2, duration_time));

    OTIO_RELEASE(clip1);
    OTIO_RELEASE(clip2);
    OTIO_RELEASE(clip3);
    OTIO_RELEASE(st);
    st = NULL;
}

static void otio_stack_range_of_child_with_duration_test(void **state) {
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(5, 24);
    duration = RationalTime_create(5, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *st = Stack_create("foo", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Composition_insert_child(
            (Composition *) st, 0, (Composable *) clip1, errorStatus);
    Composition_insert_child(
            (Composition *) st, 1, (Composable *) clip2, errorStatus);
    Composition_insert_child(
            (Composition *) st, 2, (Composable *) clip3, errorStatus);

    Item_set_source_range((Item *) st, st_sourcerange);

    /** range always returns the pre-trimmed range.  To get the post-trim
     * range, call .trimmed_range()
     */
    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) st);
    ComposableRetainerVectorIterator *it =
            ComposableRetainerVector_begin(composableRetainerVector);
    RetainerComposable *retainerComposable =
            ComposableRetainerVectorIterator_value(it);
    Composable *st_0 = RetainerComposable_take_value(retainerComposable);
    TimeRange child_range = Composition_range_of_child((Composition *) st, st_0, errorStatus);
    start_time = RationalTime_create(0, 24);
    duration = RationalTime_create(50, 24);
    TimeRange time_range =
            TimeRange_create_with_start_time_and_duration(start_time, duration);
    assert_true(TimeRange_equal(time_range, child_range));

    RationalTime rt = RationalTime_create(25, 24);
    RationalTime rt2 = RationalTime_create(125, 24);
    RationalTime st_transformed_time =
            Item_transformed_time((Item *) st, rt, (Item *) st_0, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, rt2));

    st_transformed_time =
            Item_transformed_time((Item *) st_0, rt2, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, rt));

    /** trimmed_ functions take into account the source_range */
    OptionalTimeRange st_trimmed_range_child_0 = OptionalTimeRange_create(
            Stack_trimmed_range_of_child_at_index(st, 0, errorStatus));
    st_sourcerange = Item_source_range((Item *) st);
    assert_true(OptionalTimeRange_valid(st_sourcerange));
    assert_true(TimeRange_equal(OptionalTimeRange_value(st_trimmed_range_child_0),
                                OptionalTimeRange_value(st_sourcerange)));

    st_trimmed_range_child_0 = Composition_trimmed_range_of_child(
            (Composition *) st, (Composable *) st_0, errorStatus);
    start_time = RationalTime_create(5, 24);
    duration = RationalTime_create(5, 24);
    time_range =
            TimeRange_create_with_start_time_and_duration(start_time, duration);
    assert_true(OptionalTimeRange_valid(st_trimmed_range_child_0));
    assert_true(TimeRange_equal(OptionalTimeRange_value(st_trimmed_range_child_0), time_range));

    /** get the trimmed range in the parent */
    OptionalTimeRange st_0_trimmed_range_in_parent =
            Item_trimmed_range_in_parent((Item *) st_0, errorStatus);
    assert_true(OptionalTimeRange_valid(st_0_trimmed_range_in_parent));
    assert_true(TimeRange_equal(OptionalTimeRange_value(st_0_trimmed_range_in_parent),
                                OptionalTimeRange_value(st_trimmed_range_child_0)));

    ComposableRetainerVector_destroy(composableRetainerVector);
    ComposableRetainerVectorIterator_destroy(it);
    RetainerComposable_managed_destroy(retainerComposable);

    Clip *errorClip = Clip_create(NULL, NULL, nullRange, NULL);
    OTIO_RETAIN(errorClip);
    OptionalTimeRange errorTime =
            Item_trimmed_range_in_parent((Item *) errorClip, errorStatus);
    OTIO_ErrorStatus_Outcome outcome = OTIOErrorStatus_get_outcome(errorStatus);
    assert_int_equal(outcome, 18);
    OTIO_RELEASE(errorClip);
    errorClip = NULL;

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(st);
    st = NULL;
}

static void otio_stack_transformed_time_test(void **state) {
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(5, 24);
    duration = RationalTime_create(5, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Stack *st = Stack_create("foo", nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(st);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Composition_insert_child(
            (Composition *) st, 0, (Composable *) clip1, errorStatus);
    Composition_insert_child(
            (Composition *) st, 1, (Composable *) clip2, errorStatus);
    Composition_insert_child(
            (Composition *) st, 2, (Composable *) clip3, errorStatus);

    Item_set_source_range((Item *) st, st_sourcerange);

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) st);
    RetainerComposable *rc0 =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    RetainerComposable *rc1 =
            ComposableRetainerVector_at(composableRetainerVector, 1);
    RetainerComposable *rc2 =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    Composable *c0 = RetainerComposable_take_value(rc0);
    Composable *c1 = RetainerComposable_take_value(rc1);
    Composable *c2 = RetainerComposable_take_value(rc2);
    assert_string_equal(SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) c0),
                        "clip1");
    assert_string_equal(SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) c1),
                        "clip2");
    assert_string_equal(SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) c2),
                        "clip3");

    ComposableRetainerVector_destroy(composableRetainerVector);

    RationalTime test_time = RationalTime_create(0, 24);
    RationalTime test_time2 = RationalTime_create(100, 24);
    RationalTime st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip1, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    /** ensure that transformed_time does not edit in place */
    RationalTime verify_test_time = RationalTime_create(0, 24);
    assert_true(RationalTime_equal(test_time, verify_test_time));

    test_time = RationalTime_create(0, 24);
    test_time2 = RationalTime_create(101, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip2, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(0, 24);
    test_time2 = RationalTime_create(102, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip3, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(50, 24);
    test_time2 = RationalTime_create(150, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip1, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(50, 24);
    test_time2 = RationalTime_create(151, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip2, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(50, 24);
    test_time2 = RationalTime_create(152, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) st, test_time, (Item *) clip3, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(100, 24);
    test_time2 = RationalTime_create(0, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip1, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(101, 24);
    test_time2 = RationalTime_create(0, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip2, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(102, 24);
    test_time2 = RationalTime_create(0, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip3, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(150, 24);
    test_time2 = RationalTime_create(50, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip1, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(151, 24);
    test_time2 = RationalTime_create(50, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip2, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    test_time = RationalTime_create(152, 24);
    test_time2 = RationalTime_create(50, 24);
    st_transformed_time = Item_transformed_time(
            (Item *) clip3, test_time, (Item *) st, errorStatus);
    assert_true(RationalTime_equal(st_transformed_time, test_time2));

    OTIO_RELEASE(st);
    st = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_track_serialize_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create("foo", nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Any *sq_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) sq);
    otiostr encoded = serialize_json_to_string(sq_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) sq);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) sq, decoded_object));

    OTIO_RELEASE(sq);
    sq = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    otiostr_delete(encoded);
}

static void otio_track_instancing_test(void **state) {
    RationalTime length = RationalTime_create(5, 1);
    RationalTime zero_time = RationalTime_create(0, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(zero_time, length));
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, tr, NULL, NULL, NULL);
    Track *sq = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool insertOK = Composition_insert_child(
            (Composition *) sq, 0, (Composable *) it, errorStatus);
    assert_true(insertOK);
    TimeRange sq_range_of_child_0 =
            Track_range_of_child_at_index(sq, 0, errorStatus);
    assert_true(TimeRange_equal(sq_range_of_child_0, OptionalTimeRange_value(tr)));

    /** Can't put item on a composition if it's already in one */
    Track *test_track = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(test_track);
    insertOK = Composition_insert_child(
            (Composition *) test_track, 0, (Composable *) it, errorStatus);
    assert_false(insertOK);
    OTIO_RELEASE(test_track);
    test_track = NULL;

    /** Instancing is not allowed */
    ComposableVector *composableVector = ComposableVector_create();
    ComposableVector_push_back(composableVector, (Composable *) it);
    ComposableVector_push_back(composableVector, (Composable *) it);
    ComposableVector_push_back(composableVector, (Composable *) it);
    Track *test_track2 = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(test_track2);
    insertOK = Composition_set_children((Composition *) test_track2, composableVector, errorStatus);
    assert_false(insertOK);
    OTIO_RELEASE(test_track2);
    test_track = NULL;
    ComposableVector_destroy(composableVector);
    composableVector = NULL;

    /**inserting duplicates should raise error and have no side effects*/
    ComposableRetainerVector *composableRetainerVector = Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 1);
    ComposableRetainerVector_destroy(composableRetainerVector);
    insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) it, errorStatus);
    assert_false(insertOK);
    composableRetainerVector = Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 1);
    ComposableRetainerVector_destroy(composableRetainerVector);

    insertOK = Composition_insert_child(
            (Composition *) sq, 1, (Composable *) it, errorStatus);
    assert_false(insertOK);
    composableRetainerVector = Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 1);
    ComposableRetainerVector_destroy(composableRetainerVector);

    OTIO_RELEASE(sq);
    sq = NULL;
}

static void otio_track_delete_parent_container_test(void **state) {
    /** deleting the parent container should null out the parent pointer */
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Item *it = Item_create(NULL, nullRange, NULL, NULL, NULL);
    Track *sq = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool insertOK = Composition_insert_child(
            (Composition *) sq, 0, (Composable *) it, errorStatus);
    assert_true(insertOK);
    OTIO_RELEASE(sq);
    sq = NULL;
    Composition *parent = Composable_parent((Composable *) it);
    assert_null(parent);
}

//TEST_F(OTIOTrackTests, TransactionalTest
//)
//{
//    Item*            item        = Item_create(NULL, NULL, NULL, NULL, NULL);
//    Track*           trackA      = Track_create(NULL, NULL, NULL, NULL);
//    Track*           trackB      = Track_create(NULL, NULL, NULL, NULL);
//    OTIOErrorStatus* errorStatus = OTIOErrorStatus_create();
//    Item*            item1       = Item_create(NULL, NULL, NULL, NULL, NULL);
//    Item*            item2       = Item_create(NULL, NULL, NULL, NULL, NULL);
//    Item*            item3       = Item_create(NULL, NULL, NULL, NULL, NULL);
//
//    SerializableObject* itemClone1 =
//        SerializableObject_clone((SerializableObject*) item, errorStatus);
//
//    SerializableObject* itemClone2 =
//        SerializableObject_clone((SerializableObject*) item, errorStatus);
// TODO segfault here

//    SerializableObject* itemClone3 =
//        SerializableObject_clone((SerializableObject*) item, errorStatus);
//        SerializableObject* itemClone3 =
//            SerializableObject_clone((SerializableObject*) itemClone2, errorStatus);
//    SerializableObject* itemClone4 =
//        SerializableObject_clone((SerializableObject*) itemClone1, errorStatus);
//    SerializableObject* itemClone5 =
//        SerializableObject_clone((SerializableObject*) item, errorStatus);
//    SerializableObject* itemClone6 =
//        SerializableObject_clone((SerializableObject*) itemClone1, errorStatus);
//    bool insertOK = false;
//
//    insertOK = Composition_insert_child(
//        (Composition*) trackA, 0, (Composable*) item1, errorStatus);
//    ASSERT_TRUE(insertOK);
//    insertOK = Composition_insert_child(
//        (Composition*) trackA, 1, (Composable*) item2, errorStatus);
//    ASSERT_TRUE(insertOK);
//    insertOK = Composition_insert_child(
//        (Composition*) trackA, 2, (Composable*) item3, errorStatus);
//    ASSERT_TRUE(insertOK);
//    ComposableRetainerVector* composableRetainerVector =
//        Composition_children((Composition*) trackA);
//    EXPECT_EQ(ComposableRetainerVector_size(composableRetainerVector), 3);
//    ComposableRetainerVector_destroy(composableRetainerVector);
//    composableRetainerVector = NULL;

//    insertOK = Composition_insert_child(
//        (Composition*) trackB, 0, (Composable*) itemClone4, errorStatus);
//    ASSERT_TRUE(insertOK);
//    insertOK = Composition_insert_child(
//        (Composition*) trackB, 1, (Composable*) itemClone5, errorStatus);
//    ASSERT_TRUE(insertOK);
//    insertOK = Composition_insert_child(
//        (Composition*) trackB, 2, (Composable*) itemClone6, errorStatus);
//    ASSERT_TRUE(insertOK);
//    composableRetainerVector = Composition_children((Composition*) trackA);
//    EXPECT_EQ(ComposableRetainerVector_size(composableRetainerVector), 3);
//    ComposableRetainerVector_destroy(composableRetainerVector);
//    composableRetainerVector = NULL;
//}

static void otio_track_range_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    RationalTime length = RationalTime_create(5, 1);
    RationalTime zero_time = RationalTime_create(0, 1);
    OptionalTimeRange tr = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(zero_time, length));
    Item *it = Item_create(NULL, tr, NULL, NULL, NULL);
    Track *sq = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) it, errorStatus);
    assert_true(insertOK);
    TimeRange sq_range_child_0 =
            Track_range_of_child_at_index(sq, 0, errorStatus);
    assert_true(TimeRange_equal(sq_range_child_0, OptionalTimeRange_value(tr)));

    Item *it2 = Item_create(NULL, tr, NULL, NULL, NULL);
    Item *it3 = Item_create(NULL, tr, NULL, NULL, NULL);
    Item *it4 = Item_create(NULL, tr, NULL, NULL, NULL);
    insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) it2, errorStatus);
    assert_true(insertOK);
    insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) it3, errorStatus);
    assert_true(insertOK);
    insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) it4, errorStatus);
    assert_true(insertOK);

    TimeRange sq_range_child_1 =
            Track_range_of_child_at_index(sq, 1, errorStatus);
    RationalTime start_time = RationalTime_create(5, 1);
    RationalTime duration = RationalTime_create(5, 1);
    tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), sq_range_child_1));

    sq_range_child_0 = Track_range_of_child_at_index(sq, 0, errorStatus);
    start_time = RationalTime_create(0, 1);
    duration = RationalTime_create(5, 1);
    tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), sq_range_child_0));

    TimeRange sq_range_child_minus_1 =
            Track_range_of_child_at_index(sq, -1, errorStatus);
    start_time = RationalTime_create(15, 1);
    duration = RationalTime_create(5, 1);
    tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), sq_range_child_minus_1));

    TimeRange sq_range_child_minus_error =
            Track_range_of_child_at_index(sq, 11, errorStatus);
    OTIO_ErrorStatus_Outcome outcome = OTIOErrorStatus_get_outcome(errorStatus);
    assert_int_equal(outcome, 13);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    RationalTime sq_duration = Item_duration((Item *) sq, errorStatus);
    RationalTime duration_compare = RationalTime_create(20, 1);
    assert_true(RationalTime_equal(sq_duration, duration_compare));

    /** add a transition to either side */
    TimeRange range_of_child_3 =
            Track_range_of_child_at_index(sq, 3, errorStatus);
    OptionalRationalTime in_offset = OptionalRationalTime_create(RationalTime_create(10, 24));
    OptionalRationalTime out_offset = OptionalRationalTime_create(RationalTime_create(12, 24));
    TimeRange range_of_item =
            Track_range_of_child_at_index(sq, 3, errorStatus);
    Transition *trx1 =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);
    Transition *trx2 =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);
    Transition *trx3 =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);
    insertOK = Composition_insert_child(
            (Composition *) sq, 0, (Composable *) trx1, errorStatus);
    assert_true(insertOK);
    insertOK = Composition_insert_child(
            (Composition *) sq, 3, (Composable *) trx2, errorStatus);
    assert_true(insertOK);
    insertOK = Composition_append_child(
            (Composition *) sq, (Composable *) trx3, errorStatus);
    assert_true(insertOK);

    /** range of Transition */
    start_time = RationalTime_create(230, 24);
    duration = RationalTime_create(22, 24);
    tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    range_of_item = Track_range_of_child_at_index(sq, 3, errorStatus);
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), range_of_item));

    start_time = RationalTime_create(470, 24);
    duration = RationalTime_create(22, 24);
    tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    range_of_item = Track_range_of_child_at_index(sq, -1, errorStatus);
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), range_of_item));

    tr = OptionalTimeRange_create(Track_range_of_child_at_index(sq, 5, errorStatus));
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr), range_of_child_3));

    sq_duration = Item_duration((Item *) sq, errorStatus);
    /** duration_compare = length x 4 + in_offset + out_offset */
    duration_compare = RationalTime_create(20 + 22.0 / 24.0, 1);
    assert_true(RationalTime_equal(sq_duration, duration_compare));

    OTIO_RELEASE(sq);
    sq = NULL;
}

static void otio_track_range_of_child_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create("foo", nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, st_sourcerange, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip1, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip2, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip3, errorStatus);
    assert_true(appendOK);

    /** The Track should be as long as the children summed up */
    RationalTime sq_duration = Item_duration((Item *) sq, errorStatus);
    RationalTime duration_compare = RationalTime_create(150, 24);
    assert_true(RationalTime_equal(sq_duration, duration_compare));

    /** Sequenced items should all land end-to-end */
    duration_compare = RationalTime_create(50, 24);
    TimeRange range_of_child_index =
            Track_range_of_child_at_index(sq, 0, errorStatus);
    RationalTime range_time = TimeRange_start_time(range_of_child_index);
    RationalTime range_duration = TimeRange_duration(range_of_child_index);
    RationalTime time_compare = RationalTime_create(0, 1);
    assert_true(RationalTime_equal(range_time, time_compare));
    assert_true(RationalTime_equal(duration_compare, range_duration));

    range_of_child_index = Track_range_of_child_at_index(sq, 1, errorStatus);
    range_time = TimeRange_start_time(range_of_child_index);
    range_duration = TimeRange_duration(range_of_child_index);
    time_compare = RationalTime_create(50, 24);
    assert_true(RationalTime_equal(range_time, time_compare));
    assert_true(RationalTime_equal(duration_compare, range_duration));

    range_of_child_index = Track_range_of_child_at_index(sq, 2, errorStatus);
    range_time = TimeRange_start_time(range_of_child_index);
    range_duration = TimeRange_duration(range_of_child_index);
    time_compare = RationalTime_create(100, 24);
    assert_true(RationalTime_equal(range_time, time_compare));
    assert_true(RationalTime_equal(duration_compare, range_duration));

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) sq);
    RetainerComposable *retainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    Composable *retainerComposableValue =
            RetainerComposable_take_value(retainerComposable);
    TimeRange range_compare = Composition_range_of_child(
            (Composition *) sq, retainerComposableValue, errorStatus);
    assert_true(TimeRange_equal(range_compare, range_of_child_index));

    /** should trim 5 frames off the front, and 5 frames off the back */
    start_time = RationalTime_create(5, 24);
    duration = RationalTime_create(140, 24);
    OptionalTimeRange sq_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Item_set_source_range((Item *) sq, sq_sourcerange);
    TimeRange sq_trimmed_range_of_child_index =
            Track_trimmed_range_of_child_at_index(sq, 0, errorStatus);

    duration = RationalTime_create(45, 24);
    range_compare =
            TimeRange_create_with_start_time_and_duration(start_time, duration);

    assert_true(TimeRange_equal(range_compare, sq_trimmed_range_of_child_index));

    sq_trimmed_range_of_child_index =
            Track_trimmed_range_of_child_at_index(sq, 1, errorStatus);
    range_compare = Track_range_of_child_at_index(sq, 1, errorStatus);
    assert_true(TimeRange_equal(range_compare, sq_trimmed_range_of_child_index));

    sq_trimmed_range_of_child_index =
            Track_trimmed_range_of_child_at_index(sq, 2, errorStatus);
    start_time = RationalTime_create(100, 24);
    duration = RationalTime_create(45, 24);
    range_compare =
            TimeRange_create_with_start_time_and_duration(start_time, duration);
    assert_true(TimeRange_equal(range_compare, sq_trimmed_range_of_child_index));

    /** get the trimmed range in the parent */
    retainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    OptionalTimeRange trimmed_range_in_parent = Item_trimmed_range_in_parent(
            (Item *) retainerComposableValue, errorStatus);
    OptionalTimeRange trimmed_range_of_child = Composition_trimmed_range_of_child(
            (Composition *) sq, retainerComposableValue, errorStatus);
    assert_true(OptionalTimeRange_valid(trimmed_range_in_parent));
    assert_true(OptionalTimeRange_valid(trimmed_range_of_child));
    assert_true(TimeRange_equal(OptionalTimeRange_value(trimmed_range_in_parent),
                                OptionalTimeRange_value(trimmed_range_of_child)));

    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;
    OTIO_RELEASE(sq);
    sq = NULL;
    RetainerComposable_managed_destroy(retainerComposable);
    retainerComposable = NULL;
}

static void otio_track_range_trimmed_out_test(void **state) {
    RationalTime start_time = RationalTime_create(60, 24);
    RationalTime duration = RationalTime_create(10, 24);
    OptionalTimeRange source_range =
            OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start_time, duration));
    Track *sq = Track_create("top_track", source_range, NULL, NULL);
    OTIO_RETAIN(sq);

    start_time = RationalTime_create(100, 24);
    duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip1, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip2, errorStatus);
    assert_true(appendOK);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();
    /** should be trimmed out, at the moment, the sentinel for that is None */
    TimeRange trimmed_range_of_child_index =
            Track_trimmed_range_of_child_at_index(sq, 0, errorStatus);
    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 21);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    TimeRange not_nothing =
            Track_trimmed_range_of_child_at_index(sq, 1, errorStatus);
    source_range = Item_source_range((Item *) sq);
    assert_true(OptionalTimeRange_valid(source_range));
    assert_true(TimeRange_equal(not_nothing, OptionalTimeRange_value(source_range)));

    /** should trim out second clip */
    start_time = RationalTime_create(0, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Item_set_source_range((Item *) sq, source_range);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    trimmed_range_of_child_index =
            Track_trimmed_range_of_child_at_index(sq, 1, errorStatus);
    assert_int_equal(OTIOErrorStatus_get_outcome(errorStatus), 21);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();

    not_nothing = Track_trimmed_range_of_child_at_index(sq, 0, errorStatus);
    source_range = Item_source_range((Item *) sq);
    assert_true(OptionalTimeRange_valid(source_range));
    assert_true(TimeRange_equal(not_nothing, OptionalTimeRange_value(source_range)));

    OTIO_RELEASE(sq);
    sq = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_track_range_nested_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create("inner", nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, st_sourcerange, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip1, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip2, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip3, errorStatus);
    assert_true(appendOK);

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 3);
    ComposableRetainerVector_destroy(composableRetainerVector);

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = OTIOErrorStatus_create();
    Track *sq2 = (Track *) SerializableObject_clone(
            (OTIOSerializableObject *) sq, errorStatus);
    OTIO_RETAIN(sq2);
    Track *sq3 = (Track *) SerializableObject_clone(
            (OTIOSerializableObject *) sq, errorStatus);
    OTIO_RETAIN(sq3);
    OTIO_RELEASE(sq);
    OTIO_RELEASE(sq2);
    OTIO_RELEASE(sq3);
}

static void otio_track_set_item_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    Clip *it = Clip_create(NULL, NULL, nullRange, NULL);
    Clip *it_2 = Clip_create(NULL, NULL, nullRange, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) it, errorStatus);
    assert_true(appendOK);
    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 1);
    ComposableRetainerVector_destroy(composableRetainerVector);

    bool setOK = Composition_set_child(
            (Composition *) sq, 0, (Composable *) it_2, errorStatus);
    assert_true(setOK);
    composableRetainerVector = Composition_children((Composition *) sq);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 1);
    ComposableRetainerVector_destroy(composableRetainerVector);
    OTIO_RELEASE(sq);
    sq = NULL;
}

static void otio_track_transformed_time_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create("foo", nullRange, NULL, NULL);
    OTIO_RETAIN(sq);
    RationalTime start_time = RationalTime_create(100, 24);
    RationalTime duration = RationalTime_create(50, 24);
    OptionalTimeRange st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, st_sourcerange, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(50, 24);
    st_sourcerange = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, st_sourcerange, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip1, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip2, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) clip3, errorStatus);
    assert_true(appendOK);

    start_time = RationalTime_create(0, 24);
    duration = RationalTime_create(50, 24);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Gap *fl =
            Gap_create_with_source_range(source_range, "GAP", NULL, NULL, NULL);
    assert_false(Gap_visible(fl));

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) sq);
    RetainerComposable *retainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    Composable *retainerComposableValue =
            RetainerComposable_take_value(retainerComposable);
    clip1 = (Clip *) retainerComposableValue;
    assert_string_equal("clip1",
                        SerializableObjectWithMetadata_name(
                                (SerializableObjectWithMetadata *) clip1));
    retainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 1);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    clip2 = (Clip *) retainerComposableValue;
    assert_string_equal("clip2",
                        SerializableObjectWithMetadata_name(
                                (SerializableObjectWithMetadata *) clip2));;
    retainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    clip3 = (Clip *) retainerComposableValue;
    assert_string_equal("clip3",
                        SerializableObjectWithMetadata_name(
                                (SerializableObjectWithMetadata *) clip3));
    ComposableRetainerVector_destroy(composableRetainerVector);

    RationalTime rationalTime = RationalTime_create(0, 24);
    RationalTime transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip1, errorStatus);
    RationalTime compare_time = RationalTime_create(100, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(0, 24);
    transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip2, errorStatus);
    compare_time = RationalTime_create(51, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(0, 24);
    transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip3, errorStatus);
    compare_time = RationalTime_create(2, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(50, 24);
    transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip1, errorStatus);
    compare_time = RationalTime_create(150, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(50, 24);
    transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip2, errorStatus);
    compare_time = RationalTime_create(101, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(50, 24);
    transformed_time = Item_transformed_time(
            (Item *) sq, rationalTime, (Item *) clip3, errorStatus);
    compare_time = RationalTime_create(52, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(100, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip1, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(0, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(101, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip2, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(50, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(102, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip3, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(100, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(150, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip1, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(50, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(151, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip2, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(100, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    rationalTime = RationalTime_create(152, 24);
    transformed_time = Item_transformed_time(
            (Item *) clip3, rationalTime, (Item *) sq, errorStatus);
    compare_time = RationalTime_create(150, 24);
    assert_true(RationalTime_equal(transformed_time, compare_time));

    OTIO_RELEASE(sq);
    sq = NULL;
}

static void otio_track_neighbors_of_simple_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(sq);

    OptionalRationalTime in_offset = OptionalRationalTime_create(RationalTime_create(10, 24));
    OptionalRationalTime out_offset = OptionalRationalTime_create(RationalTime_create(10, 24));

    Transition *trans =
            Transition_create(NULL, NULL, in_offset, out_offset, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) trans, errorStatus);
    assert_true(appendOK);

    /** neighbors of first transition */
    RetainerPairComposable *neighbors = Track_neighbors_of(
            sq,
            (Composable *) trans,
            errorStatus,
            OTIO_Track_NeighbourGapPolicy_never);
    RetainerComposable *retainerComposable =
            RetainerPairComposable_first(neighbors);
    Composable *retainerComposableValue =
            RetainerComposable_take_value(retainerComposable);
    assert_null(retainerComposableValue);
    retainerComposable = RetainerPairComposable_second(neighbors);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    assert_null(retainerComposableValue);

    /** test with the neighbor filling policy on */
    neighbors = Track_neighbors_of(
            sq,
            (Composable *) trans,
            errorStatus,
            OTIO_Track_NeighbourGapPolicy_around_transitions);
    RationalTime start_time = RationalTime_create(0, 24);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, OptionalRationalTime_value(in_offset)));
    Gap *fill = Gap_create_with_source_range(source_range, NULL, NULL, NULL, NULL);
    OTIO_RETAIN(fill);
    retainerComposable = RetainerPairComposable_first(neighbors);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) retainerComposableValue,
            (OTIOSerializableObject *) fill));
    retainerComposable = RetainerPairComposable_second(neighbors);
    retainerComposableValue = RetainerComposable_take_value(retainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) retainerComposableValue,
            (OTIOSerializableObject *) fill));

    OTIO_RELEASE(fill);
    OTIO_RELEASE(sq);
    sq = NULL;
}

static void otio_track_neighbors_of_from_data_test(void **state) {
    const char *sample_data_dir = xstr(SAMPLE_DATA_DIR);
    const char *edl_file = "transition_test.otio";
    char *edl_path = (char *) calloc(
            strlen(sample_data_dir) + strlen(edl_file) + 1, sizeof(char));
    strcpy(edl_path, sample_data_dir);
    strcat(edl_path, edl_file);

    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Any *timelineAny = create_safely_typed_any_serializable_object(
            (OTIOSerializableObject *) timeline);
    bool deserializeOK =
            deserialize_json_from_file(edl_path, timelineAny, errorStatus);
    assert_true(deserializeOK);

    timeline = (Timeline *) safely_cast_retainer_any(timelineAny);

    Stack *stack = Timeline_tracks(timeline);

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) stack);
    RetainerComposable *firstTrackRetainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 0);

    Track *seq = (Track *) RetainerComposable_take_value(firstTrackRetainerComposable);
    OTIO_RETAIN(seq);

    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;

    composableRetainerVector = Composition_children((Composition *) seq);
    RetainerComposable *seq_0_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    RetainerComposable *seq_1_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 1);
    RetainerComposable *seq_2_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    RetainerComposable *seq_3_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 3);
    RetainerComposable *seq_4_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 4);
    RetainerComposable *seq_5_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 5);
    Composable *seq_0 = RetainerComposable_take_value(seq_0_retainer);
    Composable *seq_1 = RetainerComposable_take_value(seq_1_retainer);
    Composable *seq_2 = RetainerComposable_take_value(seq_2_retainer);
    Composable *seq_3 = RetainerComposable_take_value(seq_3_retainer);
    Composable *seq_4 = RetainerComposable_take_value(seq_4_retainer);
    Composable *seq_5 = RetainerComposable_take_value(seq_5_retainer);
    RetainerPairComposable *neighbors = Track_neighbors_of(
            seq, seq_0, errorStatus, OTIO_Track_NeighbourGapPolicy_never);
    RetainerComposable *firstRetainerComposable =
            RetainerPairComposable_first(neighbors);
    RetainerComposable *secondRetainerComposable =
            RetainerPairComposable_second(neighbors);
    Composable *firstComposable =
            RetainerComposable_take_value(firstRetainerComposable);
    Composable *secondComposable =
            RetainerComposable_take_value(secondRetainerComposable);
    assert_null(firstComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) secondComposable,
            (OTIOSerializableObject *) seq_1));

    RetainerPairComposable_destroy(neighbors);

    RationalTime seq_0_in_offset = Transition_in_offset((Transition *) seq_0);
    RationalTime start_time =
            RationalTime_create(0, RationalTime_rate(seq_0_in_offset));
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, seq_0_in_offset));
    Gap *fill =
            Gap_create_with_source_range(source_range, NULL, NULL, NULL, NULL);
    neighbors = Track_neighbors_of(
            seq,
            seq_0,
            errorStatus,
            OTIO_Track_NeighbourGapPolicy_around_transitions);
    firstRetainerComposable = RetainerPairComposable_first(neighbors);
    secondRetainerComposable = RetainerPairComposable_second(neighbors);
    firstComposable = RetainerComposable_take_value(firstRetainerComposable);
    secondComposable = RetainerComposable_take_value(secondRetainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) firstComposable,
            (OTIOSerializableObject *) fill));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) secondComposable,
            (OTIOSerializableObject *) seq_1));
    RetainerPairComposable_destroy(neighbors);

    /** neighbor around second transition */
    neighbors = Track_neighbors_of(
            seq, seq_2, errorStatus, OTIO_Track_NeighbourGapPolicy_never);
    firstRetainerComposable = RetainerPairComposable_first(neighbors);
    secondRetainerComposable = RetainerPairComposable_second(neighbors);
    firstComposable = RetainerComposable_take_value(firstRetainerComposable);
    secondComposable = RetainerComposable_take_value(secondRetainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) firstComposable,
            (OTIOSerializableObject *) seq_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) secondComposable,
            (OTIOSerializableObject *) seq_3));

    RetainerPairComposable_destroy(neighbors);

    /** no change w/ different policy */
    neighbors = Track_neighbors_of(
            seq,
            seq_2,
            errorStatus,
            OTIO_Track_NeighbourGapPolicy_around_transitions);
    firstRetainerComposable = RetainerPairComposable_first(neighbors);
    secondRetainerComposable = RetainerPairComposable_second(neighbors);
    firstComposable = RetainerComposable_take_value(firstRetainerComposable);
    secondComposable = RetainerComposable_take_value(secondRetainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) firstComposable,
            (OTIOSerializableObject *) seq_1));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) secondComposable,
            (OTIOSerializableObject *) seq_3));

    RetainerPairComposable_destroy(neighbors);

    /** neighbor around third transition */
    neighbors = Track_neighbors_of(
            seq, seq_5, errorStatus, OTIO_Track_NeighbourGapPolicy_never);
    firstRetainerComposable = RetainerPairComposable_first(neighbors);
    secondRetainerComposable = RetainerPairComposable_second(neighbors);
    firstComposable = RetainerComposable_take_value(firstRetainerComposable);
    secondComposable = RetainerComposable_take_value(secondRetainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) firstComposable,
            (OTIOSerializableObject *) seq_4));
    assert_null(secondComposable);

    RetainerPairComposable_destroy(neighbors);

    RationalTime seq_5_out_offset = Transition_out_offset((Transition *) seq_5);
    start_time = RationalTime_create(0, RationalTime_rate(seq_5_out_offset));
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, seq_5_out_offset));
    fill = Gap_create_with_source_range(source_range, NULL, NULL, NULL, NULL);
    neighbors = Track_neighbors_of(
            seq,
            seq_5,
            errorStatus,
            OTIO_Track_NeighbourGapPolicy_around_transitions);
    firstRetainerComposable = RetainerPairComposable_first(neighbors);
    secondRetainerComposable = RetainerPairComposable_second(neighbors);
    firstComposable = RetainerComposable_take_value(firstRetainerComposable);
    secondComposable = RetainerComposable_take_value(secondRetainerComposable);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) firstComposable,
            (OTIOSerializableObject *) seq_4));
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) secondComposable,
            (OTIOSerializableObject *) fill));
    RetainerPairComposable_destroy(neighbors);

    OTIO_RELEASE(seq);
    seq = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_track_range_of_all_children_test(void **state) {
    const char *sample_data_dir = xstr(SAMPLE_DATA_DIR);
    const char *edl_file = "transition_test.otio";
    char *edl_path = (char *) calloc(
            strlen(sample_data_dir) + strlen(edl_file) + 1, sizeof(char));
    strcpy(edl_path, sample_data_dir);
    strcat(edl_path, edl_file);

    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIO_RETAIN(timeline);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Any *timelineAny = create_safely_typed_any_serializable_object(
            (OTIOSerializableObject *) (timeline));
    bool deserializeOK =
            deserialize_json_from_file(edl_path, timelineAny, errorStatus);
    assert_true(deserializeOK);

    timeline = (Timeline *) safely_cast_retainer_any(timelineAny);

    Stack *stack = Timeline_tracks(timeline);

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) stack);
    RetainerComposable *firstTrackRetainerComposable =
            ComposableRetainerVector_at(composableRetainerVector, 0);

    Track *tr =
            (Track *) RetainerComposable_take_value(firstTrackRetainerComposable);

    MapComposableTimeRange *mp = Track_range_of_all_children(tr, errorStatus);

    /** fetch all the valid children that should be in the map */
    ComposableVector *vc = Track_each_clip(tr);

    Composable *vc_0 = ComposableVector_at(vc, 0);
    Composable *vc_1 = ComposableVector_at(vc, 1);
    MapComposableTimeRangeIterator *it = MapComposableTimeRange_find(mp, vc_0);
    TimeRange mp_vc_0 = MapComposableTimeRangeIterator_value(it);
    MapComposableTimeRangeIterator_destroy(it);
    it = MapComposableTimeRange_find(mp, vc_1);
    TimeRange mp_vc_1 = MapComposableTimeRangeIterator_value(it);
    MapComposableTimeRangeIterator_destroy(it);
    RationalTime mp_vc_0_start_time = TimeRange_start_time(mp_vc_0);
    RationalTime mp_vc_0_duration = TimeRange_duration(mp_vc_0);
    RationalTime mp_vc_1_start_time = TimeRange_start_time(mp_vc_1);

    assert_double_equal(RationalTime_value(mp_vc_0_start_time), 0, DBL_EPSILON);
    assert_true(RationalTime_equal(mp_vc_1_start_time, mp_vc_0_duration));
    ComposableVector_destroy(vc);

    ComposableRetainerVector *timeline_tracks_retainer_vector =
            composableRetainerVector;
    //        Composition_children((Composition*) timeline);
    ComposableRetainerVectorIterator *it_tracks =
            ComposableRetainerVector_begin(timeline_tracks_retainer_vector);
    ComposableRetainerVectorIterator *it_tracks_end =
            ComposableRetainerVector_end(timeline_tracks_retainer_vector);
    for (; ComposableRetainerVectorIterator_not_equal(it_tracks, it_tracks_end);
           ComposableRetainerVectorIterator_advance(it_tracks, 1)) {
        RetainerComposable *track_retainer =
                ComposableRetainerVectorIterator_value(it_tracks);
        Track *track = (Track *) RetainerComposable_value(track_retainer);

        ComposableRetainerVector *track_children_retainer_vector =
                Composition_children((Composition *) track);
        ComposableRetainerVectorIterator *it_track_children =
                ComposableRetainerVector_begin(track_children_retainer_vector);
        ComposableRetainerVectorIterator *it_track_children_end =
                ComposableRetainerVector_end(track_children_retainer_vector);
        for (; ComposableRetainerVectorIterator_not_equal(
                it_track_children, it_track_children_end);
               ComposableRetainerVectorIterator_advance(it_track_children, 1)) {
            RetainerComposable *child_retainer =
                    ComposableRetainerVectorIterator_value(it_track_children);
            Composable *child = RetainerComposable_value(child_retainer);

            TimeRange child_range_in_parent =
                    Item_range_in_parent((Item *) child, errorStatus);

            it = MapComposableTimeRange_find(mp, child);
            TimeRange range_compare = MapComposableTimeRangeIterator_value(it);

            assert_true(TimeRange_equal(child_range_in_parent, range_compare));

            MapComposableTimeRangeIterator_destroy(it);
            it = NULL;
        }
        ComposableRetainerVector_destroy(track_children_retainer_vector);
        track_children_retainer_vector = NULL;
        ComposableRetainerVectorIterator_destroy(it_track_children);
        it_track_children = NULL;
        ComposableRetainerVectorIterator_destroy(it_track_children_end);
        it_track_children_end = NULL;
    }
    ComposableRetainerVector_destroy(timeline_tracks_retainer_vector);
    timeline_tracks_retainer_vector = NULL;
    ComposableRetainerVectorIterator_destroy(it_tracks);
    it_tracks = NULL;
    ComposableRetainerVectorIterator_destroy(it_tracks_end);
    it_tracks_end = NULL;
    MapComposableTimeRange_destroy(mp);
    mp = NULL;

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *track = Track_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(track);
    mp = Track_range_of_all_children(track, errorStatus);
    assert_int_equal(MapComposableTimeRange_size(mp), 0);
    MapComposableTimeRange_destroy(mp);
    mp = NULL;
    OTIO_RELEASE(track);
    track = NULL;

    OTIO_RELEASE(timeline);
    timeline = NULL;
}

static void otio_edge_cases_empty_composition_test(void **state) {
    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIO_RETAIN(timeline);
    Stack *stack = Timeline_tracks(timeline);
    ComposableRetainerVector *children = Composition_children((Composition *) stack);
    assert_int_equal(ComposableRetainerVector_size(children), 0);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Stack *tracks = Timeline_tracks(timeline);
    RationalTime duration = Item_duration((Item *) tracks, errorStatus);
    RationalTime duration_compare = RationalTime_create(0, 24);
    assert_true(RationalTime_equal(duration, duration_compare));

    ComposableRetainerVector_destroy(children);
    children = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    OTIO_RELEASE(timeline);
    timeline = NULL;
}

static void otio_edge_cases_iterating_over_dupes_test(void **state) {
    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIO_RETAIN(timeline);
    Track *track = Track_create(NULL, nullRange, NULL, NULL);
    Stack *stack = Timeline_tracks(timeline);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool appendOK = Composition_append_child(
            (Composition *) stack, (Composable *) track, errorStatus);
    assert_true(appendOK);

    RationalTime start_time = RationalTime_create(10, 30);
    RationalTime duration = RationalTime_create(15, 30);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    /** make several identical copies */
    for (int i = 0; i < 10; ++i) {
        Clip *clip = Clip_create("Dupe", NULL, source_range, NULL);
        appendOK = Composition_append_child(
                (Composition *) track, (Composable *) clip, errorStatus);
        assert_true(appendOK);
    }

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) track);
    assert_int_equal(ComposableRetainerVector_size(composableRetainerVector), 10);

    start_time = RationalTime_create(0, 30);
    duration = RationalTime_create(150, 30);
    TimeRange range_compare =
            TimeRange_create_with_start_time_and_duration(start_time, duration);
    TimeRange track_trimmed_range =
            Item_trimmed_range((Item *) track, errorStatus);
    assert_true(TimeRange_equal(range_compare, track_trimmed_range));

    /** test normal iteration */
    OptionalTimeRange previous = OptionalTimeRange_create_null();
    ComposableRetainerVectorIterator *it =
            ComposableRetainerVector_begin(composableRetainerVector);
    ComposableRetainerVectorIterator *it_end =
            ComposableRetainerVector_end(composableRetainerVector);
    for (; ComposableRetainerVectorIterator_not_equal(it, it_end);
           ComposableRetainerVectorIterator_advance(it, 1)) {
        RetainerComposable *retainerComposable =
                ComposableRetainerVectorIterator_value(it);
        Composable *item = RetainerComposable_value(retainerComposable);

        TimeRange range_of_child =
                Composition_range_of_child((Composition *) track, item, errorStatus);
        TimeRange range_in_parent =
                Item_range_in_parent((Item *) item, errorStatus);

        assert_true(TimeRange_equal(range_of_child, range_in_parent));
        if (OptionalTimeRange_valid(previous)) {
            assert_false(TimeRange_equal(OptionalTimeRange_value(previous), range_in_parent));
            previous = OptionalTimeRange_create_null();
        }

        previous = OptionalTimeRange_create(Item_range_in_parent((Item *) item, errorStatus));

    }
    previous = OptionalTimeRange_create_null();
    ComposableRetainerVectorIterator_destroy(it);
    it = NULL;
    ComposableRetainerVectorIterator_destroy(it_end);
    it_end = NULL;

    /** test recursive iteration */

    ComposableVector *composableVector = Track_each_clip(track);
    ComposableVectorIterator *clip_it =
            ComposableVector_begin(composableVector);
    ComposableVectorIterator *clip_it_end =
            ComposableVector_end(composableVector);
    for (; ComposableVectorIterator_not_equal(clip_it, clip_it_end);
           ComposableVectorIterator_advance(clip_it, 1)) {
        Composable *item = ComposableVectorIterator_value(clip_it);

        TimeRange range_of_child =
                Composition_range_of_child((Composition *) track, item, errorStatus);
        TimeRange range_in_parent =
                Item_range_in_parent((Item *) item, errorStatus);

        assert_true(TimeRange_equal(range_of_child, range_in_parent));
        if (OptionalTimeRange_valid(previous)) {
            assert_false(TimeRange_equal(OptionalTimeRange_value(previous), range_in_parent));
            previous = OptionalTimeRange_create_null();
        }

        previous = OptionalTimeRange_create(Item_range_in_parent((Item *) item, errorStatus));
    }
    previous = OptionalTimeRange_create_null();
    ComposableVectorIterator_destroy(clip_it);
    clip_it = NULL;
    ComposableVectorIterator_destroy(clip_it_end);
    clip_it_end = NULL;

    /** compare to iteration by index */
    it = ComposableRetainerVector_begin(composableRetainerVector);
    it_end = ComposableRetainerVector_end(composableRetainerVector);
    int i = 0;
    for (; ComposableRetainerVectorIterator_not_equal(it, it_end);
           ComposableRetainerVectorIterator_advance(it, 1), i++) {
        RetainerComposable *retainerComposable =
                ComposableRetainerVectorIterator_value(it);
        Composable *item = RetainerComposable_value(retainerComposable);

        TimeRange range_of_child =
                Composition_range_of_child((Composition *) track, item, errorStatus);
        TimeRange range_in_parent =
                Item_range_in_parent((Item *) item, errorStatus);
        TimeRange range_of_child_index =
                Track_range_of_child_at_index(track, i, errorStatus);

        assert_true(TimeRange_equal(range_of_child, range_in_parent));
        assert_true(TimeRange_equal(range_of_child, range_of_child_index));
        if (OptionalTimeRange_valid(previous)) {
            assert_false(TimeRange_equal(OptionalTimeRange_value(previous), range_in_parent));
            previous = OptionalTimeRange_create_null();
        }

        previous = OptionalTimeRange_create(Item_range_in_parent((Item *) item, errorStatus));
    }
    previous = OptionalTimeRange_create_null();
    ComposableRetainerVectorIterator_destroy(it);
    it = NULL;
    ComposableRetainerVectorIterator_destroy(it_end);
    it_end = NULL;
    i = 0;

    /** compare recursive to iteration by index */
    composableVector = Track_each_clip(track);
    clip_it = ComposableVector_begin(composableVector);
    clip_it_end = ComposableVector_end(composableVector);
    for (; ComposableVectorIterator_not_equal(clip_it, clip_it_end);
           ComposableVectorIterator_advance(clip_it, 1), i++) {
        Composable *item = ComposableVectorIterator_value(clip_it);

        TimeRange range_of_child =
                Composition_range_of_child((Composition *) track, item, errorStatus);
        TimeRange range_in_parent =
                Item_range_in_parent((Item *) item, errorStatus);
        TimeRange range_of_child_index =
                Track_range_of_child_at_index(track, i, errorStatus);

        assert_true(TimeRange_equal(range_of_child, range_in_parent));
        assert_true(TimeRange_equal(range_of_child, range_of_child_index));
        if (OptionalTimeRange_valid(previous)) {
            assert_false(TimeRange_equal(OptionalTimeRange_value(previous), range_in_parent));
            previous = OptionalTimeRange_create_null();
        }

        previous = OptionalTimeRange_create(Item_range_in_parent((Item *) item, errorStatus));
    }
    ComposableVectorIterator_destroy(clip_it);
    clip_it = NULL;
    ComposableVectorIterator_destroy(clip_it_end);
    clip_it_end = NULL;

    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;
    ComposableVector_destroy(composableVector);
    composableVector = NULL;
    OTIO_RELEASE(timeline);
    timeline = NULL;
}

static void otio_nesting_deeply_nesting_test(void **state) {
    /**
     * Take a single clip of media (frames 100-200) and nest it into a bunch
     * of layers
     * Nesting it should not shift the media at all.
     * At one level:
     * Timeline:
     *   Stack: [0-99]
     *    Track: [0-99]
     *     Clip: [100-199]
     *      Media Reference: [100-199]
     */

    /** here are some times in the top-level coordinate system */
    RationalTime zero = RationalTime_create(0, 24);
    RationalTime one = RationalTime_create(1, 24);
    RationalTime fifty = RationalTime_create(50, 24);
    RationalTime ninetynine = RationalTime_create(99, 24);
    RationalTime onehundred = RationalTime_create(100, 24);
    TimeRange top_level_range =
            TimeRange_create_with_start_time_and_duration(zero, onehundred);

    /** here are some times in the media-level coordinate system */
    RationalTime first_frame = RationalTime_create(100, 24);
    RationalTime middle = RationalTime_create(150, 24);
    RationalTime last = RationalTime_create(199, 24);
    OptionalTimeRange media_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(first_frame, onehundred));

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Timeline *timeline = Timeline_create(NULL, nullTime, NULL);
    OTIO_RETAIN(timeline);
    Stack *stack = Timeline_tracks(timeline);
    Track *track = Track_create(NULL, nullRange, NULL, NULL);
    Clip *clip = Clip_create(NULL, NULL, nullRange, NULL);
    MissingReference *media = MissingReference_create(NULL, media_range, NULL);
    Clip_set_media_reference(clip, (MediaReference *) media);
    bool appendOK = Composition_append_child(
            (Composition *) track, (Composable *) clip, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) stack, (Composable *) track, errorStatus);
    assert_true(appendOK);

    Composition *clip_parent = Composable_parent((Composable *) clip);
    Composition *track_parent = Composable_parent((Composable *) track);

    assert_ptr_equal(track, (Track *) clip_parent);
    assert_ptr_equal(stack, (Stack *) track_parent);

    /**
     * the clip and track should auto-size to fit the media, since we
     * haven't trimmed anything
     */
    RationalTime clip_duration = Item_duration((Item *) clip, errorStatus);
    RationalTime stack_duration = Item_duration((Item *) stack, errorStatus);
    RationalTime track_duration = Item_duration((Item *) track, errorStatus);
    assert_true(RationalTime_equal(clip_duration, onehundred));
    assert_true(RationalTime_equal(stack_duration, onehundred));
    assert_true(RationalTime_equal(track_duration, onehundred));

    /** the ranges should match our expectations... */
    TimeRange clip_trimmed_range =
            Item_trimmed_range((Item *) clip, errorStatus);
    TimeRange stack_trimmed_range =
            Item_trimmed_range((Item *) stack, errorStatus);
    TimeRange track_trimmed_range =
            Item_trimmed_range((Item *) track, errorStatus);
    assert_true(TimeRange_equal(clip_trimmed_range, OptionalTimeRange_value(media_range)));
    assert_true(TimeRange_equal(stack_trimmed_range, top_level_range));
    assert_true(TimeRange_equal(track_trimmed_range, top_level_range));

    /** verify that the media is where we expect */
    RationalTime stack_transformed_time_zero_clip =
            Item_transformed_time((Item *) stack, zero, (Item *) clip, errorStatus);
    RationalTime stack_transformed_time_fifty_clip =
            Item_transformed_time((Item *) stack, fifty, (Item *) clip, errorStatus);
    RationalTime stack_transformed_time_ninetynine_clip =
            Item_transformed_time(
                    (Item *) stack, ninetynine, (Item *) clip, errorStatus);
    assert_true(RationalTime_equal(stack_transformed_time_zero_clip, first_frame));
    assert_true(RationalTime_equal(stack_transformed_time_fifty_clip, middle));
    assert_true(RationalTime_equal(stack_transformed_time_ninetynine_clip, last));

    int num_wrappers = 10;
    Stack *wrappers[num_wrappers];
    struct ClipWrapperPair clipWrapperPair;
    clipWrapperPair.clip = clip;
    for (int i = 0; i < num_wrappers; ++i) {
        //        Stack* wrapper = _nest(clip, 0);
        clipWrapperPair = _nest(clipWrapperPair.clip, 0);
        wrappers[i] = clipWrapperPair.wrapper;
        clip = clipWrapperPair.clip;
    }
    /* nothing should have shifted at all */

    /*const char* encoded = serialize_json_to_string(
        create_safely_typed_any_serializable_object(
            (SerializableObject*) timeline),
        errorStatus,
        4);
    printf("%s\n", encoded);*/

    /**
     * the clip and track should auto-size to fit the media, since we
     * haven't trimmed anything
     */
    clip_duration = Item_duration((Item *) clip, errorStatus);
    stack_duration = Item_duration((Item *) stack, errorStatus);
    track_duration = Item_duration((Item *) track, errorStatus);
    assert_true(RationalTime_equal(clip_duration, onehundred));
    assert_true(RationalTime_equal(stack_duration, onehundred));
    assert_true(RationalTime_equal(track_duration, onehundred));

    /** the ranges should match our expectations... */
    clip_trimmed_range = Item_trimmed_range((Item *) clip, errorStatus);
    stack_trimmed_range = Item_trimmed_range((Item *) stack, errorStatus);
    track_trimmed_range = Item_trimmed_range((Item *) track, errorStatus);
    assert_true(TimeRange_equal(clip_trimmed_range, OptionalTimeRange_value(media_range)));
    assert_true(TimeRange_equal(stack_trimmed_range, top_level_range));
    assert_true(TimeRange_equal(track_trimmed_range, top_level_range));

    /** verify that the media is where we expect */
    stack_transformed_time_zero_clip =
            Item_transformed_time((Item *) stack, zero, (Item *) clip, errorStatus);
    stack_transformed_time_fifty_clip =
            Item_transformed_time((Item *) stack, fifty, (Item *) clip, errorStatus);
    stack_transformed_time_ninetynine_clip = Item_transformed_time(
            (Item *) stack, ninetynine, (Item *) clip, errorStatus);
    assert_true(RationalTime_equal(stack_transformed_time_zero_clip, first_frame));
    assert_true(RationalTime_equal(stack_transformed_time_fifty_clip, middle));
    assert_true(RationalTime_equal(stack_transformed_time_ninetynine_clip, last));

    /** now trim them all by one frame at each end */
    RationalTime duration = RationalTime_subtract(ninetynine, one);
    OptionalTimeRange trim = OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(one, duration));
    RationalTime time_compare = RationalTime_create(98, 24);
    RationalTime trim_duration = TimeRange_duration(OptionalTimeRange_value(trim));
    assert_true(RationalTime_equal(time_compare, trim_duration));

    for (int j = 0; j < num_wrappers; ++j) {
        Item_set_source_range((Item *) wrappers[j], trim);
    }

    /*const char* encoded = serialize_json_to_string(
        create_safely_typed_any_serializable_object(
            (SerializableObject*) timeline),
        errorStatus,
        4);
    printf("%s\n", encoded);*/

    /** the clip should be the same */
    clip_duration = Item_duration((Item *) clip, errorStatus);
    assert_true(RationalTime_equal(clip_duration, onehundred));

    /** the parents should have shrunk by only 2 frames */

    track_duration = Item_duration((Item *) track, errorStatus);
    stack_duration = Item_duration((Item *) stack, errorStatus);
    assert_true(RationalTime_equal(track_duration, time_compare));
    assert_true(RationalTime_equal(stack_duration, time_compare));

    /**
     * but the media should have shifted over by 1 one frame for each level
     * of nesting
     */

    RationalTime ten = RationalTime_create(num_wrappers, 24);
    stack_transformed_time_zero_clip =
            Item_transformed_time((Item *) stack, zero, (Item *) clip, errorStatus);
    stack_transformed_time_fifty_clip =
            Item_transformed_time((Item *) stack, fifty, (Item *) clip, errorStatus);
    stack_transformed_time_ninetynine_clip = Item_transformed_time(
            (Item *) stack, ninetynine, (Item *) clip, errorStatus);
    RationalTime first_frame_plus_ten = RationalTime_add(first_frame, ten);
    RationalTime middle_plus_ten = RationalTime_add(middle, ten);
    RationalTime last_plus_ten = RationalTime_add(last, ten);
    assert_true(RationalTime_equal(
            stack_transformed_time_zero_clip, first_frame_plus_ten));
    assert_true(RationalTime_equal(stack_transformed_time_fifty_clip, middle_plus_ten));
    assert_true(RationalTime_equal(
            stack_transformed_time_ninetynine_clip, last_plus_ten));

    OTIO_RELEASE(timeline);
    timeline = NULL;
}

static void otio_nesting_child_at_time_with_children_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Track *sq = Track_create("foo", nullRange, NULL, NULL);
    OTIO_RETAIN(sq);

    RationalTime start_time = RationalTime_create(9, 24);
    RationalTime duration = RationalTime_create(12, 24);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));

    Track *body = Track_create("body", source_range, NULL, NULL);

    start_time = RationalTime_create(100, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip1 = Clip_create("clip1", NULL, source_range, NULL);

    start_time = RationalTime_create(101, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip2 = Clip_create("clip2", NULL, source_range, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *clip3 = Clip_create("clip3", NULL, source_range, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    bool appendOK = Composition_append_child(
            (Composition *) body, (Composable *) clip1, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) body, (Composable *) clip2, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) body, (Composable *) clip3, errorStatus);
    assert_true(appendOK);

    start_time = RationalTime_create(100, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *leader = Clip_create("leader", NULL, source_range, NULL);

    start_time = RationalTime_create(102, 24);
    duration = RationalTime_create(10, 24);
    source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start_time, duration));
    Clip *credits = Clip_create("credits", NULL, source_range, NULL);

    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) leader, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) body, errorStatus);
    assert_true(appendOK);
    appendOK = Composition_append_child(
            (Composition *) sq, (Composable *) credits, errorStatus);
    assert_true(appendOK);

/**
 * Looks like this:
 * [ leader ][ body ][ credits ]
 * 10 f       12f     10f
 *
 * body: (source range starts: 9f duration: 12f)
 * [ clip1 ][ clip2 ][ clip 3]
 * 1f       11f
 */

    ComposableRetainerVector *composableRetainerVector =
            Composition_children((Composition *) sq);
    RetainerComposable *sq_0_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    RetainerComposable *sq_1_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 1);
    RetainerComposable *sq_2_retainer =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    leader = (Clip *) RetainerComposable_value(sq_0_retainer);
    body = (Track *) RetainerComposable_value(sq_1_retainer);
    credits = (Clip *) RetainerComposable_value(sq_2_retainer);
    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;

    composableRetainerVector = Composition_children((Composition *) body);
    RetainerComposable *body_0 =
            ComposableRetainerVector_at(composableRetainerVector, 0);
    RetainerComposable *body_1 =
            ComposableRetainerVector_at(composableRetainerVector, 1);
    RetainerComposable *body_2 =
            ComposableRetainerVector_at(composableRetainerVector, 2);
    clip1 = (Clip *) RetainerComposable_value(body_0);
    clip2 = (Clip *) RetainerComposable_value(body_1);
    clip3 = (Clip *) RetainerComposable_value(body_2);
    ComposableRetainerVector_destroy(composableRetainerVector);
    composableRetainerVector = NULL;

    otiostr serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) leader);
    assert_string_equal(serializableObjectName, "leader");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) body);
    assert_string_equal(serializableObjectName, "body");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) credits);
    assert_string_equal(serializableObjectName, "credits");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) clip1);
    assert_string_equal(serializableObjectName, "clip1");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) clip2);
    assert_string_equal(serializableObjectName, "clip2");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) clip3);
    assert_string_equal(serializableObjectName, "clip3");
    otiostr_delete(serializableObjectName);

    struct NameFrame {
        const char *name;
        int frame;
    };
    typedef struct NameFrame NameFrame;

    NameFrame expected[] = {
            {"leader",  100},
            {"leader",  101},
            {"leader",  102},
            {"leader",  103},
            {"leader",  104},
            {"leader",  105},
            {"leader",  106},
            {"leader",  107},
            {"leader",  108},
            {"leader",  109},
            {"clip1",   109},
            {"clip2",   101},
            {"clip2",   102},
            {"clip2",   103},
            {"clip2",   104},
            {"clip2",   105},
            {"clip2",   106},
            {"clip2",   107},
            {"clip2",   108},
            {"clip2",   109},
            {"clip2",   110},
            {"clip3",   102},
            {"credits", 102},
            {"credits", 103},
            {"credits", 104},
            {"credits", 105},
            {"credits", 106},
            {"credits", 107},
            {"credits", 108},
            {"credits", 109},
            {"credits", 110},
            {"credits", 111}
    };

    int expected_size = sizeof(expected) / sizeof(expected[0]);

//    for(int i = 0; i < expected_size; ++i)
//    {
//        RationalTime* playhead = RationalTime_create(i, 24);
//
//    }
//TODO create child_at_time_function

    OTIO_RELEASE(sq);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_composition_constructor_test),
            cmocka_unit_test(otio_composition_equality_test),
            cmocka_unit_test(otio_composition_is_parent_of_test),
            cmocka_unit_test(otio_composition_parent_manip_test),
            cmocka_unit_test(otio_composition_parent_move_child_test),
            cmocka_unit_test(otio_composition_remove_actually_removes_test),
            cmocka_unit_test(otio_stack_constructor_test),
            cmocka_unit_test(otio_stack_serialize_test),
            cmocka_unit_test(otio_stack_trim_child_range_test),
            cmocka_unit_test(otio_stack_range_of_child_test),
            cmocka_unit_test(otio_stack_range_of_child_with_duration_test),
            cmocka_unit_test(otio_stack_transformed_time_test),
            cmocka_unit_test(otio_track_serialize_test),
            cmocka_unit_test(otio_track_instancing_test),
            cmocka_unit_test(otio_track_delete_parent_container_test),
            cmocka_unit_test(otio_track_range_test),
            cmocka_unit_test(otio_track_range_of_child_test),
            cmocka_unit_test(otio_track_range_trimmed_out_test),
            cmocka_unit_test(otio_track_range_nested_test),
            cmocka_unit_test(otio_track_set_item_test),
            cmocka_unit_test(otio_track_transformed_time_test),
            cmocka_unit_test(otio_track_neighbors_of_simple_test),
            cmocka_unit_test(otio_track_neighbors_of_from_data_test),
            cmocka_unit_test(otio_track_range_of_all_children_test),
            cmocka_unit_test(otio_edge_cases_empty_composition_test),
            cmocka_unit_test(otio_edge_cases_iterating_over_dupes_test),
            cmocka_unit_test(otio_nesting_deeply_nesting_test),
            cmocka_unit_test(otio_nesting_child_at_time_with_children_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}