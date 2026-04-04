//  Program: testBM.cc --- test the Berlekamp Massey algorithm
//  Todd K. Moon

#include "GF2.h"                // provide binary arithmetic
#include "GFNUM2m.h"            // provide GF(2^m) arithmetic
#include "ModArnew.h"              // Provide arithmetic mod 5
#include "polynomialT.cc"       // provide polynomial operations
#include "berlmass.cc"          // Berlekamp-Massey algorithm
#include "berlmass2.cc"         // Berlekamp-Massey, w/o polynomials

// create instantiations of the polynomial class of type GF2
template class polynomialT<GF2>;
template class polytemp<GF2>;
typedef ModAr<5> Z5;


// create instantiations of the polynomial class of type GFNUM2m
template class polynomialT<GFNUM2m>;
template class polytemp<GFNUM2m>;

// create instantiations of the polynomial class of type ModAr
template class polynomialT<Z5>;
template class polytemp<Z5>;

int main()
{
   int j;
   int l;
   // GF2 test
   //GF2 d1[] = {1,1,1,0,1,0,0};           // array of data
	GF2 d1[] = {1, 1, 1, 1, 1, 1};
   polynomialT<GF2> c = berlmass(d1,6);  // polynomial form
   cout << "prelim (2) GF2 polynomial form: c=" << c << endl<<endl;
	
   // ModAr test
   Z5 d2[] = {2,3,4,2,2,3};          // array of data
   polynomialT<Z5> c2 = berlmass(d2,6); // polynomial form
   cout << "prelim (1) Z5 c=" << c2 << endl<<endl;

   GFNUM2m::initgf(4,0x13);             // Initialize field: 1 011 = 1+x+x^4
      
   GFNUM2m d4[] = {0, A^3, A^4, A^7};// array of data
   polynomialT<GFNUM2m> c4 = berlmass(d4,4);// polynomial form
   cout << "prelim (3) GF(16) c=" << c4 << endl<<endl;
	
	//problem 3
	GFNUM2m d5[] = {A^8, A, A^13, A^2, A^5, A^11};
	polynomialT<GFNUM2m> c5 = berlmass(d5, 6);
	cout << "problem (3) GF(16) c=" << c5 << endl<<endl;
	
	//problem 4
	GFNUM2m d6[] = { 0, 0, A^5, 0, 1, A^10};
	polynomialT<GFNUM2m> c6 = berlmass(d6, 6);
	cout << "problem (4) GF(16) c=" << c6 << endl<<endl;
	
	//problem 5 (a)
	GFNUM2m d7[] = {1, 1, A^10, 1, A^10, A^5};
	polynomialT<GFNUM2m> c7 = berlmassBCH(d7, 6);
	cout << "problem 5(a) GF(16) c=" << c7 << endl<<endl;
	
	//problem 5(b)
	GFNUM2m d8[] = {A^14, A^13, 1, A^11, A^5, 1};
	polynomialT<GFNUM2m> c8 = berlmassBCH(d8,6);
	cout << "problem 5(b) GF(16) c=" << c8 << endl<<endl;
}

/*
Local Variables:
compile-command: "g++ -o testBM -g testBM.cc GFNUM2m.cc BinLFSR.cc"
End:
*/
