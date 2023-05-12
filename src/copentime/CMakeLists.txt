set(COPENTIME_HEADER_FILES
        ${PROJECT_SOURCE_DIR}/include/copentime/timeRange.h
        ${PROJECT_SOURCE_DIR}/include/copentime/rationalTime.h
        ${PROJECT_SOURCE_DIR}/include/copentime/errorStatus.h
        ${PROJECT_SOURCE_DIR}/include/copentime/timeTransform.h
        ${PROJECT_SOURCE_DIR}/include/copentime/optionalOpenTime.h)

add_library(copentime ${COTIO_SHARED_OR_STATIC_LIB}
        errorStatus.cpp
        rationalTime.cpp
        timeRange.cpp
        timeTransform.cpp
        optionalOpenTime.cpp
        util.cpp
        ${PROJECT_SOURCE_DIR}/include/copentime/util.h
        ${COPENTIME_HEADER_FILES})

target_include_directories(copentime PUBLIC
        "${PROJECT_SOURCE_DIR}/include/"
        "${PROJECT_SOURCE_DIR}/OpenTimelineIO/src/")

target_link_libraries(copentime opentime)

if(WIN32)
    target_compile_definitions(copentime PRIVATE _CRT_NONSTDC_NO_WARNINGS)
endif()

set_target_properties(copentime PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        MACOSX_RPATH true WINDOWS_EXPORT_ALL_SYMBOLS true)

if (COTIO_INSTALL)
    install(TARGETS copentime
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib)
    install(FILES ${COPENTIME_HEADER_FILES}
            DESTINATION include/copentime)
endif ()