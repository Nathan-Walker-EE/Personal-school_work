#include "main.h"

void frequency_mult(complx* H, int fft_size, complx* input_block)
{
	fft842(0, fft_size, input_block);
	for(int i = 0; i < fft_size; i++)
	{
		double re = (H[i].re * input_block[i].re) - (H[i].im * input_block[i].im);
		double im = (H[i].re * input_block[i].im) + (H[i].im * input_block[i].re);
		input_block[i].re = re;
		input_block[i].im = im;
	}
	fft842(1, fft_size, input_block);
}

void test_sinusoid_fft_add(complx* H, double f, double Fs, int length, int fft_size, int write, const char* filename)
{
	float* output = new float[length]();
	double* overlap_block1 = new double[N -1]();
	complx* input_block = new complx[fft_size]();
	int overlap_size = N - 1;
	int valid_size = fft_size - overlap_size;
	int output_tracker = 0;
	int i = 0;
	int write_index = 0;
	
	while (output_tracker < length) 
	{
		input_block[write_index++].re = cos(2 * PI * (f/Fs) * i++);
		if (write_index == fft_size) {
			for(int j = 0; j <  overlap_size; j++)
			{
				overlap_block1[j] = input_block[j + valid_size].re;
			}
			frequency_mult(H, fft_size, input_block);

			for (int j = overlap_size; j < fft_size; ++j) {
				output[output_tracker++] = input_block[j].re;
			}

			for(int j = 0; j < overlap_size; j++)
			{
				input_block[j].re = overlap_block1[j];
			}

			write_index = overlap_size;
		}
	}

	if(write)
		write_binary_file<float>(output, length, filename);
}

void test_audio_fft(complx* H, double* audio, int length, int fft_size, int write, const char* filename)
{
	float* output = new float[length]();
	double* overlap_block1 = new double[N -1]();
	complx* input_block = new complx[fft_size]();
	int overlap_size = N - 1;
	int valid_size = fft_size - overlap_size;
	int output_tracker = 0;
	int i = 0;
	int write_index = 0;
	
	auto start = chrono::high_resolution_clock::now();
	while (output_tracker < length) 
	{
		input_block[write_index++].re = audio[i++];

		if (write_index == fft_size) {
			for(int j = 0; j <  overlap_size; j++)
			{
				overlap_block1[j] = input_block[j + valid_size].re;
			}
			frequency_mult(H, fft_size, input_block);

			for (int j = overlap_size; j < fft_size; ++j) {
				output[output_tracker++] = input_block[j].re;
			}

			for(int j = 0; j < overlap_size; j++)
			{
				input_block[j].re = overlap_block1[j];
			}

			write_index = overlap_size;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "fft convolution: " << duration.count() <<" ms"<< endl;


	
		write_binary_file<float>(output, length, filename);
}
