#include "BinConvIIR.h"			// the multi-input convolutional object
#include "matalloc.h"
#include <iostream>
using namespace std;

BinConvIIR::BinConvIIR(int in_k, int in_n, int in_p, unsigned int* h_in,
				 unsigned int g_in)
   : BinConv(in_k,in_n), tf(in_p,in_k,h_in,g_in)
{
   nu = in_p;
};

// encode ins[0] ... ins[k-1] to get the n outputs
unsigned int *
BinConvIIR::encode(const unsigned char *ins)
{
    // initialize outs to 0
    for(int j = 0; j < n; j++) outs[j] = 0; 
    // copy n or k bits, depending on bigger into outs
    int min_k_n = (k < n) ? k : n;
    for(int j = 0; j < min_k_n; j++) {
        outs[j] = (ins[j] & 1);
    }
    // use the transfer function and feed the inputs into it
    if(n > 0) {
        unsigned int tf_out = tf.step(ins) & 1u;
        outs[n-1] = tf_out;
    }
    return outs;
}

unsigned int
BinConvIIR::getstate() const
{
    // use transfer function state
    return tf.getstate();
}

void
BinConvIIR::setstate(unsigned int instate)
{
    // use transfer function set state
    tf.setstate(instate);
}