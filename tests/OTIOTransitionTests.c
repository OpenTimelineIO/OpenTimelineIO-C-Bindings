#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>

#include <copentimelineio/deserialization.h>
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/safely_typed_any.h>
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <copentimelineio/transition.h>

static void otio_transition_constructor_test(void **state) {
    AnyDictionary *metadata = AnyDictionary_create();
    Any *value_any = create_safely_typed_any_string("bar");
    AnyDictionaryIterator *it =
            AnyDictionary_insert(metadata, "foo", value_any);

    OptionalRationalTime nullTime = OptionalRationalTime_create_null();
    Transition *trx = Transition_create("AtoB", "SMPTE.Dissolve", nullTime, nullTime, metadata);
    OTIO_RETAIN(trx);

    AnyDictionaryIterator_destroy(it);
    it = NULL;

    assert_string_equal(Transition_transition_type(trx), "SMPTE.Dissolve");
    assert_string_equal(Transition_name(trx), "AtoB");

    AnyDictionary *metadata_compare = Transition_metadata(trx);
    it = AnyDictionary_find(metadata_compare, "foo");
    AnyDictionaryIterator *it_end = AnyDictionary_end(metadata_compare);
    assert_true(AnyDictionaryIterator_not_equal(it, it_end));
    Any *compare_any = AnyDictionaryIterator_value(it);
    const char *compare_value = safely_cast_string_any(compare_any);
    assert_string_equal(compare_value, "bar");
    assert_int_equal(AnyDictionary_size(metadata_compare), 1);
    AnyDictionaryIterator_destroy(it);
    it = NULL;
    AnyDictionaryIterator_destroy(it_end);
    it_end = NULL;
    AnyDictionary_destroy(metadata_compare);
    metadata_compare = NULL;

    OTIO_RELEASE(trx);
    trx = NULL;
    Any_destroy(value_any);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(otio_transition_constructor_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}