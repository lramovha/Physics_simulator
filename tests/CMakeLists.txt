project(PhysicsSim_Tests)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# ====================== C CORE TESTS ======================
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

set_target_properties(test_physmath PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ====================== C++ ENGINE TESTS ======================
add_executable(physengine_test
    cpp_engine/test_physics_world.cpp
)

target_link_libraries(physengine_test PRIVATE 
    physengine      # ← Correct library name
)

target_include_directories(physengine_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/cpp_engine/include
    ${CMAKE_SOURCE_DIR}/c_core/include
)

set_target_properties(physengine_test PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)