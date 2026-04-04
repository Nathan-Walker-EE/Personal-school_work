#include "CRC16.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>

int main(int argc, char *argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	unsigned int g = 0x18005;
	if(argc==1) 
	{
		cout << "Usage is crcenc K filein fileout" << endl;
		cout << "Message block length in bytes" << endl;
		exit(-1);
	}
	
	unsigned int k = atoi(argv[1]);
	//cout << "k = " <<  k << endl;
	
	ifstream infile(argv[2]);
	if(!infile)
	{
		cerr << "Cannot open file " << argv[1] << endl;
		exit(-1);
	}
	
	vector<unsigned char> data;
	char c;
	while(infile.get(c))
	{
		if(c == '0') data.push_back(0);
		else if(c == '1') data.push_back(1);
	}
	
	// for(unsigned char bit : data)
	// {
		// cout << static_cast<int>(bit);
	// }
	// cout << endl;
	
	int bitsperblock = k * 8;
	int numBlocks = (data.size() + bitsperblock - 1)/bitsperblock;
	//cout << numBlocks << endl;
	vector<vector<unsigned char>> blocks(numBlocks);
	
	for(int i = 0; i < numBlocks; i++)
	{
		int start = i * bitsperblock;
		int end = min(start + bitsperblock, (int)data.size());
		blocks[i].insert(blocks[i].end(), data.begin() + start, data.begin() + end);
		// cout << "i = " << i << endl;
	}
	
	//vector<vector<unsigned char>> out_blocks(numBlocks);
	CRC16 X(g);
	unsigned char temp[k*8];
	const int crcbits = 16;
	vector<unsigned char> crcparity(crcbits);
	for(size_t i = 0; i < blocks.size(); ++i)
	{
		unsigned int rem = X.CRC(blocks[i].data(), k);
		for(int b = crcbits - 1; b >= 0; --b)
		{
			crcparity[crcbits-1-b] = (rem >> b) & 1;
		}
		reverse(crcparity.begin(), crcparity.end());
		blocks[i].insert(blocks[i].begin(), crcparity.begin(),crcparity.end());
	}
	
	ofstream outfile(argv[3]);
	if(!outfile)
	{
		cerr << "Cannot open output.txt for writing" << endl;
		exit(-1);
	}
	
	for(size_t i = 0; i < blocks.size(); ++i)
	{
		for(unsigned char bit : blocks[i])
		{
			outfile << static_cast<int>(bit);
		}
		outfile << "\n";
	}
	
	outfile.close();
	infile.close();
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration_ms = end - start;

   std::cout << "Function took " << duration_ms.count() << " ms" << std::endl;
}
//0110111011010100
//g++ -o crcenc -g crcenc.cc CRC16.cc BinPolyDiv.cc BinLFSR.cc
