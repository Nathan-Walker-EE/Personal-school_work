//L/M = 3/4
// L = 3 M = 4
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <chrono>
#define L 3
#define M 4
#define N 479
#define PI  3.14159265358979323846

using namespace std;

void problem4(double* h);
void problem5(double* h);
void problem6(double* h);
double FIR_filtering(double input, double* filter, double* filter_buffer, int length);
void split_filter(double* h_in, double* h_out, int in_L, int split, int step);

int main(void) {
	
	double* h = new double[N];
	ifstream infile("lowpass_filter.bin", ios::binary | ios::in);
	infile.read(reinterpret_cast<char*>(h), sizeof(double) * N);
	//problem4(h);
	//problem5(h);
	problem6(h);
	
	return(0);
}

void split_filter(double* h_in, double* h_out, int in_L, int split, int step)
{
	for(int i = 0; i < in_L/split; i++)
	{
		h_out[i] = 0;
	}
	int H_tracker = 0;
	for(int i = 0; i < in_L/split; i++)
	{
		if((i+step) < in_L)
		{
			h_out[i] = h_in[(i*split) + step];//H0 -> h indexs: 0 3 6 9 
		}										//H1 -> h indexs: 1 4 7 10
	}											//H2 -> h indexs: 2 5 8 11
}

double FIR_filtering(double input, double* filter, double* filter_buffer, int length)
{
	double sum = 0;
	for(int i = length-1; i > 0; i--)
	{
		filter_buffer[i] = filter_buffer[i-1];
		sum += filter_buffer[i] * filter[i];
	}
	filter_buffer[0] = input;
	sum += filter_buffer[0] * filter[0];
	return(sum);
}

void upsample(double input, int factor, double* output)
{
	output[0] = input;
	for(int i = 1; i < factor; i++)
	{
		output[i] = input;
	}
}

