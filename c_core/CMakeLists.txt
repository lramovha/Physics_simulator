project(PhysMath C)

add_library(physmath STATIC
    src/linear_algebra.c
    # Will add more files in later phases
    src/vec3.c
    src/mat3.c
    include/physmath/calculus.h
    src/calculus.c
    src/ode.c
)

target_include_directories(physmath PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Version
target_compile_definitions(physmath PUBLIC 
    PHYSMATH_VERSION="0.2.0"
)