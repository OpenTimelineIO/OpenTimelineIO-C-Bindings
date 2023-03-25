// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

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
#include <copentimelineio/item.h>
#include <copentimelineio/marker.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

static void otio_marker_constructor_test(void **state) {
    RationalTime start_time = RationalTime_create(5, 24);
    RationalTime duration = RationalTime_create(10, 24);
    OptionalTimeRange tr =
            OptionalTimeRange_create(TimeRange_create_with_start_time_and_duration(start_time, duration));

    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);

    Marker *m = Marker_create("marker_1", tr, MarkerColor_green, metadata);

    AnyDictionaryIterator_destroy(it);
    it = NULL;

    otiostr serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) m);
    assert_string_equal(serializableObjectName,
                        "marker_1");

    AnyDictionary *metadata_compare = SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) m);
    it = AnyDictionary_find(metadata_compare, "foo");
    AnyDictionaryIterator *it_end = AnyDictionary_end(metadata_compare);
    assert_true(AnyDictionaryIterator_not_equal(it, it_end));
    Any *compare_any = AnyDictionaryIterator_value(it);
    otiostr compare_value = safely_cast_string_any(compare_any);
    assert_string_equal(compare_value, "bar");
    assert_int_equal(AnyDictionary_size(metadata_compare), 1);
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    AnyDictionaryIterator_destroy(it_end);
    it_end = NULL;
    AnyDictionary_destroy(metadata_compare);
    metadata_compare = NULL;
    otiostr_delete(compare_value);

    TimeRange marked_range = Marker_marked_range(m);
    assert_true(TimeRange_equal(marked_range, OptionalTimeRange_value(tr)));

    const char *color = Marker_color(m);
    assert_string_equal(color, MarkerColor_green);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Any *marker_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) m);
    otiostr encoded = serialize_json_to_string(marker_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) (m));
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) m, decoded_object));

    Any_destroy(marker_any);
    marker_any = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    otiostr_delete(encoded);

    AnyDictionary_destroy(metadata);
    metadata = NULL;
    Any_destroy(value_any);
    value_any = NULL;
}

static void otio_marker_upgrade_test(void **state) {
    const char *src =
            " {\n"
            "            \"OTIO_SCHEMA\" : \"Marker.1\",\n"
            "            \"metadata\" : {},\n"
            "            \"name\" : null,\n"
            "            \"range\" : {\n"
            "                \"OTIO_SCHEMA\" : \"TimeRange.1\",\n"
            "                \"start_time\" : {\n"
            "                    \"OTIO_SCHEMA\" : \"RationalTime.1\",\n"
            "                    \"rate\" : 5,\n"
            "                    \"value\" : 0\n"
            "                },\n"
            "                \"duration\" : {\n"
            "                    \"OTIO_SCHEMA\" : \"RationalTime.1\",\n"
            "                    \"rate\" : 5,\n"
            "                    \"value\" : 0\n"
            "                }\n"
            "            }\n"
            "\n"
            "        } ";

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    OTIOSerializableObject *so = SerializableObject_create();
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) so);
    bool decoded_successfully =
            deserialize_json_from_string(src, decoded, errorStatus);
    assert_true(decoded_successfully);

    Marker *marker = (Marker *) safely_cast_retainer_any(decoded);
    OTIO_RETAIN(marker);

    RationalTime start_time = RationalTime_create(0, 5);
    TimeRange range_compare =
            TimeRange_create_with_start_time_and_duration(start_time, start_time);
    TimeRange marked_range = Marker_marked_range(marker);
    assert_true(TimeRange_equal(range_compare, marked_range));

    OTIO_RELEASE(marker);
    marker = NULL;
}

static void otio_marker_equality_test(void **state) {
    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    Marker *m = Marker_create(NULL, nullRange, NULL, NULL);
    OTIO_RETAIN(m);
    Item *bo = Item_create(NULL, nullRange, NULL, NULL, NULL);
    OTIO_RETAIN(bo);

    assert_false(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) m, (OTIOSerializableObject *) bo));
    assert_false(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) bo, (OTIOSerializableObject *) m));

    OTIO_RELEASE(bo);
    bo = NULL;
    OTIO_RELEASE(m);
    m = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_marker_constructor_test),
            cmocka_unit_test(otio_marker_upgrade_test),
            cmocka_unit_test(otio_marker_equality_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
