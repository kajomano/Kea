#include <iostream>

#include "kea.h"

int main() {
    Kea kea;

    try {
        kea.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}