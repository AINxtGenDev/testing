# Power Calculator WebAssembly

A high-performance power calculator that runs in the browser using WebAssembly. Supports **arbitrary precision big integers** for calculations like `200^300` or `999^999`.

## Features

- **Arbitrary Precision**: Calculate powers with results containing thousands of digits
- **WebAssembly Performance**: Core computation runs in compiled C++ via WASM
- **Binary Exponentiation**: O(log n) algorithm for fast large power calculations
- **Browser-Based**: No server-side computation required
- **Input Validation**: Name and number validation performed in WASM
- **Copy to Clipboard**: Easily copy large results

## What is WebAssembly?

WebAssembly (WASM) is a binary instruction format that allows you to run code written in languages like C, C++, or Rust directly in the browser at near-native speed.

### How It Works: From C++ to Browser

```
┌─────────────────┐      ┌─────────────────┐      ┌─────────────────┐
│   C++ Source    │ ──── │   Emscripten    │ ──── │   .wasm + .js   │
│  power_calc.cpp │      │    Compiler     │      │  Browser-ready  │
└─────────────────┘      └─────────────────┘      └─────────────────┘
```

1. **Write C++ Code**: Create your application logic in C++ (e.g., `power_calc.cpp`)
2. **Compile with Emscripten**: Use `emcc` to compile C++ to WebAssembly binary (`.wasm`) and JavaScript glue code (`.js`)
3. **Run in Browser**: The browser loads the WASM module and JavaScript can call your C++ functions

### Why Use WebAssembly?

| Benefit | Description |
|---------|-------------|
| **Performance** | Runs at near-native speed (much faster than JavaScript for compute-heavy tasks) |
| **Code Reuse** | Use existing C/C++ libraries in the browser without rewriting |
| **Portability** | Same code runs on any modern browser (Chrome, Firefox, Safari, Edge) |
| **Security** | Runs in a sandboxed environment, same as JavaScript |

### This Project's Workflow

```
power_calc.cpp (C++ CLI app)
        │
        ▼
power_calc_wasm.cpp (Modified for WASM exports)
        │
        ▼ [emcc compiler]
        │
   ┌────┴────┐
   ▼         ▼
.wasm     .js (glue)
   │         │
   └────┬────┘
        ▼
   index.html (calls C++ functions via JavaScript)
        │
        ▼
   Browser (runs C++ at near-native speed)
```

## Documentation

- [Auto-Claude UI Quickstart Guide](quickstart.html) - Step-by-step setup and usage instructions

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
