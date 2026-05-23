#include <catch2/catch_test_macros.hpp>
#include "physmath/physmath.h"

TEST_CASE("Physmath hello works") {
    REQUIRE_NOTHROW(physmath_hello());
}