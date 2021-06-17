#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <copentime/rationalTime.h>
#include <copentime/timeRange.h>
#include <copentimelineio/clip.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/externalReference.h>
#include <copentimelineio/item.h>
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>
#include <copentime/optionalOpenTime.h>

static void otio_clip_constructor_test(void **state) {
    const char *name = "test";
    RationalTime rt = RationalTime_create(5, 24);
    OptionalTimeRange tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(rt, rt));
    RationalTime dur = RationalTime_create(10, 24);
    OptionalTimeRange available_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(rt, dur));
    ExternalReference *mr =
            ExternalReference_create("/var/tmp/test.mov", available_range, NULL);
    Clip *clip = Clip_create(name, (MediaReference *) mr, tr, NULL);

    assert_string_equal(
            name,
            SerializableObjectWithMetadata_name(
                    (SerializableObjectWithMetadata *) clip));

    OptionalTimeRange source_range = Item_source_range((Item *) clip);
    assert_true(OptionalTimeRange_valid(source_range));
    assert_true(TimeRange_equal(OptionalTimeRange_value(source_range),
                                OptionalTimeRange_value(tr)));

    MediaReference *media_reference = Clip_media_reference(clip);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) media_reference,
            (OTIOSerializableObject *) mr));

    Any *clip_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) clip);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(clip_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) clip);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) clip, decoded_object));

    Any_destroy(clip_any);
    clip_any = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    otiostr_delete(encoded);
}

static void otio_clip_ranges_test(void **state) {
    const char *name = "test_clip";
    RationalTime start = RationalTime_create(86400, 24);
    RationalTime duration = RationalTime_create(200, 24);
    OptionalTimeRange tr = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    ExternalReference *mr =
            ExternalReference_create("/var/tmp/test.mov", tr, NULL);
    Clip *clip = Clip_create(name, (MediaReference *) mr, tr, NULL);
    RetainerSerializableObject *clip_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) clip);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    RationalTime clip_duration = Item_duration((Item *) clip, errorStatus);
    TimeRange clip_trimmed_range =
            Item_trimmed_range((Item *) clip, errorStatus);
    RationalTime clip_trimmed_range_duration =
            TimeRange_duration(clip_trimmed_range);

    assert_true(RationalTime_equal(clip_duration, clip_trimmed_range_duration));

    RationalTime tr_duration = TimeRange_duration(tr.value);

    assert_true(RationalTime_equal(clip_duration, tr_duration));

    assert_true(OptionalTimeRange_valid(tr));
    assert_true(TimeRange_equal(OptionalTimeRange_value(tr),
                                clip_trimmed_range));

    TimeRange clip_available_range = Clip_available_range(clip, errorStatus);

    assert_true(TimeRange_equal(OptionalTimeRange_value(tr),
                                clip_available_range));

    start = RationalTime_create(86500, 24);
    duration = RationalTime_create(50, 24);
    OptionalTimeRange source_range = OptionalTimeRange_create(
            TimeRange_create_with_start_time_and_duration(start, duration));
    Item_set_source_range((Item *) clip, source_range);
    clip_duration = Item_duration((Item *) clip, errorStatus);

    assert_false(RationalTime_equal(clip_duration, tr_duration));

    clip_trimmed_range = Item_trimmed_range((Item *) clip, errorStatus);

    assert_false(TimeRange_equal(clip_trimmed_range,
                                 OptionalTimeRange_value(tr)));

    OptionalTimeRange clip_source_range = Item_source_range((Item *) clip);
    assert_true(OptionalTimeRange_valid(clip_source_range));
    RationalTime clip_source_range_duration =
            TimeRange_duration(OptionalTimeRange_value(clip_source_range));

    assert_true(RationalTime_equal(clip_source_range_duration, clip_duration));

    assert_true(TimeRange_equal(clip_trimmed_range, OptionalTimeRange_value(clip_source_range)));

    RetainerSerializableObject_managed_destroy(clip_r);
    clip = NULL;
}

static void otio_clip_ref_default_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Clip *clip = Clip_create(NULL, NULL, nullRange, NULL);
    RetainerSerializableObject *clip_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) clip);
    MissingReference *missing_reference = MissingReference_create(NULL, nullRange, NULL);
    RetainerSerializableObject *missing_reference_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) missing_reference);
    MediaReference *clip_media_reference = Clip_media_reference(clip);
    RetainerSerializableObject *clip_media_reference_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) clip_media_reference);
    assert_true(
            SerializableObject_is_equivalent_to((OTIOSerializableObject *) missing_reference,
                                                (OTIOSerializableObject *) clip_media_reference));

    RetainerSerializableObject_managed_destroy(missing_reference_r);
    clip_media_reference = NULL;

    ExternalReference *external_reference = ExternalReference_create(NULL, nullRange, NULL);
    RetainerSerializableObject *external_reference_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) external_reference);
    Clip_set_media_reference(clip, (MediaReference *) external_reference);
    clip_media_reference = Clip_media_reference(clip);
    assert_true(SerializableObject_is_equivalent_to((OTIOSerializableObject *) external_reference,
                                                    (OTIOSerializableObject *) clip_media_reference));

    RetainerSerializableObject_managed_destroy(clip_r);
    RetainerSerializableObject_managed_destroy(clip_media_reference_r);
    RetainerSerializableObject_managed_destroy(external_reference_r);
    clip = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_clip_constructor_test),
            cmocka_unit_test(otio_clip_ranges_test),
            cmocka_unit_test(otio_clip_ref_default_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
