find_package(OpenCV REQUIRED)
find_package(JsonCpp REQUIRED)

add_library(detector SHARED detector/detector.cpp)

target_include_directories(detector PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(detector
    PUBLIC
        ${OpenCV_LIBS}
        JsonCpp::JsonCpp
)

set_target_properties(detector PROPERTIES
    PREFIX ""                # remove "lib" prefix
    SUFFIX ".so"             # force .so extension
    OUTPUT_NAME "detector"   # final file: detector.so
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)