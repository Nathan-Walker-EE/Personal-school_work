// RSenc.cc -- a general RS encoder
// Todd K. Moon
// Copyright 2004 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include "RSenc.h"
#include "polynomialT.cc"

template class polynomialT<GFNUM2m>;
template class polytemp<GFNUM2m>;

RSenc::RSenc(int n_in, int k_in, int t_in, int j0_in, GFNUM2m A1_in)
// constructor
{
   n = n_in;
   k = k_in;
   t = t_in;
   j0 = j0_in;
   A1 = A1_in;
	c.setc(n-1, GFNUM2m(0));
   g = GFNUM2m(1);
   GFNUM2m lc[2] = {1,1};
   polynomialT<GFNUM2m> l(1,lc);  // set up the linear term
   for(int i=0; i < 2*t; i++) {
	  l[0] = (A1^(j0+i));
	  g *= l;
   }
   m.setc(k-1,1);				// allocate space for message polynomial
}

GFNUM2m *
RSenc::encode(GFNUM2m *m_in)
// encode from Galios field elements
{
   // fill in the blanks ...
	polynomialT<GFNUM2m> m_poly(k, m_in);
	polynomialT<GFNUM2m> xl(GFNUM2m(0));
	xl.setc(n-k, GFNUM2m(1));
	polynomialT<GFNUM2m> temp;
	polynomialT<GFNUM2m> remainder;
	temp = m_poly * xl;
	temp.divide(g);
	temp = temp + temp.getlastremainder();
	GFNUM2m c_row[n];
	for(int i = 0; i < n; i++){
		if(i <= temp.getdegree())
			c.setc(i,temp[i]);
		else
			c.setc(i,GFNUM2m(0));
	}
	GFNUM2m *dumb;
	return dumb;
}

void
RSenc::encode(unsigned char *m_in, unsigned char *c_out)
// encode from chars
{
   // fill in the blanks
}
   
/*
Local Variables:
compile-command: "g++ -c -g RSenc.cc"
End:
*/
	  
