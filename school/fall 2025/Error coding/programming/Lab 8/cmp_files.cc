#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file1> <file2>\n";
        return 1;
    }

    std::string file1 = argv[1];
    std::string file2 = argv[2];

    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1) {
        std::cerr << "Error opening " << file1 << "\n";
        return 1;
    }
    if (!f2) {
        std::cerr << "Error opening " << file2 << "\n";
        return 1;
    }

    const size_t BUFSZ = 4096;
    std::vector<unsigned char> buf1(BUFSZ);
    std::vector<unsigned char> buf2(BUFSZ);

    uint64_t index = 0;

    while (true) {
        f1.read(reinterpret_cast<char*>(buf1.data()), BUFSZ);
        f2.read(reinterpret_cast<char*>(buf2.data()), BUFSZ);

        std::streamsize r1 = f1.gcount();
        std::streamsize r2 = f2.gcount();

        if (r1 == 0 && r2 == 0) {
            break;  // both files ended
        }

        std::streamsize rmin = std::min(r1, r2);

        for (std::streamsize i = 0; i < rmin; i++) {
            if (buf1[i] != buf2[i]) {
                std::cout << "Files differ at byte " 
                          << (index + i) << "\n";
                std::cout << file1 << ": " << (int)buf1[i] << "\n";
                std::cout << file2 << ": " << (int)buf2[i] << "\n";
                return 0;
            }
        }

        if (r1 != r2) {
            std::cout << "Files match for " << (index + rmin)
                      << " bytes, but sizes differ.\n";
            return 0;
        }

        index += rmin;
    }

    std::cout << "Files are IDENTICAL.\n";
    return 0;
}
