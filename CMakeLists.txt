cmake_minimum_required(VERSION 3.16)
project(PhysicsSim VERSION 0.4.0 LANGUAGES C CXX)

# Enable OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found - Parallelism enabled")
endif()

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


# ==================== JNI LIBRARY (Windows Fix) ====================
add_library(physengine_jni SHARED
    cpp_engine/jni/PhysicsJNI.cpp
)

target_include_directories(physengine_jni PRIVATE
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/cpp_engine/include
    ${CMAKE_SOURCE_DIR}/c_core/include
)

target_link_libraries(physengine_jni PRIVATE physengine)

# Strong Windows DLL settings
set_target_properties(physengine_jni PROPERTIES 
    OUTPUT_NAME "physengine"
    PREFIX ""
    SUFFIX ".dll"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)