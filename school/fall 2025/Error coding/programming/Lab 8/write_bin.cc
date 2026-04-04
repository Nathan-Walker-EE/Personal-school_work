#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    int N;
    std::cout << "Enter number of bytes to generate: ";
    std::cin >> N;

    if (N <= 0) {
        std::cerr << "N must be positive.\n";
        return 1;
    }

    std::vector<unsigned char> data(N);

    // Random generator (uniform 0–255)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    // Fill with random bytes
    for (int i = 0; i < N; i++) {
        data[i] = static_cast<unsigned char>(dist(gen));
    }

    // Write binary file
    std::ofstream outfile("random_bytes.bin", std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file for writing.\n";
        return 1;
    }

    outfile.write(reinterpret_cast<const char*>(data.data()), data.size());
    outfile.close();

    std::cout << "Wrote " << N << " random bytes to random_bytes.bin\n";
    return 0;
}
