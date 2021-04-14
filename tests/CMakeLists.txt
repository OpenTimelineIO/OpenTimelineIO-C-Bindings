set(TESTS
    openTimeTests
    OTIOComposableTests
    OTIOClipTests
#    OTIOCompositionTests
    OTIOEffectTests
#    OTIOGeneratorReferenceTests
#    OTIOMarkerTests
#    OTIOMediaReferenceTests
#    OTIOTransitionTests
#    OTIOUnknownSchemaTests
#    OTIOSerializableCollectionTests
#    OTIOItemTests not working
#    OTIOSerializableObjectTests
#    OTIOTimelineTests
#    OTIOStackAlgoTests not working
#    OTIOTrackAlgoTests
#    OTIOMultithreadingTests
)
foreach(TEST ${TESTS})
    add_executable(C${TEST} ${TEST}.c epsilon.h)
    target_include_directories(C${TEST} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps/optional-lite/include"
        "${PROJECT_SOURCE_DIR}/cmocka/include")
    target_link_libraries(C${TEST} PUBLIC
        cmocka
        copentime
        copentimelineio
        pthread)
    target_compile_definitions(C${TEST} PRIVATE
        SAMPLE_DATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/sample_data/)
    add_test(NAME C${TEST} COMMAND C${TEST})
endforeach()
