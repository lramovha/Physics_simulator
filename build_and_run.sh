#!/bin/bash
echo "===================================================="
echo "   Physics Simulator - Full Build & Run (MSYS2 UCRT64)"
echo "===================================================="
echo

# Go to project root
cd "$(dirname "$0")" || exit

echo "[1/6] Cleaning previous build..."
rm -rf build 2>/dev/null

echo "[2/6] Configuring CMake..."
cmake -B build -S . -G "MinGW Makefiles" \
  -DCMAKE_MAKE_PROGRAM=/ucrt64/bin/mingw32-make.exe \
  -DCMAKE_C_COMPILER=/ucrt64/bin/gcc.exe \
  -DCMAKE_CXX_COMPILER=/ucrt64/bin/g++.exe \
  -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    read -p "Press Enter to exit..."
    exit 1
fi

echo "[3/6] Building native library (C/C++ + JNI)..."
cmake --build build --config Release -j4

if [ $? -ne 0 ]; then
    echo "Build failed!"
    read -p "Press Enter to exit..."
    exit 1
fi

echo "[4/6] Running native tests..."
echo "→ test_physmath"
./build/bin/test_physmath

echo "→ physengine_test"
./build/bin/physengine_test

echo "→ main_demo"
./build/bin/main_demo

echo "[5/6] Building JavaFX GUI..."
cd java_app || exit

rm -rf build/classes 2>/dev/null

"/c/Program Files/Java/jdk-26.0.1/bin/javac.exe" \
  --module-path javafx-sdk-26.0.1/lib \
  --add-modules javafx.controls,javafx.graphics \
  -d build/classes \
  src/main/java/com/physics/sim/*.java \
  src/main/java/com/physics/sim/nativebridge/*.java

echo "[6/6] Launching JavaFX Application..."
"/c/Program Files/Java/jdk-26.0.1/bin/java.exe" \
  --module-path javafx-sdk-26.0.1/lib \
  --add-modules javafx.controls,javafx.graphics \
  -Djava.library.path=../build/lib \
  -cp build/classes com.physics.sim.MainApp

echo
echo "===================================================="
echo "✅ Build and Run completed!"
echo "===================================================="
read -p "Press Enter to close..."