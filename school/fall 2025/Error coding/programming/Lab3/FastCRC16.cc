#include "FastCRC16.h"
#include "BinPolyDiv.h"
#define NO_BUG

int FastCRC16::crctable[256];

FastCRC16::FastCRC16(unsigned int crcpoly)
{
	BinPolyDiv X(crcpoly, 16);   // initialize BinPolyDiv with (poly, degree)
	for(int i = 0; i < 256;  i++)
	{
		bitset<8> bits(i); // 8-bit representation
		string s = bits.to_string(); // "00001101"	

		// Convert to unsigned char*
		std::vector<unsigned char> vec;
      vec.reserve(8 + 16); // reserve 24 slots
      for (char ch : s)
         vec.push_back(ch == '1' ? 1 : 0);
		vec.insert(vec.end(), 16, 0);
		reverse(vec.begin(), vec.end());
		#ifdef DEBUG1
		for(unsigned char bit:vec)
			cout << static_cast<int>(bit);
		cout << endl;
		#endif
		unsigned char q[8]; int qdegree = 7; int remdegree;
		int table_entry = X.div(vec.data(), 23, q, qdegree, remdegree);
		crctable[i] = table_entry;
	}
	#ifdef DEBUG2
	for(int i = 0; i < 256; i++)
	{
		cout << "table entry " << dec << i << "= 0x" << hex << crctable[i] << endl;
	}
	#endif
}

int FastCRC16::CRC(unsigned char* data, int bytes)
{
	unsigned char crc1 = 0, crc0 = 0;
	for(int i = 0; i < bytes; i++)
	{
		unsigned char t = data[i] ^ crc1;
		int temp = crc0;
		temp = temp << 8;
		#ifdef DEBUG_CRC
		cout << "[crc0, 0] = " << hex << temp << endl;
		#endif
		temp = temp ^ crctable[t];
		crc0 = 0xFF & temp;
		crc1 = 0xFF & (temp >> 8);
		#ifdef DEBUG_CRC
		cout << "i = " << dec << i << ", t = " << hex << static_cast<int>(t) << 
		",[crc1 crc0] = " << static_cast<int>(crc1) << static_cast<int>(crc0) << endl;
		#endif
	}
	int parity = (crc1 << 8) + crc0;
	return parity;
}
//g++ -o testFastCRC16 -g testFastCRC16.cc FastCRC16.cc BinPolyDiv.cc BinLFSR.cc
