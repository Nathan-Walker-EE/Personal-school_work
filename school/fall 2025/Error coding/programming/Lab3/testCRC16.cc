#include "CRC16.h"

#define PRELIM

int main()
{
	#ifdef PRELIM //1 0100 0000 0000 0011
	unsigned int g = 0x18005; //0b1	1000	0000	0000	0101;// right side is high polynomial term
	int gdeg = 16;	
	unsigned char d[] = {1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0};// right side is high polynomial term
	//unsigned char d[] = {0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,1};
	unsigned char q[16];
	int qdegree = 15;
	//testing stupid CRC16
	unsigned char d2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0};
	//unsigned char d2[] = {0,1,1,0,1,1,0,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	BinPolyDiv test(g,16);
	int rdegree;
	int rem2 = test.div(d2, 31, q, qdegree, rdegree);
	CRC16 X(g);
	unsigned int rem = X.CRC(d, 2);
	bitset<16> bin(rem);
	cout << bin << endl;
	bitset<16> bin2(rem2);
	cout << bin2 << endl;
	#endif
	
}

// g++ -o testFastCRC16 -g testFastCRC16.cc FastCRC16.cc BinPolyDiv.cc BinLFSR.cc