void problem5(double* h)
{
	double* H0 = new double[160];
	double* H1 = new double[160];
	double* H2 = new double[160];
	double* H00 = new double[40];
	double* H01 = new double[40];
	double* H02 = new double[40];
	double* H03 = new double[40];
	double* H10 = new double[40];
	double* H11 = new double[40];
	double* H12 = new double[40];
	double* H13 = new double[40];
	double* H20 = new double[40];
	double* H21 = new double[40];
	double* H22 = new double[40];
	double* H23 = new double[40];
	int h_tracker = 0;

	split_filter(h, H0, N, 3, 0);
	split_filter(h, H1, N, 3, 1);
	split_filter(h, H2, N, 3, 2);
	
	split_filter(H0, H00, 160, 4, 0);
	split_filter(H0, H01, 160, 4, 1);
	split_filter(H0, H02, 160, 4, 2);
	split_filter(H0, H03, 160, 4, 3);
	
	split_filter(H1, H10, 160, 4, 0);
	split_filter(H1, H11, 160, 4, 1);
	split_filter(H1, H12, 160, 4, 2);
	split_filter(H1, H13, 160, 4, 3);
	
	split_filter(H2, H20, 160, 4, 0);
	split_filter(H2, H21, 160, 4, 1);
	split_filter(H2, H22, 160, 4, 2);
	split_filter(H2, H23, 160, 4, 3);
	
	int n = 100* N;
	double** filter_buffer = new double*[12];
	double f = 1.0/16;
	double output_M[n * L / M];
	
	for(int i = 0; i < 12; i++)
	{
		filter_buffer[i] = new double[40];
	}
	
	int output_counter_L = 0;
	int output_counter_M = 0;
	int m_trck = 0;
	
	double H0_buffer[3];
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	double H1_buffer[2];
	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	double* H0_filters = new double[4];
	double* H1_filters = new double[4];
	double* H2_filters = new double[4];
	
	for(int j = 0; j < 4; j++)
	{
		H0_filters[j] = 0;
		H1_filters[j] = 0;
		H2_filters[j] = 0;
	}
	
	double H0_output = 0;
	double H1_output = 0;
	double H2_output = 0;

	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		for(int j = 2; j > 0; j--)
		{
			H0_buffer[j] = H0_buffer[j-1];
		}
		H0_buffer[0] = temp;
		H1_buffer[1] = H1_buffer[0];
		H1_buffer[0] = temp;
		
		if(i % 4 == 0 )
		{
			for(int j = 0; j < 4; j++)
			{
				H0_output += H0_filters[j];
				H1_output += H1_filters[j];
				H2_output += H2_filters[j];
			}
			output_M[output_counter_M++] = H0_output;
			output_M[output_counter_M++] = H1_output;
			output_M[output_counter_M++] = H2_output;
			H0_output = 0;
			H1_output = 0;
			H2_output = 0;
		}
		
		switch(i%4) {
			case 0:
				H0_filters[0] = L * FIR_filtering(H0_buffer[2], H03, filter_buffer[11], 40);
				H1_filters[0] = L * FIR_filtering(H1_buffer[1], H13, filter_buffer[7], 40);
				H2_filters[0] = L * FIR_filtering(temp, H23, filter_buffer[3], 40);
				break;
			case 1:
				H0_filters[1] = L * FIR_filtering(H0_buffer[2], H02, filter_buffer[10], 40);
				H1_filters[1] = L * FIR_filtering(H1_buffer[1], H12, filter_buffer[6], 40);
				H2_filters[1] = L * FIR_filtering(temp, H22, filter_buffer[2], 40);
				break;
			case 2:
				H0_filters[2] = L * FIR_filtering(H0_buffer[2], H01, filter_buffer[9], 40);
				H1_filters[2] = L * FIR_filtering(H1_buffer[1], H11, filter_buffer[5], 40);
				H2_filters[2] = L * FIR_filtering(temp, H21, filter_buffer[1], 40);
				break;
			case 3:
				H0_filters[3] = L * FIR_filtering(H0_buffer[2], H00, filter_buffer[8], 40);
				H1_filters[3] = L * FIR_filtering(H1_buffer[1], H10, filter_buffer[4], 40);
				H2_filters[3] = L * FIR_filtering(temp, H20, filter_buffer[0], 40);
				break;
		}
	}
	ofstream outfile3("outputM_cos_16_p5.bin", ios::binary | ios::out);
    outfile3.write(reinterpret_cast<const char*>(output_M), (n * L / M) * sizeof(double));
    outfile3.close();
	
	f = 1.0/8;
	output_counter_L = 0;
	output_counter_M = 0;
	m_trck = 0;
	H0_output = 0;
	H1_output = 0;
	H2_output = 0;
	
	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		for(int j = 2; j > 0; j--)
		{
			H0_buffer[j] = H0_buffer[j-1];
		}
		H0_buffer[0] = temp;
		H1_buffer[1] = H1_buffer[0];
		H1_buffer[0] = temp;
		
		if(i % 4 == 0 )
		{
			for(int j = 0; j < 4; j++)
			{
				H0_output += H0_filters[j];
				H1_output += H1_filters[j];
				H2_output += H2_filters[j];
			}
			output_M[output_counter_M++] = H0_output;
			output_M[output_counter_M++] = H1_output;
			output_M[output_counter_M++] = H2_output;
			H0_output = 0;
			H1_output = 0;
			H2_output = 0;
		}
		
		switch(i%4) {
			case 0:
				H0_filters[0] = L * FIR_filtering(H0_buffer[2], H03, filter_buffer[11], 40);
				H1_filters[0] = L * FIR_filtering(H1_buffer[1], H13, filter_buffer[7], 40);
				H2_filters[0] = L * FIR_filtering(temp, H23, filter_buffer[3], 40);
				break;
			case 1:
				H0_filters[1] = L * FIR_filtering(H0_buffer[2], H02, filter_buffer[10], 40);
				H1_filters[1] = L * FIR_filtering(H1_buffer[1], H12, filter_buffer[6], 40);
				H2_filters[1] = L * FIR_filtering(temp, H22, filter_buffer[2], 40);
				break;
			case 2:
				H0_filters[2] = L * FIR_filtering(H0_buffer[2], H01, filter_buffer[9], 40);
				H1_filters[2] = L * FIR_filtering(H1_buffer[1], H11, filter_buffer[5], 40);
				H2_filters[2] = L * FIR_filtering(temp, H21, filter_buffer[1], 40);
				break;
			case 3:
				H0_filters[3] = L * FIR_filtering(H0_buffer[2], H00, filter_buffer[8], 40);
				H1_filters[3] = L * FIR_filtering(H1_buffer[1], H10, filter_buffer[4], 40);
				H2_filters[3] = L * FIR_filtering(temp, H20, filter_buffer[0], 40);
				break;
		}
	}	
	ofstream outfile2("outputM_cos_8_p5.bin", ios::binary | ios::out);
    outfile2.write(reinterpret_cast<const char*>(output_M), (n * L / M) * sizeof(double));
    outfile2.close();
	
	f = 1.0/4;
	output_counter_L = 0;
	output_counter_M = 0;
	m_trck = 0;
	H0_output = 0;
	H1_output = 0;
	H2_output = 0;
	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		for(int j = 2; j > 0; j--)
		{
			H0_buffer[j] = H0_buffer[j-1];
		}
		H0_buffer[0] = temp;
		H1_buffer[1] = H1_buffer[0];
		H1_buffer[0] = temp;
		
		if(i % 4 == 0 )
		{
			for(int j = 0; j < 4; j++)
			{
				H0_output += H0_filters[j];
				H1_output += H1_filters[j];
				H2_output += H2_filters[j];
			}
			output_M[output_counter_M++] = H0_output;
			output_M[output_counter_M++] = H1_output;
			output_M[output_counter_M++] = H2_output;
			H0_output = 0;
			H1_output = 0;
			H2_output = 0;
		}
		
		switch(i%4) {
			case 0:
				H0_filters[0] = L * FIR_filtering(H0_buffer[2], H03, filter_buffer[11], 40);
				H1_filters[0] = L * FIR_filtering(H1_buffer[1], H13, filter_buffer[7], 40);
				H2_filters[0] = L * FIR_filtering(temp, H23, filter_buffer[3], 40);
				break;
			case 1:
				H0_filters[1] = L * FIR_filtering(H0_buffer[2], H02, filter_buffer[10], 40);
				H1_filters[1] = L * FIR_filtering(H1_buffer[1], H12, filter_buffer[6], 40);
				H2_filters[1] = L * FIR_filtering(temp, H22, filter_buffer[2], 40);
				break;
			case 2:
				H0_filters[2] = L * FIR_filtering(H0_buffer[2], H01, filter_buffer[9], 40);
				H1_filters[2] = L * FIR_filtering(H1_buffer[1], H11, filter_buffer[5], 40);
				H2_filters[2] = L * FIR_filtering(temp, H21, filter_buffer[1], 40);
				break;
			case 3:
				H0_filters[3] = L * FIR_filtering(H0_buffer[2], H00, filter_buffer[8], 40);
				H1_filters[3] = L * FIR_filtering(H1_buffer[1], H10, filter_buffer[4], 40);
				H2_filters[3] = L * FIR_filtering(temp, H20, filter_buffer[0], 40);
				break;
		}
	}
	ofstream outfile1("outputM_cos_4_p5.bin", ios::binary | ios::out);
    outfile1.write(reinterpret_cast<const char*>(output_M), (n * L / M) * sizeof(double));
    outfile1.close();
	
}

