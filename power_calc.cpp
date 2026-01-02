#include <iostream>
#include <string>
#include <cmath>
#include <cctype>

bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

int main() {
    std::string name;
    int base, exponent;

    // Get name (only letters allowed)
    do {
        std::cout << "Enter your name (letters only, no numbers): ";
        std::getline(std::cin, name);
        if (!isValidName(name)) {
            std::cout << "Invalid name. Please use only letters.\n";
        }
    } while (!isValidName(name));

    // Get first positive integer (base)
    do {
        std::cout << "Enter the first positive integer (base): ";
        std::cin >> base;
        if (std::cin.fail() || base <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a positive integer.\n";
            base = 0;
        }
    } while (base <= 0);

    // Get second positive integer (exponent)
    do {
        std::cout << "Enter the second positive integer (exponent): ";
        std::cin >> exponent;
        if (std::cin.fail() || exponent <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a positive integer.\n";
            exponent = 0;
        }
    } while (exponent <= 0);

    // Calculate and display result
    long long result = static_cast<long long>(std::pow(base, exponent));

    std::cout << "\nHello, " << name << "!\n";
    std::cout << base << " ^ " << exponent << " = " << result << "\n";

    return 0;
}
