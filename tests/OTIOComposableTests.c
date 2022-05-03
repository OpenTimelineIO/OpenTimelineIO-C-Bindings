// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <copentimelineio/anyDictionary.h>
#include <copentimelineio/composable.h>
#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>


static void otio_composable_constructor_test(void **state) {
    Any *value = create_safely_typed_any_string("bar");
    AnyDictionary *metadata = AnyDictionary_create();
    AnyDictionaryIterator *it = AnyDictionary_insert(metadata, "foo", value);
    Composable *seqi =
            Composable_create_with_name_and_metadata("test", metadata);
    const char *name = SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) seqi);
    assert_string_equal(name, "test");

    AnyDictionary *resultMetadata = SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) seqi);
    assert_int_equal(AnyDictionary_size(metadata), AnyDictionary_size(resultMetadata));

    AnyDictionaryIterator *itMetadata = AnyDictionary_begin(metadata);
    AnyDictionaryIterator *itMetadataEnd = AnyDictionary_end(metadata);
    AnyDictionaryIterator *itMetadataResult =
            AnyDictionary_begin(resultMetadata);

    for (; AnyDictionaryIterator_not_equal(itMetadata, itMetadataEnd);
           AnyDictionaryIterator_advance(itMetadata, 1),
                   AnyDictionaryIterator_advance(itMetadataResult, 1)) {
        assert_string_equal(AnyDictionaryIterator_key(itMetadata),
                            AnyDictionaryIterator_key(itMetadataResult));
        Any *x = AnyDictionaryIterator_value(itMetadata);
        Any *y = AnyDictionaryIterator_value(itMetadataResult);
        assert_string_equal(safely_cast_string_any(x), safely_cast_string_any(y));
        Any_destroy(x);
        Any_destroy(y);
    }

    Any_destroy(value);
    value = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    AnyDictionary_destroy(resultMetadata);
    resultMetadata = NULL;
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    AnyDictionaryIterator_destroy(itMetadata);
    itMetadata = NULL;
    AnyDictionaryIterator_destroy(itMetadataEnd);
    itMetadataEnd = NULL;
    AnyDictionaryIterator_destroy(itMetadataResult);
    itMetadataResult = NULL;
}

static void otio_composable_serialize_test(void **state) {
    Any *value = create_safely_typed_any_string("bar");
    AnyDictionary *metadata = AnyDictionary_create();
    AnyDictionaryIterator *it = AnyDictionary_insert(metadata, "foo", value);
    Composable *seqi =
            Composable_create_with_name_and_metadata("test", metadata);
    Any *seqi_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) seqi);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    const char *encoded = serialize_json_to_string(seqi_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) seqi);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to((OTIOSerializableObject *) seqi, decoded_object));

    Any_destroy(value);
    value = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    Any_destroy(seqi_any);
    seqi_any = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_composable_constructor_test),
            cmocka_unit_test(otio_composable_serialize_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