void problem4(double* h){
	int n = 100 * N;
	int conv_L = n;
	double f = 1.0/16;
	double output_L[L];
	double output_M[n * L / M];
	double* filter_buffer = new double[N];
	
	for(int i = 0; i < L; i++)
	{
		output_L[i] = 0;
	}
	
	for(int i = 0; i < (conv_L + 1)/ M; i++)
	{
		output_M[i] = 0;
	}
	int output_counter_L = 0;
	int output_counter_M = 0;
	int m_trck = 0;
	
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		upsample(temp, L, output_L);
		for(int j = 0; j < L; j++)
		{
			double filter_output = FIR_filtering(output_L[j], h, filter_buffer, N);
			if( m_trck % M == 0)
			{
				output_M[output_counter_M++] = filter_output;
			}
			m_trck++;
		}
	}
	
	ofstream outfile1("outputM_cos_16.bin", ios::binary | ios::out);
    outfile1.write(reinterpret_cast<const char*>(output_M), (conv_L / M) * sizeof(double));
    outfile1.close();
	
	f = 1.0/8.0;
	for(int i = 0; i < (conv_L + 1)/ M; i++)
	{
		output_M[i] = 0;
	}
	for(int i = 0; i < N; i++)
	{
		filter_buffer[i] = 0;
	}
	output_counter_L = 0;
	output_counter_M = 0;
	m_trck = 0;
	
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		upsample(temp, L, output_L);
		for(int j = 0; j < L; j++)
		{
			double filter_output = FIR_filtering(output_L[j], h, filter_buffer, N);
			if( m_trck % M == 0)
			{
				output_M[output_counter_M++] = filter_output;
			}
			m_trck++;
		}
	}
	
	ofstream outfile2("outputM_cos_8.bin", ios::binary | ios::out);
    outfile2.write(reinterpret_cast<const char*>(output_M), (conv_L / M) * sizeof(double));
    outfile2.close();
	
	f = 1.0/4.0;
	for(int i = 0; i < (conv_L + 1)/ M; i++)
	{
		output_M[i] = 0;
	}
	for(int i = 0; i < N; i++)
	{
		filter_buffer[i] = 0;
	}
	output_counter_L = 0;
	output_counter_M = 0;
	m_trck = 0;
	
	for(int i = 0; i < n; i++)
	{
		double temp = cos(2 * PI * f * i);
		upsample(temp, L, output_L);
		for(int j = 0; j < L; j++)
		{
			double filter_output = FIR_filtering(output_L[j], h, filter_buffer, N);
			if( m_trck % M == 0)
			{
				output_M[output_counter_M++] = filter_output;
			}
			m_trck++;
		}
	}
	
	ofstream outfile3("outputM_cos_4.bin", ios::binary | ios::out);
    outfile3.write(reinterpret_cast<const char*>(output_M), (conv_L / M) * sizeof(double));
    outfile3.close();
	
}

