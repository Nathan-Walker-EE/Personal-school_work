#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#define PI  3.14159265358979323846
#define N 257

/* typedef's */
typedef struct {
	float re;
	float im;
}complx;

using namespace std;


/* functions to perform problems */
void problem2(double* h);
void problem3(double* h);
void problem4(double* h);

/* time domain functions */
double FIR_filtering(double* h, double input, double* filter_buffer, int length);
void test_sinusoid(double* h, double f, double Fs, int length, int write, const char* filename);
void test_audio(double* h, double* audio, int length, int write, const char* filename);

/* frequency domain functions*/
void overlap_add(complx* H, int fft_size, complx* input_block);
void test_sinusoid_fft_add(complx* H, double f, double Fs, int length, int fft_size, int write, const char* filename);
void test_audio_fft(complx* H, double* audio, int length, int fft_size, int write, const char* filename);

#ifdef __cplusplus
extern "C" {
#endif

void fft842(int in, int n, complx* x);

#ifdef __cplusplus
}
#endif

/* useful functions*/
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