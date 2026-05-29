project(PhysEngine CXX)

add_library(physengine STATIC
    src/PhysicsWorld.cpp
    src/RigidBody.cpp
    # include/physengine/integrators/RK4Integrator.hpp
    src/integrators/RK4Integrator.cpp
    # include/physengine/scenes/BaseScene.hpp
    src/scenes/BaseScene.cpp
    # include/physengine/scenes/FallingBallsScene.hpp
    src/scenes/FallingBallsScene.cpp
    src/scenes/ProjectileScene.cpp
    src/scenes/PendulumScene.cpp
    src/scenes/BouncingBallsScene.cpp
    # More files will be added later
)

target_include_directories(physengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/c_core/include
)

target_link_libraries(physengine PUBLIC physmath)

target_compile_definitions(physengine PUBLIC 
    PHYSENGINE_VERSION="0.6.0"
)

# ====================== DEMO EXECUTABLE ======================
add_executable(main_demo
    src/main_demo.cpp
)

target_link_libraries(main_demo PRIVATE physengine)

set_target_properties(main_demo PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)