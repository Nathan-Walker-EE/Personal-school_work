#include "main.h"
#include "time_domain.h"

double FIR_filtering(double* h, double input, double* filter_buffer, int length)
{
	double sum = 0;
	
	for(int i = length - 1; i > 0; i--)
	{
		filter_buffer[i] = filter_buffer[i - 1];
		sum += filter_buffer[i] * h[i];
	}
	filter_buffer[0] = input;
	return(sum += filter_buffer[0] * h[0]);
}

void test_sinusoid(double* h, double f, double Fs, int length, int write, const char* filename)
{
	double* filter_buffer = new double[N];
	double* output = new double[length];
	
	for(int i = 0; i < N; i++)
	{
		filter_buffer[i] = 0;
	}
	
	for(int i = 0; i < length; i++)
	{
		double temp = cos(2 * PI * (f/Fs) * i);
		output[i] = FIR_filtering(h, temp, filter_buffer, N);
	}
	
	if(write == 1)
	{
		ofstream outfile1(filename, ios::binary | ios::out);
		outfile1.write(reinterpret_cast<const char*>(output), (length) * sizeof(double));
		outfile1.close();
	}else 
	{
		for(int i = 0; i < length; i++)
		{
			cout << output[i] << " ";
		}
	}

}

void test_audio(double* h, double* audio, int length, int write, const char* filename)
{
	double* filter_buffer = new double[N]();
	float* output = new float[length]();
	
	auto start = chrono::high_resolution_clock::now();
	for(int i = 0; i < length; i++)
	{
		output[i] = FIR_filtering(h, audio[i], filter_buffer, N);
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "time convolution: " << duration.count() <<" ms"<< endl;
	
	if(write == 1)
	{
		write_binary_file<float>(output, length, filename);
	}else 
	{
		for(int i = 0; i < length; i++)
		{
			cout << output[i] << " ";
		}
	}

}