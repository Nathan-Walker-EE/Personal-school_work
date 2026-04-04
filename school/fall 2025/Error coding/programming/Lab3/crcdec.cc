#include "CRC16.h"
#include <iostream>
#include <fstream>

vector<unsigned char> intobin(int num, int k)
{
	int num_bits = k * 8;
	vector<unsigned char> bin(num_bits);
	for(int b = num_bits - 1; b >= 0; --b)
	{
		bin[num_bits-1-b] = (num >> b) & 1;
	}
	return bin;
}

int main(int argc, char *argv[])
{
	unsigned int g = 0x18005;
	if(argc==1) 
	{
		cout << "Usage is crcdec K filein fileout" << endl;
		cout << "Message block length in bytes" << endl;
		exit(-1);
	}
	
	unsigned int k = atoi(argv[1]);
	cout << "k = " <<  k << endl;
	
	ifstream infile(argv[2]);
	if(!infile)
	{
		cerr << "Cannot open file " << argv[1] << endl;
		exit(-1);
	}
	
	BinPolyDiv x(g, 16);
	
	vector<unsigned char> data;
	vector<vector<unsigned char>> output_block;
	char c;
	int j = 0;
	int block_counter = 0;
	int error_count = 0;
	while(infile.get(c))
	{
		if(c == '0') 
		{
			data.push_back(0);
			j++;
		}else if(c == '1')
		{
			data.push_back(1);
			j++;
		}
		if(j == k * 8)
		{
			j = 0;
			block_counter++;
			unsigned char q[(k-2)*8];
			int quotientdegree;
			int remdegree;
			unsigned int rem = x.div(data.data(), k*8, q, quotientdegree, remdegree);
			if(rem != 0 )
				cout << "An error has occured in block "<< block_counter << endl << "error_count = " << error_count << endl;
			output_block.push_back(data);
			data.clear();
		}
	}
	
	ofstream outfile(argv[3]);
	if(!outfile)
	{
		cerr << "Cannot open output.txt for writing" << endl;
		exit(-1);
	}
	
	for(size_t i = 0; i < output_block.size(); ++i)
	{
		int p = 0;
		for(unsigned char bit : output_block[i])
		{
			p++;
			if(p > 16)
			{
				outfile << static_cast<int>(bit);
			}
		}
		outfile << "\n";
	}
	
}

//g++ -o crcdec -g crcdec.cc CRC16.cc BinPolyDiv.cc BinLFSR.cc
