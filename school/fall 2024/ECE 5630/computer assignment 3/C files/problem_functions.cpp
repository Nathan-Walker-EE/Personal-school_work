#include "main.h"

void problem2(double* h)
{
	string filename = "data/cos_10Hz.bin";
	test_sinusoid(h, 10, 11025, 9*N, 1, filename.c_str());//change
	
	filename = "data/cos_40Hz.bin";
	test_sinusoid(h, 40, 11025, 9*N, 1, filename.c_str());
	
	filename = "data/cos_150Hz.bin";
	test_sinusoid(h, 150, 11025, 9*N, 1, filename.c_str());
	
	filename = "data/cos_350Hz.bin";
	test_sinusoid(h, 350, 11025, 9*N, 1, filename.c_str());
	
	filename = "data/cos_500Hz.bin";
	test_sinusoid(h, 500, 11025, 9*N, 1, filename.c_str());
	
}

void problem3(double* h)
{
	int n = 512; //length of transform
	complx* H = new complx[n];
	float* real = new float[n];
	float* im = new float[n];
	
	for(int i = 0; i < N; i++)
	{
		H[i].re = h[i];
		H[i].im = 0;
	}
	for(int i = N; i < n; i++)
	{
		H[i].re = 0;
		H[i].im = 0;
	}
	
	fft842(0, n, H);
	for(int i = 0; i < n; i++)
	{
		real[i] = H[i].re;
		im[i] = H[i].im;
	}
	
	cout << endl << "filter magnitude respones" << endl;
	for(int i = 0; i < n; i++)
	{
		//cout << i << ": " << sqrt(H[i].re * H[i].re + H[i].im * H[i].im) << endl;
	}
	
	string filename = "data/H_real.bin";
	write_binary_file<float>(real, n, filename.c_str());
	
	filename = "data/H_imag.bin";
	write_binary_file<float>(im, n, filename.c_str());
	
	for(int i = 0; i < n; i++)
	{
		real[i] = H[i].re;
		im[i] = H[i].im;
	}
	
	filename = "data/h_reverse.bin";
	write_binary_file<float>(real, n, filename.c_str());
	
	filename = "data/cos_10Hz_fft.bin";
	test_sinusoid_fft_add(H, 10, 11025, 2*n + 1, n, 1, filename.c_str());
	
	filename = "data/cos_40Hz_fft.bin";
	test_sinusoid_fft_add(H, 40, 11025, 2*n + 1, n, 1, filename.c_str());
	
	filename = "data/cos_150Hz_fft.bin";
	test_sinusoid_fft_add(H, 150, 11025, 2*n + 1, n, 1, filename.c_str());
	
	filename = "data/cos_350Hz_fft.bin";
	test_sinusoid_fft_add(H, 350, 11025, 2*n + 1, n, 1, filename.c_str());
	
	filename = "data/cos_500Hz_fft.bin";
	test_sinusoid_fft_add(H, 500, 11025, 2*n + 1, n, 1, filename.c_str());
}

void problem4(double* h)
{
	int n = 512; //length of transform
	complx* H = new complx[n];
	
	for(int i = 0; i < N; i++)
	{
		H[i].re = h[i];
		H[i].im = 0;
	}
	for(int i = N; i < n; i++)
	{
		H[i].re = 0;
		H[i].im = 0;
	}
	fft842(0, n, H);

	double* audio = new double[496125]();
	ifstream infile("data/audio.bin", ios::binary | ios::in);
	infile.read(reinterpret_cast<char*>(audio), sizeof(double) * 496125);
	test_audio_fft(H, audio, 496125, n, 1, "data/audio_out_fft.bin");
	
	test_audio(h, audio, 496125, 1, "data/audio_out.bin");
}