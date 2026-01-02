#!/bin/bash
# Build script for compiling C++ to WebAssembly using Emscripten

set -e

echo "Building Power Calculator WebAssembly Module..."
echo "================================================"

# Check if emcc is available
if ! command -v emcc &> /dev/null; then
    echo "Error: Emscripten (emcc) is not installed or not in PATH"
    echo ""
    echo "To install Emscripten:"
    echo "  1. git clone https://github.com/emscripten-core/emsdk.git"
    echo "  2. cd emsdk"
    echo "  3. ./emsdk install latest"
    echo "  4. ./emsdk activate latest"
    echo "  5. source ./emsdk_env.sh"
    echo ""
    echo "Then run this script again."
    exit 1
fi

# Compile C++ to WebAssembly
emcc power_calc_wasm.cpp \
    -o power_calc.js \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS='["_isValidName", "_calculatePower", "_isPositiveInteger", "_malloc", "_free"]' \
    -s EXPORTED_RUNTIME_METHODS='["allocateUTF8", "UTF8ToString"]' \
    -s MODULARIZE=0 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -O3

echo ""
echo "Build successful!"
echo "Generated files:"
echo "  - power_calc.js   (JavaScript glue code)"
echo "  - power_calc.wasm (WebAssembly binary)"
echo ""
echo "To run the application:"
echo "  python3 server.py"
echo ""
echo "Then open http://localhost:8080 in your browser"
