#include "FastCRC16.h"

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
	
	vector<unsigned char> data;
	vector<vector<unsigned char>> output_block;
	char c;
	int j = 0;
	int block_counter = 0;
	int error_count = 0;
	int temp;
	FastCRC16 X(g);
	while(infile >> temp)
	{
		if (temp < 0 || temp > 255) {
         std::cerr << "Value out of range: " << temp << std::endl;
         continue;
      }
      data.push_back(static_cast<unsigned char>(temp));
		j++;
		if(j == k+2)
		{
			j = 0;
			block_counter++;
			int rem = X.CRC(data.data()+2, k);
			unsigned char rem1 = (rem >> 8)&0xFF;
			unsigned char rem0 = rem&0xFF;
			rem0 = rem0 - data.data()[0];
			rem1 = rem1 - data.data()[1];
			if((rem0 != 0 ) || (rem1 != 0))
				cout << "An error has occured on block: " 
				<< dec << block_counter << endl;
			output_block.push_back(std::vector<unsigned char>
			(data.begin() + 2, data.end()));

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
		for(unsigned char byte : output_block[i])
		{
			outfile << static_cast<int>(byte);
			outfile << " ";
		}
		outfile << "\n";
	}

}