
#include "BinPolyDiv.h"
#include <vector>
class CRC16 {
	protected:
	BinPolyDiv polydiv;
	public:
	// the divider object
	CRC16(unsigned int crcpoly); // constructor
	int CRC(unsigned char *data, int len);
	// Compute the CRC for the data
	// data=data to be encoded
	// len = number of bytes to be encoded
	// Return value: the 16 bits of
	// parity
	// (data[0] is associated with the
	// highest power of x∧n)
 };