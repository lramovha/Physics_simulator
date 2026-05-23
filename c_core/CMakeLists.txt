project(PhysMath C)

add_library(physmath STATIC
    src/linear_algebra.c
    # Will add more files in later phases
)

target_include_directories(physmath PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Version
target_compile_definitions(physmath PUBLIC 
    PHYSMATH_VERSION="0.1.0"
)