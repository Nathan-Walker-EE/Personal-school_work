#include "polynomialT.h"
// #include "ModAr.h"
#include "ModArnew.h"

#include <iostream>
using namespace std;

void gcd_int(int a, int b, int &g, int &s, int &t)
{
	int r_ = a, r0 = b, s_ = 1, s0 = 0, t_ = 0, t0 = 1;
	int ri = 1;
	int qi, si, ti;
	while(ri != 0)
	{
		qi = r_/r0;
		ri = r_ - (qi * r0);
		si = s_ - (qi*s0);
		ti = t_ - (qi * t0);
		t_ = t0;t0 = ti;
		s_ = s0;s0 = si;
		r_ = r0;r0 = ri;
	}
	s=s_;t=t_;g=r_;
}

template <class T> void gcd(const polynomialT<T> &a,const polynomialT<T> &b,
polynomialT<T> &g, polynomialT<T> &s, polynomialT<T> &t)
{
	polynomialT<T> r_=a,r0=b, ri=a, qi, si, ti,s__;
	polynomialT<T> s_ = polynomialT<T>(1), s0 = polynomialT<T>(0);
   polynomialT<T> t_ = polynomialT<T>(0), t0 = polynomialT<T>(1);
	while(ri != 0)
	{
		// cout << "loop bleh" << endl;
		qi = r_/r0;
		// cout << "qi = " << qi << endl;
		ri = r_ - (qi * r0);
		// cout << "ri = " << ri << endl;
		si = s_ - (qi*s0);
		ti = t_ - (qi * t0);
		t_ = t0;t0 = ti;
		s__ = s_;s_ = s0;s0 = si;
		r_ = r0;r0 = ri;
	}
	s=s_;t=t_;g=r_;
}

template <class T> void sugiyama(const polynomialT<T> &a,const polynomialT<T> &b,
polynomialT<T> &g, polynomialT<T> &s, polynomialT<T> &t, int p)
{
	polynomialT<T> r_=a,r0=b, ri=a, qi, si, ti,s__;
	polynomialT<T> s_ = polynomialT<T>(1), s0 = polynomialT<T>(0);
   polynomialT<T> t_ = polynomialT<T>(0), t0 = polynomialT<T>(1);
	int i = 0;
	while(ri.getdegree() >= p)
	{
		// cout << i++<<endl;
		// cout << "loop bleh" << endl;
		qi = r_/r0;
		// cout << "qi = " << qi << endl;
		ri = r_ - (qi * r0);
		// cout << "ri = " << ri << endl;
		si = s_ - (qi*s0);
		ti = t_ - (qi * t0);
		t_ = t0;t0 = ti;
		s__ = s_;s_ = s0;s0 = si;
		r_ = r0;r0 = ri;
	}
	s=si;t=ti;g=ri;
}