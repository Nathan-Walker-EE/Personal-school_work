
#include "CRC16.h"
#include <algorithm>


// the divider object
CRC16::CRC16(unsigned int crcpoly)
        : polydiv(crcpoly, 16)   // initialize BinPolyDiv with (poly, degree)
    {}
	 
// example : CRC ( [0 1 0 1 0 1..], 2)
// Compute the CRC for the data
// data=data to be encoded
// len = number of bytes to be encoded
// Return value: the 16 bits of
// parity
// (data[0] is associated with the
// highest power of x∧n)
int CRC16::CRC(unsigned char *data, int len)
{
	int numzeros = 16;
	vector<unsigned char> vec(data, data + len*8);
	vec.insert(vec.begin(), numzeros, 0);
	
	/*for(unsigned char bit:d2)
	{cout << static_cast<int>(bit) << " ";}*/

	unsigned char q[len*8];
	int qdegree = len*8-1;
	int rdegree;
	
	unsigned int rem = polydiv.div(vec.data(), len*8-1+numzeros, q, qdegree, rdegree);
	
	return rem;
}
	