cmake_minimum_required(VERSION 3.30)
project(fface LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Libs
add_subdirectory(lib)

# Main
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

add_executable(fface
    src/main.cpp
    src/core/arguments.cpp
    src/core/processor.cpp
    src/core/library_loader.cpp
)

target_include_directories(fface
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(fface
    PRIVATE
        Boost::program_options
        ${OpenCV_LIBS}
        detector
)

# Unit tests
add_executable(fface_tests
    tests/test_processor.cpp
    tests/detector/test_detector.cpp
    src/core/arguments.cpp
    src/core/processor.cpp
    src/core/library_loader.cpp
)

target_link_libraries(fface_tests
    PRIVATE
        Boost::program_options
        GTest::gtest
        GTest::gtest_main
        ${OpenCV_LIBS}
        detector
)

target_include_directories(fface_tests
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)

gtest_discover_tests(fface_tests)
enable_testing()
add_test(NAME fface_tests COMMAND fface_tests)
