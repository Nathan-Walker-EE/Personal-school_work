// RSdec.cc -- a general Reed-Solomon decoder
// Todd K. Moon
// Copyright 2004 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include "RSdec.h"

#include "berlmass.cc"

//#include "gcdpoly.cc"
// #define BMALG					// define this to use BM alg. for decoding
// otherwise, Euclidean algorithm is used for decoding


// construtor
RSdec::RSdec(int t_in, int n_in, GFNUM2m A1_in)
   :Searcher(t_in)
{
   t = t_in;
   t2 = 2*t_in;
   n = n_in;
   A1 = A1_in;
   s = new GFNUM2m[2*t];
   Lambda = new GFNUM2m[t+1];
   spoly.setc(t2,1);  // allocate space for this
   Lambdap.setc(t,1); // allocate space for this
}

int RSdec::decode(unsigned char *r, unsigned char *dec)
{
   int i;
   GFNUM2m rgf[n];
   GFNUM2m decgf[n];
   for(i = 0; i < n; i++) {
	  rgf[i] = r[i];
   }
   int ret = decode(rgf,decgf);
   for(i = 0; i < n; i++) {
	  dec[i] = decgf[i].getv();
   }
   return ret;
}

int
RSdec::decode(GFNUM2m *r, GFNUM2m *dec)
{
   int i,j;
   int errloc;
   GFNUM2m p,dp2;			  // fast computation of formal derivative
   GFNUM2m x;
   GFNUM2m *rs;					// array of roots
	int no_error = 1;

   // Step 1: evaluate the syndromes
	// cout << "Syndromes are " << endl;
   for(i = 0; i < 2*t; i++){
		GFNUM2m sum = GFNUM2m(0);
		GFNUM2m evaluating_term_i = A^(i+1);
		//cout << "i = " << i << " term is " << evaluating_term << endl;
		for(j = 0; j < n;j++){
			sum += r[j]*(evaluating_term_i^j);
			//cout << "i = " << i << " j = " << j << " evaluating_term * r[j] = " << evaluating_term * r[j] << endl;
		}
		s[i] = sum;
		// cout << s[i] << " ";
	}
	// cout << endl; 
	// cout << 1 + A + (A^2) << endl;
	// cout << 1 + (A^2) + (A^4) << endl;
	// cout << 1 + (A^3) + (A^6) << endl;
	// cout << 1 + (A^4) + (A^8) << endl;
	// cout << 1 + (A^5) + (A^10) << endl;
	// cout << 1 + (A^6) + (A^12) << endl;
	
   // Step 2: Determine the error locator polynomial
   polynomialT<GFNUM2m> error_locator_poly = berlmass(s,t2);
	// cout << "error_locator_poly = " << error_locator_poly << endl;
	if(error_locator_poly.getdegree() == 0){
		for(i = 0; i < n; i++){
			dec[i] = r[i];
		}
		return 1;
	}
	// cout << "Lambda = " ;
	for(i = 0; i < t+1; i++){
		if(i <= error_locator_poly.getdegree())
			Lambda[i] = error_locator_poly[i];
		else
			Lambda[i] = GFNUM2m(0);
		
		// cout << Lambda[i] << " " ;
		//error_locator_poly[i] = GFNUM2m(1);
	}
	// cout << endl;
   // Step 3: Find the roots of the error locator polynomial
   int nroots;
	if(error_locator_poly.getdegree()>0){
		rs = Searcher.Search(Lambda, error_locator_poly.getdegree(), nroots);
	}
	// cout << "nroots = " << nroots << " roots of the polynomial = " << rs[0] << " " << rs[1] << " " << rs[2] << endl;

   // Step 4: Find error values: Not necessary for binary BCH codes
	polynomialT<GFNUM2m> syn_poly(t2-1,s);
	polynomialT<GFNUM2m> prod = syn_poly*error_locator_poly;
	polynomialT<GFNUM2m> xt2(GFNUM2m(0));
	xt2.setc(t2,GFNUM2m(1));
	Omega = prod % xt2;
	// cout << " Omega = " << Omega << endl;
	
   //determine a function to compute the formal derivative of a polynomial.
	polynomialT<GFNUM2m> dx_coeff;
	dx_coeff.setc(error_locator_poly.getdegree()-1, GFNUM2m(0));
	for(i = 1; i <= error_locator_poly.getdegree();i++){
		GFNUM2m coeff = error_locator_poly[i];
		if(i%2 != 0){
			dx_coeff.setc(i-1,coeff);
			// cout << "dx_coeff[" << i - 1 << "] = " << coeff << endl;;
		} else {
			dx_coeff.setc(i-1, GFNUM2m(0));
			// cout << "dx_coeff[" << i - 1 << "] = 0"  << endl;
		}
		//dx_coeffs[i] = error_locator_poly[i+1]*GFNUM2m(i+1);
	}
	
	for(i = 0; i < n; i++){
		dec[i] = r[i];
	}
	for(i = 0; i < nroots; i++){
		errloc = (GFNUM2m::getN() - rs[i].getp()) % GFNUM2m::getN();
		GFNUM2m errval = -(Omega.evaluate(rs[i]) / (dx_coeff.evaluate(rs[i])));
		// cout << errloc << " " << errval << endl;
		dec[errloc] += errval;
	}
	
   return 1;
}


/*
Local Variables:
compile-command: "g++ -c -g RSdec.cc"
End:
*/
	  
