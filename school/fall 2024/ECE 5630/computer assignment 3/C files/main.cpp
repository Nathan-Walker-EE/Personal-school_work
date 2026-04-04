#include "main.h"

int main(void)
{
	double* h = new double[N];
	ifstream infile("data/lowpass_filter.bin", ios::binary | ios::in);
	infile.read(reinterpret_cast<char*>(h), sizeof(double) * N);
	//problem2(h);
	problem3(h);
	problem4(h);
	
	return(0);
}