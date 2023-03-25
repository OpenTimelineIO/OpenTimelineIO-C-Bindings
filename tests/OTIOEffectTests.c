// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "epsilon.h"

#include <copentimelineio/anyDictionary.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/effect.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/freezeFrame.h>
#include <copentimelineio/item.h>
#include <copentimelineio/linearTimeWarp.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

static void otio_effect_constructor_test(void **state) {
    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);
    Effect *ef = Effect_create("blur it", "blur", metadata);
    RetainerSerializableObject *ef_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) ef);

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    Any *effect_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) ef);
    otiostr encoded = serialize_json_to_string(effect_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) ef);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);

    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    RetainerSerializableObject *decoded_object_r = RetainerSerializableObject_create(decoded_object);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) ef, decoded_object));

    otiostr serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) decoded_object);
    assert_string_equal(serializableObjectName, "blur it");
    otiostr_delete(serializableObjectName);
    serializableObjectName = Effect_effect_name((Effect *) decoded_object);
    assert_string_equal(serializableObjectName, "blur");
    otiostr_delete(serializableObjectName);

    AnyDictionaryIterator_destroy(it);
    it = NULL;

    AnyDictionary *metadata_compare = SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) decoded_object);
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

    AnyDictionary_destroy(metadata);
    metadata = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    Any_destroy(compare_any);
    compare_any = NULL;

    RetainerSerializableObject_managed_destroy(ef_r);
    ef = NULL;
    RetainerSerializableObject_managed_destroy(decoded_object_r);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    otiostr_delete(encoded);
    otiostr_delete(compare_value);
}

static void otio_effect_eq_test(void **state) {
    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);
    Effect *ef = Effect_create("blur it", "blur", metadata);
    Effect *ef2 = Effect_create("blur it", "blur", metadata);
    OTIO_RETAIN(ef);
    OTIO_RETAIN(ef2);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) ef, (OTIOSerializableObject *) ef2));

    AnyDictionaryIterator_destroy(it);
    it = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    OTIO_RELEASE(ef);
    ef = NULL;
    OTIO_RELEASE(ef2);
    ef2 = NULL;
}

static void otio_linear_time_warp_constructor_test(void **state) {
    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);

    LinearTimeWarp *ef = LinearTimeWarp_create("Foo", NULL, 2.5f, metadata);
    RetainerSerializableObject *ef_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) ef);

    otiostr serializableObjectName = Effect_effect_name((Effect *) ef);
    assert_string_equal(serializableObjectName, "LinearTimeWarp");
    otiostr_delete(serializableObjectName);
    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) ef);
    assert_string_equal(serializableObjectName, "Foo");
    otiostr_delete(serializableObjectName);

    assert_double_equal(LinearTimeWarp_time_scalar(ef), 2.5, DBL_EPSILON);

    AnyDictionary *metadata_compare = SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) ef);
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
    Any_destroy(compare_any);
    compare_any = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    otiostr_delete(compare_value);

    RetainerSerializableObject_managed_destroy(ef_r);
    ef = NULL;
}

static void otio_freeze_frame_constructor_test(void **state) {
    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);

    FreezeFrame *ef = FreezeFrame_create("Foo", metadata);
    RetainerSerializableObject *ef_r = RetainerSerializableObject_create(
            (OTIOSerializableObject *) ef);

    otiostr serializableObjectName = Effect_effect_name((Effect *) ef);
    assert_string_equal(serializableObjectName, "FreezeFrame");
    otiostr_delete(serializableObjectName);

    serializableObjectName = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) ef);
    assert_string_equal(serializableObjectName, "Foo");
    assert_double_equal(LinearTimeWarp_time_scalar((LinearTimeWarp *) ef), 0, DBL_EPSILON);

    AnyDictionary *metadata_compare = SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) ef);
    it = AnyDictionary_find(metadata_compare, "foo");
    AnyDictionaryIterator *it_end = AnyDictionary_end(metadata_compare);

    assert_true(AnyDictionaryIterator_not_equal(it, it_end));

    Any *compare_any = AnyDictionaryIterator_value(it);
    otiostr compare_value = safely_cast_string_any(compare_any);
    assert_string_equal(compare_value, "bar");

    assert_double_equal(AnyDictionary_size(metadata_compare), 1, DBL_EPSILON);

    AnyDictionaryIterator_destroy(it);
    it = NULL;
    AnyDictionaryIterator_destroy(it_end);
    it_end = NULL;
    AnyDictionary_destroy(metadata_compare);
    metadata_compare = NULL;
    Any_destroy(compare_any);
    compare_any = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    otiostr_delete(compare_value);

    RetainerSerializableObject_managed_destroy(ef_r);
    ef = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_effect_constructor_test),
            cmocka_unit_test(otio_effect_eq_test),
            cmocka_unit_test(otio_linear_time_warp_constructor_test),
            cmocka_unit_test(otio_freeze_frame_constructor_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