void problem6(double* h)
{
	ifstream file("audio.bin", ios::binary);
	file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);
	size_t numdoubles = fileSize/ sizeof(double);
	
	double* audio = new double[numdoubles];
	file.read(reinterpret_cast<char*>(audio), fileSize);
	
	double* output_L = new double[L];
	double* filter_buffer = new double[N];
	int output_counter_M = 0;
	int m_trck = 0;
	double* output_M = new double[numdoubles * L / M];
	
	auto start = chrono::high_resolution_clock::now();
	for(int i = 0; i < numdoubles; i++)
	{
		double temp = audio[i];
		upsample(temp, L, output_L);
		for(int j = 0; j < L; j++)
		{
			double filter_output = FIR_filtering(output_L[j], h, filter_buffer, N);
			if( m_trck % M == 0)
			{
				output_M[output_counter_M++] = filter_output;
			}
			m_trck++;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "Traditional decimation: " << duration.count() <<" ms"<< endl;
	
	ofstream outfile1("problem6_trad.bin", ios::binary | ios::out);
    outfile1.write(reinterpret_cast<const char*>(output_M), (numdoubles * L / M) * sizeof(double));
    outfile1.close();
	
	double* H0 = new double[160];
	double* H1 = new double[160];
	double* H2 = new double[160];
	double* H00 = new double[40];
	double* H01 = new double[40];
	double* H02 = new double[40];
	double* H03 = new double[40];
	double* H10 = new double[40];
	double* H11 = new double[40];
	double* H12 = new double[40];
	double* H13 = new double[40];
	double* H20 = new double[40];
	double* H21 = new double[40];
	double* H22 = new double[40];
	double* H23 = new double[40];
	int h_tracker = 0;

	split_filter(h, H0, N, 3, 0);
	split_filter(h, H1, N, 3, 1);
	split_filter(h, H2, N, 3, 2);
	
	split_filter(H0, H00, 160, 4, 0);
	split_filter(H0, H01, 160, 4, 1);
	split_filter(H0, H02, 160, 4, 2);
	split_filter(H0, H03, 160, 4, 3);
	
	split_filter(H1, H10, 160, 4, 0);
	split_filter(H1, H11, 160, 4, 1);
	split_filter(H1, H12, 160, 4, 2);
	split_filter(H1, H13, 160, 4, 3);
	
	split_filter(H2, H20, 160, 4, 0);
	split_filter(H2, H21, 160, 4, 1);
	split_filter(H2, H22, 160, 4, 2);
	split_filter(H2, H23, 160, 4, 3);
	
	int n = 100* N;
	double** filter_buffer_p = new double*[12];

	
	for(int i = 0; i < 12; i++)
	{
		filter_buffer_p[i] = new double[40];
	}
	
	int output_counter_L = 0;
	output_counter_M = 0;
	m_trck = 0;
	
	double H0_buffer[3];
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	double H1_buffer[2];
	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	double* H0_filters = new double[4];
	double* H1_filters = new double[4];
	double* H2_filters = new double[4];
	
	for(int j = 0; j < 4; j++)
	{
		H0_filters[j] = 0;
		H1_filters[j] = 0;
		H2_filters[j] = 0;
	}
	
	double H0_output = 0;
	double H1_output = 0;
	double H2_output = 0;

	for(int i = 0; i < 2; i++)
	{
		H1_buffer[i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		H0_buffer[i] = 0;
	}
	
	start = chrono::high_resolution_clock::now();
	for(int i = 0; i < numdoubles; i++)
	{
		double temp = audio[i];
		for(int j = 2; j > 0; j--)
		{
			H0_buffer[j] = H0_buffer[j-1];
		}
		H0_buffer[0] = temp;
		H1_buffer[1] = H1_buffer[0];
		H1_buffer[0] = temp;
		
		if(i % 4 == 0 )
		{
			for(int j = 0; j < 4; j++)
			{
				H0_output += H0_filters[j];
				H1_output += H1_filters[j];
				H2_output += H2_filters[j];
			}
			output_M[output_counter_M++] = H0_output;
			output_M[output_counter_M++] = H1_output;
			output_M[output_counter_M++] = H2_output;
			H0_output = 0;
			H1_output = 0;
			H2_output = 0;
		}
		
		switch(i%4) {
			case 0:
				H0_filters[0] = L * FIR_filtering(H0_buffer[2], H03, filter_buffer_p[11], 40);
				H1_filters[0] = L * FIR_filtering(H1_buffer[1], H13, filter_buffer_p[7], 40);
				H2_filters[0] = L * FIR_filtering(temp, H23, filter_buffer_p[3], 40);
				break;
			case 1:
				H0_filters[1] = L * FIR_filtering(H0_buffer[2], H02, filter_buffer_p[10], 40);
				H1_filters[1] = L * FIR_filtering(H1_buffer[1], H12, filter_buffer_p[6], 40);
				H2_filters[1] = L * FIR_filtering(temp, H22, filter_buffer_p[2], 40);
				break;
			case 2:
				H0_filters[2] = L * FIR_filtering(H0_buffer[2], H01, filter_buffer_p[9], 40);
				H1_filters[2] = L * FIR_filtering(H1_buffer[1], H11, filter_buffer_p[5], 40);
				H2_filters[2] = L * FIR_filtering(temp, H21, filter_buffer_p[1], 40);
				break;
			case 3:
				H0_filters[3] = L * FIR_filtering(H0_buffer[2], H00, filter_buffer_p[8], 40);
				H1_filters[3] = L * FIR_filtering(H1_buffer[1], H10, filter_buffer_p[4], 40);
				H2_filters[3] = L * FIR_filtering(temp, H20, filter_buffer_p[0], 40);
				break;
		}
	}
	end = chrono::high_resolution_clock::now();
	
	duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "Polyphase decimation: " << duration.count() <<" ms"<< endl;
	
	ofstream outfile3("problem6_poly.bin", ios::binary | ios::out);
    outfile3.write(reinterpret_cast<const char*>(output_M), (numdoubles * L / M) * sizeof(double));
    outfile3.close();
}