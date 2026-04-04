#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define VECTOR_WIDTH 36  // Number of bits per test vector
#define NUM_VECTORS 10  // Number of test vectors

int main() {
    std::ofstream file("test_vectors.txt");
    if (!file) {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    std::srand(std::time(0));  // Seed for random number generation

    for (int i = 0; i < NUM_VECTORS; ++i) {
        for (int j = 0; j < VECTOR_WIDTH; ++j) {
            file << (std::rand() % 2);  // Write 1 or 0
        }
        file << "\n";  // New line for each vector
    }

    file.close();
    std::cout << "Test vectors written to test_vectors.txt\n";

    return 0;
}
