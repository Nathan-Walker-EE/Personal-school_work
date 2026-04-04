#include "FastCRC16.h"

int main()
{
	unsigned int g = 0x18005;
	FastCRC16 X(g);
	unsigned char d[2] = {109, 39};
	int parity = X.CRC(d, 2);
	
	cout << hex << parity;
}

//g++ -o testFastCRC16 -g testFastCRC16.cc FastCRC16.cc BinPolyDiv.cc BinLFSR.cc
