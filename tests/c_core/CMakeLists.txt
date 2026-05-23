add_executable(c_core_tests
    test_physmath.cpp
)

target_link_libraries(c_core_tests
    PRIVATE c_core Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(c_core_tests)