// BCHdec.cc -- a general BCH decoder
// Todd K. Moon
#include "BCHdec.h"
#include "berlmass.cc"
//#include "berlmassBCH2.cc"

//#include "berlmassBCH.cc"
#include "polynomialT.cc"

#include "gcd_poly.cc"

BCHdec::BCHdec(int t_in, int n_in, GFNUM2m A1_in) :
   Searcher(t_in)
{
   t = t_in;
   t2 = 2*t_in;
   n = n_in;
   A1 = A1_in;
   s = new GFNUM2m[2*t];
   Lambda = new GFNUM2m[t+1];
}

void
BCHdec::decode(GF2 *r, GF2 *dec)
{
   int i,j;
   int errloc;
   GFNUM2m p;
   GFNUM2m x;
   GFNUM2m *rs;					// array of roots

   // Step 1: evaluate the syndromes

	//cout << endl << "computing the syndromes" << endl;
   for(i = 1; i <= t2; i++){
		GFNUM2m sum = GFNUM2m(0);
		GFNUM2m evaluating_term = A^i;
		sum += r[0];
		for(j = 1; j < 15; j++){
			if(r[j] == GF2(1)){
				sum += evaluating_term;
			}
			evaluating_term *= A^i;
		}
		s[i-1] = sum;
		//cout << s[i-1] << " ";
	}
	//cout << endl;


   // Step 2: Determine the error locator polynomial
	//cout << "computing the error locator polynomial" << endl;
	GFNUM2m incoeffx[7] = {GFNUM2m(0),GFNUM2m(0),GFNUM2m(0),GFNUM2m(0)
	,GFNUM2m(0),GFNUM2m(0),GFNUM2m(1)};
	polynomialT<GFNUM2m> x_temp(6, incoeffx);
	GFNUM2m syndromecoeff[6] = {s[0], s[1], s[2], s[3], s[4], s[5]};
	polynomialT<GFNUM2m> syndrome(5, syndromecoeff);
	polynomialT<GFNUM2m> gsugi, ssugi, tsugi;
	int psugi;
	sugiyama(x_temp,syndrome,gsugi,ssugi,tsugi,psugi);
	//polynomialT<GFNUM2m> c = berlmass(s, t2);
	//polynomialT<GFNUM2m> c = berlmassBCH(s, t2);
	int deg = tsugi.getdegree();
	for(i = 0; i < t+1; i++){
		Lambda[i] = GFNUM2m(0);
		if(i <= deg){
			Lambda[i] = tsugi[i];
		}
		//cout << Lambda[i] << " ";
	}
	//cout << endl;
   // (or berlmass or berlmass2...)

   // Step 3: Find the roots of the error locator polynomial
   int nroots; 
   rs = Searcher.Search(Lambda,nu,nroots); // from ChienSearch

   // Step 4: Find error values: Not necessary for binary BCH codes

   // Step 5: Correct errors corresponding to inverse roots
   for(i = 0; i < n; i++) {  // copy over the input bits
	  dec[i] = r[i];

   }
	if(nroots == 0)
		nroots++;
	//cout << "nroots = "<< nroots << endl;	
	//cout << "error locations" << endl;
   for(i = 0; i < nroots; i++) {
	  errloc = (GFNUM2m::getN() - rs[i].getp()) % GFNUM2m::getN();
	  //cout << errloc << endl;
	  dec[errloc] = 1+dec[errloc];  // complement the bits
   }
}


/*
Local Variables:
compile-command: "g++ -c -g BCHdec.cc"
End:
*/
	  
