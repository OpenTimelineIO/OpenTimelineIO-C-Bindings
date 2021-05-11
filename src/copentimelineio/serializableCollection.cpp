#include "copentimelineio/serializableCollection.h"
#include <copentimelineio/serializableObject.h>
#include <copentimelineio/serializableObjectWithMetadata.h>
#include <opentimelineio/serializableCollection.h>

typedef std::vector<
        OTIO_NS::SerializableObject::Retainer<OTIO_NS::SerializableObject>>
        SerializableObjectRetainerVectorDef;
typedef std::vector<OTIO_NS::SerializableObject::Retainer<
        OTIO_NS::SerializableObject>>::iterator
        SerializableObjectRetainerVectorIteratorDef;
typedef std::vector<OTIO_NS::SerializableObject *> SerializableObjectVectorDef;
typedef std::vector<OTIO_NS::SerializableObject *>::iterator
        SerializableObjectVectorIteratorDef;

OTIO_API SerializableCollection *
SerializableCollection_create(
        const char *name,
        SerializableObjectVector *children,
        AnyDictionary *metadata) {
    std::string name_str = std::string();
    if (name != NULL) { name_str = name; }
    SerializableObjectVectorDef childrenVector =
            SerializableObjectVectorDef();
    if (children != NULL) {
        childrenVector =
                *reinterpret_cast<SerializableObjectVectorDef *>(children);
    }
    OTIO_NS::AnyDictionary metadataDictionary = OTIO_NS::AnyDictionary();
    if (metadata != NULL) {
        metadataDictionary =
                *reinterpret_cast<OTIO_NS::AnyDictionary *>(metadata);
    }
    return reinterpret_cast<SerializableCollection *>(
            new OTIO_NS::SerializableCollection(
                    name_str, childrenVector, metadataDictionary));
}

OTIO_API SerializableObjectRetainerVector *
SerializableCollection_children(SerializableCollection *self) {
    SerializableObjectRetainerVectorDef vec =
            reinterpret_cast<OTIO_NS::SerializableCollection *>(self)
                    ->children();
    return reinterpret_cast<SerializableObjectRetainerVector *>(
            new SerializableObjectRetainerVectorDef(vec));
}

OTIO_API void
SerializableCollection_set_children(
        SerializableCollection *self, SerializableObjectVector *children) {
    reinterpret_cast<OTIO_NS::SerializableCollection *>(self)->set_children(
            *reinterpret_cast<SerializableObjectVectorDef *>(children));
}

OTIO_API void
SerializableCollection_clear_children(SerializableCollection *self) {
    reinterpret_cast<OTIO_NS::SerializableCollection *>(self)
            ->clear_children();
}

OTIO_API void
SerializableCollection_insert_child(
        SerializableCollection *self, int index, OTIOSerializableObject *child) {
    reinterpret_cast<OTIO_NS::SerializableCollection *>(self)->insert_child(
            index, reinterpret_cast<OTIO_NS::SerializableObject *>(child));
}

OTIO_API bool
SerializableCollection_set_child(
        SerializableCollection *self,
        int index,
        OTIOSerializableObject *child,
        OTIOErrorStatus *error_status) {
    return reinterpret_cast<OTIO_NS::SerializableCollection *>(self)
            ->set_child(
                    index,
                    reinterpret_cast<OTIO_NS::SerializableObject *>(child),
                    reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
}

OTIO_API bool
SerializableCollection_remove_child(
        SerializableCollection *self, int index, OTIOErrorStatus *error_status) {
    return reinterpret_cast<OTIO_NS::SerializableCollection *>(self)
            ->remove_child(
                    index, reinterpret_cast<OTIO_NS::ErrorStatus *>(error_status));
}

OTIO_API const char *
SerializableCollection_name(SerializableCollection *self) {
    return SerializableObjectWithMetadata_name(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API void
SerializableCollection_set_name(
        SerializableCollection *self, const char *name) {
    SerializableObjectWithMetadata_set_name(
            (SerializableObjectWithMetadata *) self, name);
}

OTIO_API AnyDictionary *
SerializableCollection_metadata(SerializableCollection *self) {
    return SerializableObjectWithMetadata_metadata(
            (SerializableObjectWithMetadata *) self);
}

OTIO_API bool
SerializableCollection_to_json_file(
        SerializableCollection *self,
        const char *file_name,
        OTIOErrorStatus *error_status,
        int indent) {
    return SerializableObject_to_json_file(
            reinterpret_cast<OTIOSerializableObject *>(self), file_name, error_status, indent);
}

OTIO_API const char *
SerializableCollection_to_json_string(
        SerializableCollection *self, OTIOErrorStatus *error_status, int indent) {
    return SerializableObject_to_json_string(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status, indent);
}

OTIO_API bool
SerializableCollection_is_equivalent_to(
        SerializableCollection *self, OTIOSerializableObject *other) {
    return SerializableObject_is_equivalent_to(
            reinterpret_cast<OTIOSerializableObject *>(self), other);
}

OTIO_API SerializableCollection *
SerializableCollection_clone(
        SerializableCollection *self, OTIOErrorStatus *error_status) {
    return (SerializableCollection *) SerializableObject_clone(
            reinterpret_cast<OTIOSerializableObject *>(self), error_status);
}

OTIO_API const char *
SerializableCollection_schema_name(SerializableCollection *self) {
    return SerializableObject_schema_name(reinterpret_cast<OTIOSerializableObject *>(self));
}

OTIO_API int
SerializableCollection_schema_version(SerializableCollection *self) {
    return SerializableObject_schema_version(reinterpret_cast<OTIOSerializableObject *>(self));
}
