project(PhysEngine CXX)

add_library(physengine STATIC
    src/PhysicsWorld.cpp
    src/RigidBody.cpp
    include/physengine/integrators/RK4Integrator.hpp
    src/integrators/RK4Integrator.cpp
    # More files will be added later
)

target_include_directories(physengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/c_core/include
)

target_link_libraries(physengine PUBLIC physmath)

target_compile_definitions(physengine PUBLIC 
    PHYSENGINE_VERSION="0.4.0"
)