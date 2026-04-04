#include "BinConvFIR.h"			// the multi-input convolutional object
#include "matalloc.h"
#include <iostream>
using namespace std;

BinConvFIR::BinConvFIR(int in_k, int in_n, int *degs, unsigned int** h_in)
   : BinConv(in_k,in_n)
{
   int i,j;

   CALLOCMATRIX(h,unsigned int, k,n);
   nui = new int[k];
   mem = new unsigned int[k];
   maxdeg = 0;
   nu = 0;
   for(i = 0; i < k; i++) {
	  nui[i] = degs[i];
	  if(nui[i] > maxdeg) maxdeg = nui[i];
	  nu += nui[i];
	  for(j = 0; j < n; j++) {
		 h[i][j] = h_in[i][j];
	  }
   }
}

// encode ins[0] ... ins[k-1] to get the n outputs
unsigned int *
BinConvFIR::encode(const unsigned char *ins)
{
    // initiailize outs to 0
    for(int j = 0; j < n; j++) outs[j] = 0; 

    // loop over all outputs
    for(int j = 0; j < n; j++) {
        unsigned int outj = 0;
        // loop over the inputs
        for(int i = 0; i < k; i++) {
            // get the generator polynomial for each output and input
            unsigned int poly = h[i][j];
            // if D^0 = 1, include the input. 
            if(poly & 1) outj ^= (ins[i] & 1);
            // get the taps (remove the first bit with outputs)
            unsigned int taps = poly >> 1;
            // loop over max degree 
            for(int d = 1; d <= nui[i]; d++) {
                if(taps & 1) { // if coefficient of D^d is 1
                    outj ^= (mem[i] >> (d-1)) & 1; // include the memory
                }
                taps >>= 1; // remove the next bit
                if(!taps) break;
            }
        }
        outs[j] = outj; // put in the output
    }

    // update the memory
    for(int i = 0; i < k; i++) {
        mem[i] = ((mem[i] << 1) | (ins[i] & 1)); // shift over and put in input
        // mask to nui[i] bits
        if(nui[i] < 31)
            mem[i] &= ((1 << nui[i]) - 1);
    }

    return outs;
}

unsigned int BinConvFIR::getstate() const
{
    unsigned int state = 0;
    int shift = 0;
    // pack the memory into one integer
    for(int i = 0; i < k; i++) {
        state |= (mem[i] << shift);
        shift += nui[i];
    }
    return state;
}

void BinConvFIR::setstate(unsigned int state)
{
    for(int i = 0; i < k; i++) {
        if(nui[i] == 0) {
            mem[i] = 0;
            continue;
        }
        // set state into memory
        unsigned int mask = (1 << nui[i]) - 1;
        mem[i] = state & mask;
        state >>= nui[i];
    }
}