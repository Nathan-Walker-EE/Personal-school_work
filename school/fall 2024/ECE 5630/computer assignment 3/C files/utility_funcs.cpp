#include "main.h"

template <typename T>
void write_binary_file(T* data, int length, const char* filename)
{
	ofstream outfile1(filename, ios::binary | ios::out);
    if (outfile1.is_open()) {
        outfile1.write(reinterpret_cast<const char*>(data), length * sizeof(T)); // write the real part
        outfile1.close();
    } else {
        cerr << "Failed to open file for writing!" << endl;
    }
}
