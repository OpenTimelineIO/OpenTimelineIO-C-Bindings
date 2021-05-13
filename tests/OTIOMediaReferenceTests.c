#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>

#include <copentime/rationalTime.h>
#include <copentime/timeRange.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/externalReference.h>
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

static void otio_media_reference_constructor_test(void **state) {
    RationalTime start_time = RationalTime_create(5, 24);
    RationalTime duration = RationalTime_create(10, 24);
    OptionalTimeRange available_range =
            OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start_time, duration));

    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("OTIOTheMovie");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "show", value_any);

    MissingReference *mr = MissingReference_create(NULL, available_range, metadata);
    OTIO_RETAIN(mr);

    OptionalTimeRange mr_available_range =
            MediaReference_available_range((MediaReference *) mr);
    assert_true(OptionalTimeRange_valid(mr_available_range));
    assert_true(TimeRange_equal(OptionalTimeRange_value(mr_available_range),
                                OptionalTimeRange_value(available_range)));

    AnyDictionary_destroy(metadata);
    metadata = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    OTIO_RELEASE(mr);
    mr = NULL;

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    MissingReference *mr2 = MissingReference_create(NULL, nullRange, NULL);
    OTIO_RETAIN(mr2);
    mr_available_range = MediaReference_available_range((MediaReference *) mr2);
    assert_false(OptionalTimeRange_valid(mr_available_range));
    OTIO_RELEASE(mr2);
    mr = NULL;
}

static void otio_media_reference_serialization_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    MissingReference *mr = MissingReference_create(NULL, nullRange, NULL);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Any *mr_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) mr);
    const char *encoded = serialize_json_to_string(mr_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) mr);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) mr, decoded_object));

    Any_destroy(mr_any);
    mr_any = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_media_reference_filepath_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    ExternalReference *filepath =
            ExternalReference_create("var/tmp/foo.mov", nullRange, NULL);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Any *filepath_any = create_safely_typed_any_serializable_object(
            (OTIOSerializableObject *) filepath);
    const char *encoded =
            serialize_json_to_string(filepath_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object(
                    (OTIOSerializableObject *) filepath);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) filepath, decoded_object));

    Any_destroy(filepath_any);
    filepath_any = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

static void otio_media_reference_equality_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    ExternalReference *filepath =
            ExternalReference_create("var/tmp/foo.mov", nullRange, NULL);
    OTIO_RETAIN(filepath);
    ExternalReference *filepath2 =
            ExternalReference_create("var/tmp/foo.mov", nullRange, NULL);
    OTIO_RETAIN(filepath2);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) filepath, (OTIOSerializableObject *) filepath2));
    OTIO_RELEASE(filepath2);

    MissingReference *bl = MissingReference_create(NULL, nullRange, NULL);
    OTIO_RETAIN(bl);

    assert_false(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) filepath, (OTIOSerializableObject *) bl));

    OTIO_RELEASE(filepath);
    OTIO_RELEASE(bl);

    ExternalReference *filepath3 = ExternalReference_create("var/tmp/foo.mov", nullRange, NULL);
    OTIO_RETAIN(filepath3);
    ExternalReference *filepath4 = ExternalReference_create("var/tmp/foo2.mov", nullRange, NULL);
    OTIO_RETAIN(filepath4);

    assert_false(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) filepath3,
            (OTIOSerializableObject *) filepath4));
    OTIO_RELEASE(filepath3);
    OTIO_RELEASE(filepath4);
}

static void otio_media_reference_is_missing_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    ExternalReference *filepath = ExternalReference_create("var/tmp/foo.mov", nullRange, NULL);
    OTIO_RETAIN(filepath);
    assert_false(MediaReference_is_missing_reference((MediaReference *) filepath));

    MissingReference *bl = MissingReference_create(NULL, nullRange, NULL);
    OTIO_RETAIN(bl);
    assert_true(MediaReference_is_missing_reference((MediaReference *) bl));

    OTIO_RELEASE(filepath);
    filepath = NULL;
    OTIO_RELEASE(bl);
    bl = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_media_reference_constructor_test),
            cmocka_unit_test(otio_media_reference_serialization_test),
            cmocka_unit_test(otio_media_reference_filepath_test),
            cmocka_unit_test(otio_media_reference_equality_test),
            cmocka_unit_test(otio_media_reference_is_missing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}