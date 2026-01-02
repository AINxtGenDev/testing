# Power Calculator WebAssembly

A high-performance power calculator that runs in the browser using WebAssembly. Supports **arbitrary precision big integers** for calculations like `200^300` or `999^999`.

## Features

- **Arbitrary Precision**: Calculate powers with results containing thousands of digits
- **WebAssembly Performance**: Core computation runs in compiled C++ via WASM
- **Binary Exponentiation**: O(log n) algorithm for fast large power calculations
- **Browser-Based**: No server-side computation required
- **Input Validation**: Name and number validation performed in WASM
- **Copy to Clipboard**: Easily copy large results

## Quick Start

### Prerequisites

- Python 3.x (for the development server)
- Emscripten SDK (only needed if rebuilding WASM)

### Running the Application

```bash
# Start the HTTP server
python3 server.py

# Open in browser
# http://localhost:8080
```

### Building from Source

If you need to rebuild the WebAssembly module:

```bash
# Install Emscripten (one-time setup)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..

# Build the WASM module
./build_wasm.sh
```

## Project Structure

```
├── power_calc_wasm.cpp   # C++ source with BigInt implementation
├── power_calc.js         # Emscripten-generated JavaScript glue
├── power_calc.wasm       # Compiled WebAssembly binary
├── index.html            # Browser interface
├── server.py             # Python HTTP server with WASM MIME types
├── build_wasm.sh         # Build script for recompiling WASM
├── power_calc.cpp        # Original CLI version (C++)
└── SKILL.md              # Development skill reference
```

## Technical Details

### BigInt Implementation

The C++ code includes a custom `BigInt` class that:
- Stores digits in a vector (least significant first)
- Implements grade-school multiplication algorithm
- Uses binary exponentiation for O(log n) power calculation

### Exported WASM Functions

| Function | Description |
|----------|-------------|
| `isValidName(char*)` | Validates name (letters and spaces only) |
| `isPositiveInteger(int)` | Checks if value is positive |
| `calculateBigPower(int, int)` | Computes base^exponent as string |
| `getResultDigitCount(int, int)` | Returns number of digits in result |

### Example Calculations

| Expression | Digits | Notes |
|------------|--------|-------|
| 2^10 | 4 | 1024 |
| 10^100 | 101 | Googol |
| 200^300 | 690 | Large calculation |
| 999^999 | 2,997 | Very large calculation |

## Development

### Requirements for Building

- Emscripten SDK 3.0+
- C++11 or later

### Compilation Flags

```bash
emcc power_calc_wasm.cpp -o power_calc.js \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS='["_isValidName", "_calculateBigPower", ...]' \
    -s EXPORTED_RUNTIME_METHODS='["allocateUTF8", "UTF8ToString"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -O3
```

## License

MIT License
