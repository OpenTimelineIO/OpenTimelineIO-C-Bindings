// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

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
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/serialization.h>

struct UnknownSchemaTestState {
    OTIOSerializableObject *schema;
    RetainerSerializableObject *schema_r;
    const char *has_undefined_schema;
};

static int setupUnknownSchemaTests(void **state) {
    struct UnknownSchemaTestState *testState = malloc(sizeof(struct UnknownSchemaTestState));

    testState->has_undefined_schema =
            "{\n"
            "    \"OTIO_SCHEMA\": \"Clip.1\",\n"
            "    \"effects\": [],\n"
            "    \"markers\": [],\n"
            "    \"media_reference\": {\n"
            "        \"OTIO_SCHEMA\": \"ExternalReference.1\",\n"
            "        \"available_range\": {\n"
            "            \"OTIO_SCHEMA\": \"TimeRange.1\",\n"
            "            \"duration\": {\n"
            "                \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                \"rate\": 24,\n"
            "                \"value\": 140\n"
            "            },\n"
            "            \"start_time\": {\n"
            "                \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                \"rate\": 24,\n"
            "                \"value\": 91\n"
            "            }\n"
            "        },\n"
            "        \"metadata\": {\n"
            "            \"stuff\": {\n"
            "                \"OTIO_SCHEMA\": \"MyOwnDangSchema.3\",\n"
            "                \"some_data\": 895,\n"
            "                \"howlongami\": {\n"
            "                     \"OTIO_SCHEMA\": \"RationalTime.1\",\n"
            "                      \"rate\": 30,\n"
            "                      \"value\": 100\n"
            "                   }\n"
            "            }\n"
            "        },\n"
            "        \"name\": null,\n"
            "        \"target_url\": \"/usr/tmp/some_media.mov\"\n"
            "    },\n"
            "    \"metadata\": {},\n"
            "    \"name\": null,\n"
            "    \"source_range\": null\n"
            "}";

    testState->schema = SerializableObject_create();
    testState->schema_r = RetainerSerializableObject_create(testState->schema);
    Any *unknown_any = create_safely_typed_any_serializable_object(testState->schema);
    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    bool decoded_successfully = deserialize_json_from_string(
            testState->has_undefined_schema, unknown_any, errorStatus);
    testState->schema = safely_cast_retainer_any(unknown_any);
    OTIOErrorStatus_destroy(errorStatus);

    *state = testState;
    return 0;
}

static int teardownUnknownSchemaTests(void **state) {
    struct UnknownSchemaTestState *testState = *state;

    RetainerSerializableObject_managed_destroy(testState->schema_r);
    testState->schema_r = NULL;
    return 0;
}

static void otio_unknown_schema_serialize_deserialize_test(void **state) {
    struct UnknownSchemaTestState *testState = *state;
    OTIOSerializableObject *schema = testState->schema;
    RetainerSerializableObject *schema_r = testState->schema_r;
    const char *has_undefined_schema = testState->has_undefined_schema;

    OTIOErrorStatus *errorStatus = OTIOErrorStatus_create();
    Any *serialize_any = create_safely_typed_any_serializable_object(schema);
    otiostr encoded =
            serialize_json_to_string(serialize_any, errorStatus, 4);
    Any *decoded = /** allocate memory for destinantion */
            create_safely_typed_any_serializable_object(schema);
    bool decoded_successfully =
            deserialize_json_from_string(encoded, decoded, errorStatus);
    OTIOSerializableObject *decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    assert_true(SerializableObject_is_equivalent_to(schema, decoded_object));
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    otiostr_delete(encoded);
}

static void otio_unknown_schema_is_unknwon_schema_test(void **state) {
    struct UnknownSchemaTestState *testState = *state;
    OTIOSerializableObject *schema = testState->schema;
    RetainerSerializableObject *schema_r = testState->schema_r;
    const char *has_undefined_schema = testState->has_undefined_schema;

    assert_false(SerializableObject_is_unknown_schema(schema));

    Clip *clip = (Clip *) schema;
    MediaReference *mediaReference = Clip_media_reference(clip);
    AnyDictionary *metadata = MediaReference_metadata(mediaReference);
    AnyDictionaryIterator *it = AnyDictionary_find(metadata, "stuff");
    Any *any = AnyDictionaryIterator_value(it);
    OTIOSerializableObject *serializableObject = safely_cast_retainer_any(any);
    assert_true(SerializableObject_is_unknown_schema(serializableObject));
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(otio_unknown_schema_serialize_deserialize_test, setupUnknownSchemaTests,
                                            teardownUnknownSchemaTests),
            cmocka_unit_test_setup_teardown(otio_unknown_schema_is_unknwon_schema_test, setupUnknownSchemaTests,
                                            teardownUnknownSchemaTests),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
