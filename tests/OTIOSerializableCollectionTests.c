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
#include <copentimelineio/missingReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableCollection.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectVector.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

struct SerializableCollectionTestState {
    Clip *testClip;
    MissingReference *missingReference;
    AnyDictionary *md;
    SerializableObjectVector *children;
    SerializableCollection *sc;
    RetainerSerializableObject *sc_r;
};

static int setupSerializableCollectionTests(void **state) {
    struct SerializableCollectionTestState *testState = malloc(sizeof(struct SerializableCollectionTestState));

    OptionalTimeRange nullRange = OptionalTimeRange_create_null();
    testState->testClip = Clip_create("testClip", NULL, nullRange, NULL);
    testState->missingReference = MissingReference_create(NULL, nullRange, NULL);
    testState->children = SerializableObjectVector_create();
    SerializableObjectVector_push_back(
            testState->children, (OTIOSerializableObject *) testState->testClip);
    SerializableObjectVector_push_back(
            testState->children, (OTIOSerializableObject *) testState->missingReference);
    testState->md = AnyDictionary_create();
    Any *stringAny;
    stringAny = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it = AnyDictionary_insert(testState->md, "foo", stringAny);

    testState->sc = SerializableCollection_create("test", testState->children, testState->md);
    testState->sc_r = RetainerSerializableObject_create((OTIOSerializableObject *) testState->sc);

    AnyDictionaryIterator_destroy(it);
    it = NULL;
    Any_destroy(stringAny);
    stringAny = NULL;

    *state = testState;

    return 0;
}

static int teardownSerializableCollectionTests(void **state) {
    struct SerializableCollectionTestState *testState = *state;

    SerializableObjectVector_destroy(testState->children);
    testState->children = NULL;
    RetainerSerializableObject_managed_destroy(testState->sc_r);
    testState->sc_r = NULL;

    return 0;
}

static void otio_serializable_collection_constructor_test(void **state) {
    struct SerializableCollectionTestState *testState = *state;
    Clip *testClip = testState->testClip;
    MissingReference *missingReference = testState->missingReference;
    AnyDictionary *md = testState->md;
    SerializableObjectVector *children = testState->children;
    SerializableCollection *sc = testState->sc;
    RetainerSerializableObject *sc_r = testState->sc_r;

    otiostr serializableObjectName = SerializableCollection_name(sc);
    assert_string_equal(serializableObjectName, "test");
    otiostr_delete(serializableObjectName);

    SerializableObjectRetainerVector *serializableObjectRetainerVector =
            SerializableCollection_children(sc);

    assert_int_equal(SerializableObjectRetainerVector_size(serializableObjectRetainerVector),
                     SerializableObjectVector_size(children));

    SerializableObjectRetainerVectorIterator *it =
            SerializableObjectRetainerVector_begin(
                    serializableObjectRetainerVector);
    SerializableObjectRetainerVectorIterator *endIt =
            SerializableObjectRetainerVector_end(serializableObjectRetainerVector);

    SerializableObjectVectorIterator *childIt =
            SerializableObjectVector_begin(children);

    for (; SerializableObjectRetainerVectorIterator_not_equal(it, endIt);
           SerializableObjectRetainerVectorIterator_advance(it, 1),
                   SerializableObjectVectorIterator_advance(childIt, 1)) {
        RetainerSerializableObject *retainerSerializableObject =
                SerializableObjectRetainerVectorIterator_value(it);
        OTIOSerializableObject *serializableObject =
                RetainerSerializableObject_value(retainerSerializableObject);
        OTIOSerializableObject *compareSerializableObject =
                SerializableObjectVectorIterator_value(childIt);
        assert_ptr_equal(serializableObject, compareSerializableObject);
    }

    SerializableObjectVectorIterator_destroy(childIt);
    childIt = NULL;
    SerializableObjectRetainerVectorIterator_destroy(it);
    it = NULL;
    SerializableObjectRetainerVectorIterator_destroy(endIt);
    endIt = NULL;

    AnyDictionary *resultMetadata = SerializableCollection_metadata(sc);
    assert_int_equal(AnyDictionary_size(md), AnyDictionary_size(resultMetadata));

    AnyDictionaryIterator *itMetadata = AnyDictionary_begin(md);
    AnyDictionaryIterator *itMetadataEnd = AnyDictionary_end(md);
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

    AnyDictionaryIterator_destroy(itMetadata);
    itMetadata = NULL;
    AnyDictionaryIterator_destroy(itMetadataEnd);
    itMetadataEnd = NULL;
    AnyDictionaryIterator_destroy(itMetadataResult);
    itMetadataResult = NULL;
}

static void otio_serializable_collection_serialize_test(void **state) {
    struct SerializableCollectionTestState *testState = *state;
    Clip *testClip = testState->testClip;
    MissingReference *missingReference = testState->missingReference;
    AnyDictionary *md = testState->md;
    SerializableObjectVector *children = testState->children;
    SerializableCollection *sc = testState->sc;
    RetainerSerializableObject *sc_r = testState->sc_r;

    Any *sc_any =
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) sc);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();

    otiostr encoded = serialize_json_to_string(sc_any, errorStatus, 4);
    Any *decoded = /* allocate memory for destinantion */
            create_safely_typed_any_serializable_object((OTIOSerializableObject *) sc);

    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    assert_true(decoded_successfully);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);

    assert_true(SerializableObject_is_equivalent_to(
            (OTIOSerializableObject *) sc, decoded_object));

    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
    Any_destroy(decoded);
    decoded = NULL;
    otiostr_delete(encoded);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(otio_serializable_collection_constructor_test,
                                            setupSerializableCollectionTests,
                                            teardownSerializableCollectionTests),
            cmocka_unit_test_setup_teardown(otio_serializable_collection_serialize_test,
                                            setupSerializableCollectionTests,
                                            teardownSerializableCollectionTests),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}