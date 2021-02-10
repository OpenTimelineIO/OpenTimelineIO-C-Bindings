#include "gtest/gtest.h"

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

class OTIOMediaReferenceTests : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(OTIOMediaReferenceTests, ConstructorTest)
{
    RationalTime* start_time = RationalTime_create(5, 24);
    RationalTime* duration   = RationalTime_create(10, 24);
    TimeRange*    available_range =
        TimeRange_create_with_start_time_and_duration(start_time, duration);

    AnyDictionary* metadata  = AnyDictionary_create();
    Any*           value_any = create_safely_typed_any_string("OTIOTheMovie");
    AnyDictionaryIterator* it =
        AnyDictionary_insert(metadata, "show", value_any);

    MissingReference* mr = MissingReference_create(NULL, available_range, metadata);
    OTIO_RETAIN(mr);

    TimeRange* mr_available_range =
        MediaReference_available_range((MediaReference*) mr);
    EXPECT_TRUE(TimeRange_equal(mr_available_range, available_range));

    RationalTime_destroy(start_time);
    start_time = NULL;
    RationalTime_destroy(duration);
    duration = NULL;
    TimeRange_destroy(available_range);
    available_range = NULL;
    TimeRange_destroy(mr_available_range);
    mr_available_range = NULL;
    AnyDictionary_destroy(metadata);
    metadata = NULL;
    Any_destroy(value_any);
    value_any = NULL;
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    OTIO_RELEASE(mr);
    mr = NULL;

    MissingReference* mr2                 = MissingReference_create(NULL, NULL, NULL);
    OTIO_RETAIN(mr2);
    mr_available_range = MediaReference_available_range((MediaReference*) mr2);
    EXPECT_EQ(mr_available_range, nullptr);
    OTIO_RELEASE(mr2);
    mr = NULL;
}

TEST_F(OTIOMediaReferenceTests, SerializationTest)
{
    MissingReference* mr = MissingReference_create(NULL, NULL, NULL);

    OTIOErrorStatus* errorStatus = OTIOErrorStatus_create();

    Any* mr_any =
        create_safely_typed_any_serializable_object(reinterpret_cast<OTIOSerializableObject*>(mr));
    const char* encoded = serialize_json_to_string(mr_any, errorStatus, 4);
    Any*        decoded = /** allocate memory for destinantion */
        create_safely_typed_any_serializable_object(reinterpret_cast<OTIOSerializableObject*>(mr));
    bool decoded_successfully =
        deserialize_json_from_string(encoded, decoded, errorStatus);
    ASSERT_TRUE(decoded_successfully);

    OTIOSerializableObject* decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(mr), decoded_object));

    Any_destroy(mr_any);
    mr_any = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

TEST_F(OTIOMediaReferenceTests, FilepathTest)
{
    ExternalReference* filepath =
        ExternalReference_create("var/tmp/foo.mov", NULL, NULL);
    OTIOErrorStatus* errorStatus = OTIOErrorStatus_create();

    Any* filepath_any = create_safely_typed_any_serializable_object(
        reinterpret_cast<OTIOSerializableObject*>(filepath));
    const char* encoded =
        serialize_json_to_string(filepath_any, errorStatus, 4);
    Any* decoded = /** allocate memory for destinantion */
        create_safely_typed_any_serializable_object(
            reinterpret_cast<OTIOSerializableObject*>(filepath));
    bool decoded_successfully =
        deserialize_json_from_string(encoded, decoded, errorStatus);
    ASSERT_TRUE(decoded_successfully);

    OTIOSerializableObject* decoded_object = safely_cast_retainer_any(decoded);
    OTIO_RETAIN(decoded_object);
    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(filepath), decoded_object));

    Any_destroy(filepath_any);
    filepath_any = NULL;
    OTIO_RELEASE(decoded_object);
    decoded_object = NULL;
    OTIOErrorStatus_destroy(errorStatus);
    errorStatus = NULL;
}

TEST_F(OTIOMediaReferenceTests, EqualityTest)
{
    ExternalReference* filepath =
        ExternalReference_create("var/tmp/foo.mov", NULL, NULL);
    OTIO_RETAIN(filepath);
    ExternalReference* filepath2 =
        ExternalReference_create("var/tmp/foo.mov", NULL, NULL);
    OTIO_RETAIN(filepath2);

    EXPECT_TRUE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(filepath), reinterpret_cast<OTIOSerializableObject*>(filepath2)));
    OTIO_RELEASE(filepath2);

    MissingReference* bl = MissingReference_create(NULL, NULL, NULL);
    OTIO_RETAIN(bl);

    EXPECT_FALSE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(filepath), reinterpret_cast<OTIOSerializableObject*>(bl)));

    OTIO_RELEASE(filepath);
    OTIO_RELEASE(bl);

    ExternalReference* filepath3 = ExternalReference_create("var/tmp/foo.mov", NULL, NULL);
    OTIO_RETAIN(filepath3);
    ExternalReference* filepath4 = ExternalReference_create("var/tmp/foo2.mov", NULL, NULL);
    OTIO_RETAIN(filepath4);

    EXPECT_FALSE(SerializableObject_is_equivalent_to(
        reinterpret_cast<OTIOSerializableObject*>(filepath3),
        reinterpret_cast<OTIOSerializableObject*>(filepath4)));
    OTIO_RELEASE(filepath3);
    OTIO_RELEASE(filepath4);
}

TEST_F(OTIOMediaReferenceTests, IsMissingTest)
{
    ExternalReference* filepath = ExternalReference_create("var/tmp/foo.mov", NULL, NULL);
    OTIO_RETAIN(filepath);
    EXPECT_FALSE( MediaReference_is_missing_reference((MediaReference*) filepath));

    MissingReference* bl = MissingReference_create(NULL, NULL, NULL);
    OTIO_RETAIN(bl);
    EXPECT_TRUE(MediaReference_is_missing_reference((MediaReference*) bl));

    OTIO_RELEASE(filepath);
    filepath = NULL;
    OTIO_RELEASE(bl);
    bl = NULL;
}
