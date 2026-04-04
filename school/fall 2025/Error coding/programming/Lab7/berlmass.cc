//  Program: berlmass.cc --- implement the Berlekamp-Massey algorithm
//  Todd K. Moon

#ifndef BERLMASS_CC
#define BERLMASS_CC

#include "polynomialT.h"

template <class T> polynomialT<T>
berlmassBCH(const T* s, int n)
{
	int L = 0;
   polynomialT<T> c;
   c[0] = T(1);
   // cout << "c=" << c << endl;
   polynomialT<T> p(c), t;      // p is previous, t is temporary
   int shift = 1;               // n-m
   T dm = T(1);                 // previous discrepancy
   T d;                         // current discrepancy
   int j;
   for(int k=0; k < n; k=k+2) {
		T sum = T(0);
		for(int i = 0; i < L; i++)
		{
			if( (i + 1) <= c.getdegree())
			{
				// cout << "c[i+1]=" << c[i + 1] << " s[k-i]=" << s[k-i] << endl;
				sum += c[i+1]*s[k-i-1];
			}
		}
		
		d = s[k] + sum;
		// cout << "s[k]=" << s[k] << " sum=" << sum << endl;
		if (d == 0)
			shift++;
		else
			if((2*L) >= (k+1))
			{
				polynomialT<T> xl(T(0));
				T d_prod = d/dm;
				// cout << "dm=" << dm << " d=" << d << " d_prod=" << d_prod << endl;
				xl.setc(shift, d_prod);
				c = c - (xl * p);
				shift++;
			}
			else
			{
				t = c;
				polynomialT<T> xl(T(0));
				T d_prod = d/dm;
				xl.setc(shift, d_prod);
				c = c - (xl * p);
				L = (k+1) - L;
				p = t;
				dm = d;
				shift = 1;
			}
		shift++;
	  // fill in the blanks ....
	  
//cout<<"k="<<k << " s=" << s[k] << " d=" << d << "  c=" << c << "  L=" << L;
//cout << " p=" << p << "  l=" << shift << " dm=" << dm << endl;

   }
	// cout << "exiting function" << endl;
   return c;	
}

template <class T> polynomialT<T>
berlmass(const T* s, int n)
// s = input coefficients s[0],s[1],... s[n-1]
// returns = final connection polynomial
{
   int L = 0;
   polynomialT<T> c;
   c[0] = T(1);
   // cout << "c=" << c << endl;
   polynomialT<T> p(c), t;      // p is previous, t is temporary
   int shift = 1;               // n-m
   T dm = T(1);                 // previous discrepancy
   T d;                         // current discrepancy
   int j;
   for(int k=0; k < n; k++) {
		T sum = T(0);
		for(int i = 0; i < L; i++)
		{
			if( (i + 1) <= c.getdegree())
			{
				// cout << "c[i+1]=" << c[i + 1] << " s[k-i]=" << s[k-i] << endl;
				sum += c[i+1]*s[k-i-1];
			}
		}
		d = s[k] + sum;
		// cout << "s[k]=" << s[k] << " sum=" << sum << endl;
		if (d == 0)
			shift++;
		else
			if((2*L) >= (k+1))
			{
				polynomialT<T> xl(T(0));
				T d_prod = d/dm;
				// cout << "dm=" << dm << " d=" << d << " d_prod=" << d_prod << endl;
				xl.setc(shift, d_prod);
				c = c - (xl * p);
				shift++;
			}
			else
			{
				t = c;
				polynomialT<T> xl(T(0));
				T d_prod = d/dm;
				xl.setc(shift, d_prod);
				c = c - (xl * p);
				L = (k+1) - L;
				p = t;
				dm = d;
				shift = 1;
			}
	  // fill in the blanks ....
	  
//cout<<"k="<<k << " s=" << s[k] << " d=" << d << "  c=" << c << "  L=" << L;
//cout << " p=" << p << "  l=" << shift << " dm=" << dm << endl;

   }
	// cout << "exiting function" << endl;
   return c;
}


#endif
/*
Local Variables:
compile-command: "g++ -g berlmass.cc"
End:
*/


