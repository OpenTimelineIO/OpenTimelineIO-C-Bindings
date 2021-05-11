#include "copentime/util.h"
#include "copentimelineio/externalReference.h"
#include <copentimelineio/errorStatus.h>
#include <copentimelineio/mediaReference.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentime/timeRange.h>
#include <opentimelineio/anyDictionary.h>
#include <opentimelineio/externalReference.h>
#include <string.h>

OTIO_API ExternalReference *ExternalReference_create(
        const char *target_url,
        OptionalTimeRange available_range,
        AnyDictionary *metadata) {
    nonstd::optional<opentime::TimeRange> timeRangeOptional = nonstd::nullopt;
    if (available_range.valid)
        timeRangeOptional = nonstd::optional<opentime::TimeRange>(
                _COTTimeRange_to_OTTimeRange(available_range.value));

    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL)
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);

    std::string target_url_str = std::string();
    if (target_url != NULL) target_url_str = target_url;
    return reinterpret_cast<ExternalReference *>(
            new OTIO_NS::ExternalReference(
                    target_url_str, timeRangeOptional, metadataDictionary));
}

OTIO_API const char *ExternalReference_target_url(ExternalReference *self) {
    std::string returnStr =
            reinterpret_cast<OTIO_NS::ExternalReference *>(self)->target_url();
    char *charPtr = (char *) malloc((returnStr.size() + 1) * sizeof(char));
    strcpy(charPtr, returnStr.c_str());
    return charPtr;
}

OTIO_API void ExternalReference_set_target_url(
        ExternalReference *self, const char *target_url) {
    reinterpret_cast<OTIO_NS::ExternalReference *>(self)->set_target_url(
            target_url);
}

OTIO_API OptionalTimeRange ExternalReference_available_range(ExternalReference *self) {
    return MediaReference_available_range((MediaReference *) self);
}

OTIO_API void ExternalReference_set_available_range(
        ExternalReference *self, OptionalTimeRange available_range) {
    MediaReference_set_available_range(
            (MediaReference *) self, available_range);
}

OTIO_API bool ExternalReference_is_missing_reference(ExternalReference *self) {
    return MediaReference_is_missing_reference((MediaReference *) self);
}

OTIO_API const char *ExternalReference_name(ExternalReference *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API void ExternalReference_set_name(ExternalReference *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}

OTIO_API AnyDictionary *ExternalReference_metadata(ExternalReference *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool ExternalReference_to_json_file(
        ExternalReference *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}

OTIO_API const char *ExternalReference_to_json_string(
        ExternalReference *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}

OTIO_API bool ExternalReference_is_equivalent_to(
        ExternalReference *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}

OTIO_API ExternalReference *ExternalReference_clone(
        ExternalReference *self, OTIOErrorStatus *error_status) {
    return (ExternalReference *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}

OTIO_API const char *ExternalReference_schema_name(ExternalReference *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}

OTIO_API int ExternalReference_schema_version(ExternalReference *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
