add_executable(cpp_engine_tests
    test_physics_world.cpp
)

target_link_libraries(cpp_engine_tests
    PRIVATE
    cpp_engine
    c_core
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(cpp_engine_tests)