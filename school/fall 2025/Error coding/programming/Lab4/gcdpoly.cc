// gcdpoly.cc --- Implement a gcd algorithm for a general
// polynomial type.  Also, implement a gcd for polynomials with real
// coefficients, truncating coefficients as necessary to avoid
// roundoff problems.
//
// Copyright 2019 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include <math.h>
#include "polynomialT.h"

// declare the gcd function, setting up a default value for the rdeg
template <class T> void
gcd(const polynomialT<T> &a, const polynomialT<T> &b, 
	polynomialT<T> &g,
	polynomialT<T> &s, polynomialT<T> &t, int rdeg=0);

// Function definition: gcd
template <class T>  void
gcd(const polynomialT<T> &a, const polynomialT<T> &b, polynomialT<T> &g,
	polynomialT<T> &s, polynomialT<T> &t, int rdeg)
{
   polynomialT<T> qi;  // quotient
   polynomialT<T> ri, rim1, rim2, si, sim1, sim2, ti, tim1, tim2;
   polynomialT<T> *rip, *rim1p, *rim2p, *sip, *sim1p, *sim2p;
   polynomialT<T> *tip, *tim1p, *tim2p;
   polynomialT<T> *temp;
   T norm;

   // Fill in the blanks...
}