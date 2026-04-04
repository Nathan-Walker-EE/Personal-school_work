// RSdec.cc -- a general Reed-Solomon decoder
// Todd K. Moon
// Copyright 2004 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include "RSdec.h"
//#include "berlmass2.cc"

#include "berlmass.cc"

//#include "gcdpoly.cc"
#define BMALG					// define this to use BM alg. for decoding
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
    GFNUM2m* rgf = new GFNUM2m[n];
    GFNUM2m* decgf = new GFNUM2m[n];

    for(i = 0; i < n; i++) {
	    rgf[i] = r[i];
    }
    int ret = decode(rgf,decgf);
    for(i = 0; i < n; i++) {
	    dec[i] = decgf[i].getv();
    }
    return ret;
}


#ifdef BMALG
int
RSdec::decode(GFNUM2m *r, GFNUM2m *dec)
{
    int i,j;
    int errloc;
    GFNUM2m p,dp2;			  // fast computation of formal derivative
    GFNUM2m x;
    GFNUM2m *rs;					// array of roots

    for(i = 0; i < n; i++) {  // copy over the input bits
	    dec[i] = r[i];
    }
    // Step 1: evaluate the syndromes
    for (i = 0; i < 2*t; i++)
    {
        GFNUM2m Si(0);
        GFNUM2m alpha_i = (A ^ (i+1));

        for (int j = 0; j < n; j++) {
            if (r[j] == 1) {
                Si += (alpha_i ^ j);  // accumulate α^{i*j}
            }
        }
        s[i] = Si;
        cout << "S(" << i << ") = " << Si << " " ;
    }
    cout << endl;

    // Step 2: Determine the error locator polynomial
    polynomialT<GFNUM2m> poly = berlmass(s, 2*t);
    int deg = poly.getdegree() + 1;
    for (int i = 0; i < deg; i++) {
        Lambda[i] = poly[i];
    }
 
    // Step 3: Find the roots of the error locator polynomial
    int nroots; 
    nu = poly.getdegree();
    ChienSearch Searcher(t);
    rs = Searcher.Search(Lambda,nu); 
    nroots = Searcher.getnroots();
 
    // Step 4: Find error values: Not necessary for binary BCH codes
    // Forney's Algorithm
    // make polynomial S(x), Lambda(x), Omega(x)
    for (i = 0; i < 2 * t; i++) spoly.setc(i, s[i]);
    Lambdap = poly; 
    polynomialT<GFNUM2m> x_2t;
    x_2t.setc(2 * t, GFNUM2m(1));
    Omega = (spoly * Lambdap) % x_2t;
    // derivative of Lambda(x)
    polynomialT<GFNUM2m> dLambda;
    dLambda.setc(Lambdap.getdegree() - 1, GFNUM2m(0));  // allocate space
    for (int i = 1; i <= Lambdap.getdegree(); i++) {
        GFNUM2m coeff = Lambdap[i];
        // Multiply by i in GF(2^m); in GF(2^m), even i → 0, odd i → 1
        if (i % 2 != 0) {
            dLambda.setc(i - 1, coeff);
        } else {
            dLambda.setc(i - 1, GFNUM2m(0));
        }
    }
    // find correct value
    for(i = 0; i < n; i++) {  // copy over the input bits
        dec[i] = r[i];
    }
    for (i = 0; i < nroots; i++)
    {
        errloc = (GFNUM2m::getN() - rs[i].getp()) % GFNUM2m::getN();
        GFNUM2m errval = - Omega.evaluate(rs[i]) / (1/rs[i] * dLambda.evaluate(rs[i]));
        dec[errloc] = errval;
    } 

   return 1;
}


#else
#include "sugiyama.cpp"

// decoding using the Euclidean algorithm
int
RSdec::decode(GFNUM2m *r, GFNUM2m *dec)
{
    int i,j;
    int errloc;
    GFNUM2m p,dp2;			  // fast computation of formal derivative
    GFNUM2m x;
    GFNUM2m *rs;					// array of roots

    for(i = 0; i < n; i++) {  // copy over the input bits
	    dec[i] = r[i];
    }
    // Step 1: evaluate the syndromes
    for (i = 0; i < 2*t; i++)
    {
        GFNUM2m Si(0);
        GFNUM2m alpha_i = (A ^ (i+1));

        for (int j = 0; j < n; j++) {
            if (r[j] == 1) {
                Si += (alpha_i ^ j);  // accumulate α^{i*j}
            }
        }
        s[i] = Si;
        cout << "S(" << i << ") = " << Si << " " ;
    }
    cout << endl;

    // Step 2: Determine the error locator polynomial
    for (i = 0; i < 2 * t; i++) spoly.setc(i, s[i]);
    polynomialT<GFNUM2m> x_2t;
    x_2t.setc(2 * t, GFNUM2m(1));
    polynomialT<GFNUM2m> poly, s_poly, r_poly;
    sugiyama(spoly, s_poly, Omega, poly, x_2t);
    int deg = poly.getdegree() + 1;
    for (int i = 0; i < deg; i++) {
        Lambda[i] = poly[i];
    }
 
    // Step 3: Find the roots of the error locator polynomial
    int nroots; 
    nu = poly.getdegree();
    ChienSearch Searcher(t);
    rs = Searcher.Search(Lambda,nu); 
    nroots = Searcher.getnroots();
 
    // Step 4: Find error values: Not necessary for binary BCH codes
    // Forney's Algorithm
    // make polynomial S(x), Lambda(x), Omega(x)
    Lambdap = poly; 
    Omega = (spoly * Lambdap) % x_2t;
    // derivative of Lambda(x)
    polynomialT<GFNUM2m> dLambda;
    dLambda.setc(Lambdap.getdegree() - 1, GFNUM2m(0));  // allocate space
    for (int i = 1; i <= Lambdap.getdegree(); i++) {
        GFNUM2m coeff = Lambdap[i];
        // Multiply by i in GF(2^m); in GF(2^m), even i → 0, odd i → 1
        if (i % 2 != 0) {
            dLambda.setc(i - 1, coeff);
        } else {
            dLambda.setc(i - 1, GFNUM2m(0));
        }
    }
    // find correct value
    for(i = 0; i < n; i++) {  // copy over the input bits
        dec[i] = r[i];
    }
    for (i = 0; i < nroots; i++)
    {
        errloc = (GFNUM2m::getN() - rs[i].getp()) % GFNUM2m::getN();
        GFNUM2m errval = - Omega.evaluate(rs[i]) / (1/rs[i] * dLambda.evaluate(rs[i]));
        dec[errloc] = errval;
    } 

   return 1;
}


#endif
/*
Local Variables:
compile-command: "g++ -c -g RSdec.cc"
End:
*/
	  