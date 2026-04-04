#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
using namespace std;
#include <stdio.h>
#include <bitset>
#include <vector>
#include <string>


class FastCRC16 {
protected:
	static int crctable[256];
	unsigned char crc0, crc1;
	
public:
	FastCRC16(unsigned int crcpoly);
	int CRC(unsigned char *data, int len);
	//compute the CRC for the data
	//data[0] corresponds to the 
	//highest powers of x
};