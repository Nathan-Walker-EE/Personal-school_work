#include "FastCRC16.h"
#include <algorithm>

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
	int temp;  // read into an int first

   while (infile >> temp)// operator>> skips whitespace automatically
	{
		if (temp < 0 || temp > 255) {
         std::cerr << "Value out of range: " << temp << std::endl;
         continue;
      }
      data.push_back(static_cast<unsigned char>(temp));
   }
	
	int numBlocks = (data.size() + k - 1)/k;
	vector<vector<unsigned char>> blocks(numBlocks);
	
	for(int i = 0; i < numBlocks; i++)
	{
		int start = i;
		int end = min((int)(start + k), (int)data.size());
		blocks[i].insert(blocks[i].end(), data.begin() + start, data.begin() + end);
	}
	
	FastCRC16 X(g);
	for(size_t i = 0; i < blocks.size(); ++i)
	{
		int rem = X.CRC(blocks[i].data(), k);
		unsigned char rem1 = (rem >> 8)&0xFF;
		unsigned char rem0 = rem&0xFF;
		blocks[i].insert(blocks[i].begin(), rem1);
		blocks[i].insert(blocks[i].begin(), rem0);
	}
	
	ofstream outfile(argv[3]);
	if(!outfile)
	{
		cerr << "Cannot open output.txt for writing" << endl;
		exit(-1);
	}
	for(size_t i = 0; i < blocks.size(); ++i)
	{
		for(unsigned char byte : blocks[i])
		{
			outfile << static_cast<int>(byte);
			outfile << " ";
		}
		outfile << "\n";
	}
	
	outfile.close();
	infile.close();
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration_ms = end - start;

   std::cout << "Function took " << duration_ms.count() << " ms" << std::endl;
}

// g++ -o FastCRC16_enc -g FastCRC16_enc.cc FastCRC16.cc BinPolyDiv.cc BinLFSR.cc
