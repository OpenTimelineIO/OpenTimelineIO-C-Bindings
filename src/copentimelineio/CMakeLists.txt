set(COPENTIMELINEIO_HEADER_FILES
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/any.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/anyDictionary.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/anyVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/errorStatus.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serializableObject.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serializableObjectWithMetadata.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serialization.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/deserialization.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/unknownSchema.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serializableObjectVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serializableObjectRetainerVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/serializableCollection.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/typeRegistry.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/composable.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/item.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/effect.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/effectVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/effectRetainerVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/marker.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/markerVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/markerRetainerVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/mediaReference.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/missingReference.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/externalReference.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/timeEffect.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/linearTimeWarp.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/generatorReference.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/freezeFrame.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/transition.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/clip.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/gap.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/stack.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/mapComposableTimeRange.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/composition.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/composableRetainerVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/composableVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/optionalPairRationalTime.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/retainerPairComposable.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/track.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/trackVector.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/timeline.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/stackAlgorithm.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/trackAlgorithm.h
        ${PROJECT_SOURCE_DIR}/include/copentimelineio/safely_typed_any.h
        )

add_library(copentimelineio ${COTIO_SHARED_OR_STATIC_LIB}
        any.cpp
        anyDictionary.cpp
        anyVector.cpp
        errorStatus.cpp
        serializableObject.cpp
        serializableObjectWithMetadata.cpp
        serialization.cpp
        deserialization.cpp
        unknownSchema.cpp
        serializableObjectVector.cpp
        serializableObjectRetainerVector.cpp
        serializableCollection.cpp
        typeRegistry.cpp
        composable.cpp
        item.cpp
        effect.cpp
        effectVector.cpp
        effectRetainerVector.cpp
        marker.cpp
        markerVector.cpp
        markerRetainerVector.cpp
        mediaReference.cpp
        missingReference.cpp
        externalReference.cpp
        timeEffect.cpp
        linearTimeWarp.cpp
        generatorReference.cpp
        freezeFrame.cpp
        transition.cpp
        clip.cpp
        gap.cpp
        stack.cpp
        mapComposableTimeRange.cpp
        composition.cpp
        composableRetainerVector.cpp
        composableVector.cpp
        optionalPairRationalTime.cpp
        retainerPairComposable.cpp
        track.cpp
        trackVector.cpp
        timeline.cpp
        stackAlgorithm.cpp
        trackAlgorithm.cpp
        safely_typed_any.cpp
        ${COPENTIMELINEIO_HEADER_FILES})

target_include_directories(copentimelineio PUBLIC
        "${PROJECT_SOURCE_DIR}/include/"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps/"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps//optional-lite/include")

target_link_libraries(copentimelineio
        opentime
        opentimelineio)

set_target_properties(copentimelineio PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        MACOSX_RPATH true WINDOWS_EXPORT_ALL_SYMBOLS true)

if (COTIO_INSTALL)
    install(TARGETS copentimelineio
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib)
    install(FILES ${COPENTIMELINEIO_HEADER_FILES}
            DESTINATION include/copentimelineio)
endif ()