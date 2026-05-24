# add_subdirectory(c_core)
# add_subdirectory(cpp_engine)

project(PhysicsSim_Tests)

# Download Catch2 (if not installed)
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# C Core Tests
add_executable(test_physmath
    c_core/test_physmath.cpp
)

target_link_libraries(test_physmath PRIVATE 
    physmath
    Catch2::Catch2WithMain
)

target_include_directories(test_physmath PRIVATE 
    ${CMAKE_SOURCE_DIR}/c_core/include
)

add_test(NAME TestPhysMath COMMAND test_physmath)

# Output test executable to bin/
set_target_properties(test_physmath PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)