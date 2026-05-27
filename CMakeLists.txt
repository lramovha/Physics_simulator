cmake_minimum_required(VERSION 3.16)
project(PhysicsSim VERSION 0.4.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # Important for JNI

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add subdirectories
add_subdirectory(c_core)
add_subdirectory(cpp_engine)

# Java JNI setup
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

message(STATUS "Java_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
message(STATUS "Java libraries ready for JNI bridge")

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

# Optional: Enable testing
enable_testing()
add_subdirectory(tests)