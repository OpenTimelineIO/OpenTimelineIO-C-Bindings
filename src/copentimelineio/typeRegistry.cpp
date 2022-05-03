// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the OpenTimelineIO project

#include "copentimelineio/typeRegistry.h"
#include <opentimelineio/typeRegistry.h>

OTIO_API TypeRegistry* TypeRegistry_instance()
{
    return reinterpret_cast<TypeRegistry*>(
        &OTIO_NS::TypeRegistry::instance());
}
/*bool TypeRegistry_register_type(
    TypeRegistry*              self,
    const char*                schema_name,
    int                        schema_version,
    TypeInfo*                  type,
    TypeRegistryCreateFunction create, //unable to cast this function pointer to std::function because of the wrapped return type
    const char*                class_name)
{
    std::function<OTIO_NS::SerializableObject*()> create_func =
        static_cast<OTIO_NS::SerializableObject*()>(create);
    return reinterpret_cast<OTIO_NS::TypeRegistry*>(self)->register_type(
        schema_name,
        reinterpret_cast<std::type_info*>(type),
        create_func,
        class_name);
}*/
OTIO_API bool TypeRegistry_register_type_from_existing_type(
    TypeRegistry*    self,
    const char*      schema_name,
    int              schema_version,
    const char*      existing_schema_name,
    OTIOErrorStatus* error_status)
{
    return reinterpret_cast<OTIO_NS::TypeRegistry*>(self)
        ->register_type_from_existing_type(
            schema_name,
            schema_version,
            existing_schema_name,
            reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
}
//    bool TypeRegistry_register_upgrade_function(
//        TypeRegistry*               self,
//        const char*                 schema_name,
//        int                         version_to_upgrade_to,
//        TypeRegistryUpgradeFunction upgrade_function);
//
OTIO_API OTIOSerializableObject* TypeRegistry_instance_from_schema(
    TypeRegistry*    self,
    const char*      schema_name,
    int              schema_version,
    AnyDictionary*   dict,
    OTIOErrorStatus* error_status)
{
    return reinterpret_cast<OTIOSerializableObject*>(
        reinterpret_cast<OTIO_NS::TypeRegistry*>(self)
            ->instance_from_schema(
                schema_name,
                schema_version,
                *reinterpret_cast<OTIO_NS::AnyDictionary*>(dict),
                reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status)));
}
OTIO_API bool TypeRegistry_set_type_record(
    TypeRegistry*       self,
    OTIOSerializableObject* var1,
    const char*         schema_name,
    OTIOErrorStatus*    error_status)
{
    return reinterpret_cast<OTIO_NS::TypeRegistry*>(self)->set_type_record(
        reinterpret_cast<OTIO_NS::SerializableObject*>(var1),
        schema_name,
        reinterpret_cast<OTIO_NS::ErrorStatus*>(error_status));
}
OTIO_API void TypeRegistry_destroy(TypeRegistry* self)
{
    delete reinterpret_cast<OTIO_NS::TypeRegistry*>(self);
}
