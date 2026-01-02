#include <emscripten/emscripten.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

// Big Integer class for arbitrary precision arithmetic
class BigInt {
private:
    std::vector<int> digits; // Stored in reverse order (least significant first)

public:
    BigInt() : digits(1, 0) {}

    BigInt(unsigned long long n) {
        if (n == 0) {
            digits.push_back(0);
        } else {
            while (n > 0) {
                digits.push_back(n % 10);
                n /= 10;
            }
        }
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.assign(digits.size() + other.digits.size(), 0);

        for (size_t i = 0; i < digits.size(); i++) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; j++) {
                long long cur = result.digits[i + j] + carry;
                if (j < other.digits.size()) {
                    cur += (long long)digits[i] * other.digits[j];
                }
                result.digits[i + j] = cur % 10;
                carry = cur / 10;
            }
        }

        // Remove leading zeros
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }

        return result;
    }

    // Fast power using binary exponentiation
    static BigInt power(unsigned long long base, unsigned long long exp) {
        BigInt result(1);
        BigInt b(base);

        while (exp > 0) {
            if (exp & 1) {
                result = result * b;
            }
            b = b * b;
            exp >>= 1;
        }

        return result;
    }

    std::string toString() const {
        std::string s;
        s.reserve(digits.size());
        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            s += ('0' + *it);
        }
        return s;
    }

    size_t numDigits() const {
        return digits.size();
    }
};

// Global buffer for returning strings to JavaScript
static char* resultBuffer = nullptr;
static size_t resultBufferSize = 0;

extern "C" {

// Validate name: only letters and spaces allowed
EMSCRIPTEN_KEEPALIVE
int isValidName(const char* name) {
    if (name == nullptr || strlen(name) == 0) {
        return 0;
    }
    for (size_t i = 0; i < strlen(name); i++) {
        char c = name[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            return 0;
        }
    }
    return 1;
}

// Validate positive integer
EMSCRIPTEN_KEEPALIVE
int isPositiveInteger(int value) {
    return value > 0 ? 1 : 0;
}

// Calculate power for small numbers (returns -1 if overflow likely)
EMSCRIPTEN_KEEPALIVE
double calculatePower(int base, int exponent) {
    if (base <= 0 || exponent < 0) {
        return -1;
    }
    // Only use this for small results
    if (exponent <= 30 && base <= 10) {
        double result = 1;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }
    return -1; // Signal to use big integer version
}

// Calculate power using big integers - returns pointer to result string
EMSCRIPTEN_KEEPALIVE
const char* calculateBigPower(int base, int exponent) {
    if (base <= 0 || exponent < 0) {
        return "Error: Invalid input";
    }

    BigInt result = BigInt::power(base, exponent);
    std::string str = result.toString();

    // Allocate/reallocate buffer if needed
    if (resultBufferSize < str.size() + 1) {
        if (resultBuffer) {
            free(resultBuffer);
        }
        resultBufferSize = str.size() + 1;
        resultBuffer = (char*)malloc(resultBufferSize);
    }

    strcpy(resultBuffer, str.c_str());
    return resultBuffer;
}

// Get the number of digits in the result
EMSCRIPTEN_KEEPALIVE
int getResultDigitCount(int base, int exponent) {
    if (base <= 0 || exponent < 0) {
        return -1;
    }
    BigInt result = BigInt::power(base, exponent);
    return result.numDigits();
}

// Free the result buffer (for cleanup)
EMSCRIPTEN_KEEPALIVE
void freeResultBuffer() {
    if (resultBuffer) {
        free(resultBuffer);
        resultBuffer = nullptr;
        resultBufferSize = 0;
    }
}

}
