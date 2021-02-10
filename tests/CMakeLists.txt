# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

set(TESTS
    openTimeTests
    OTIOComposableTests
    OTIOClipTests
    OTIOCompositionTests
    OTIOEffectTests
    OTIOGeneratorReferenceTests
    OTIOMarkerTests
    OTIOMediaReferenceTests
    OTIOTransitionTests
    OTIOUnknownSchemaTests
    OTIOSerializableCollectionTests
#    OTIOItemTests
    OTIOSerializableObjectTests
    OTIOTimelineTests
#    OTIOStackAlgoTests
    OTIOTrackAlgoTests
    OTIOMultithreadingTests
)
foreach(TEST ${TESTS})
    add_executable(C${TEST} ${TEST}.cpp)
    target_include_directories(C${TEST} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/deps/optional-lite/include")
    target_link_libraries(C${TEST} PUBLIC
        gtest_main
        copentime
        copentimelineio
        pthread)
    target_compile_definitions(C${TEST} PRIVATE
        SAMPLE_DATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/sample_data/)
    add_test(NAME C${TEST} COMMAND C${TEST})
endforeach()
