//
//  Program: testpolygcd.cc

#include <math.h>
#include "ModArnew.h"
#include "polynomialT.cc"
#include "gcd_poly.cc"

// create instantiations of the polynomial class of type ModAr
template class polynomialT<ModAr<5> >;
template class polytemp<ModAr<5> >;
// create instantiations of the polynomial class of type double
template class polynomialT<double>;
template class polytemp<double>;

typedef ModAr<5> Z5;

// Create an instantiation of the gcd function, for polynomials of type ModAr
template <class Z5 > void
gcd(const polynomialT<Z5> &a, const polynomialT<Z5> &b, 
	polynomialT<Z5> &g,
	polynomialT<Z5> &s, polynomialT<Z5> &t, int sdeg);

// Create an instantiation of the gcd function, for polynomials of type double
// template <double> void
// gcd(const polynomialT<double> &a, const polynomialT<double> &b, 
// 	polynomialT<double> &g,
// 	polynomialT<double> &s, polynomialT<double> &t, int sdeg);


int main()
{
	// POLYC(Z5, a, {4, 0, 1, 2, 4, 0, 3, 1});
	// POLYC(Z5, b, {4, 2, 0, 3, 0, 0, 1});
	polynomialT<Z5> s_p,s1,s2,s4,s5, t_p,t1,t2,t4,t5, g_p,g1,g2,g4,g5;
	polynomialT<double> s3,t3,g3,spd,tpd,gpd;
	POLYC(Z5,a_p,{1,2,3,4,3,2});
	POLYC(Z5,b_p,{3,4,3,2,1});
	POLYC(double,apd,{4,1,2,1});
	POLYC(double,bpd,{4,3,1});
	POLYC(Z5,a1,{1,0,0,1,3,0,4,3});
	POLYC(Z5,b1,{0,1,0,1,4});
	POLYC(Z5,a2,{1,2,3,4,3,2});
	POLYC(Z5,b2,{3,4,3,2,1});
	POLYC(double,a3,{2,8,10,4});
	POLYC(double,b3,{1,7,14,8});
	POLYC(Z5,a4,{0,0,0,0,0,0,0,0,1});
	POLYC(Z5,b4,{3,2,3,1,4,0,4,3});
	// POLYC(Z5,a4,{0,0,0,0,0,0,1});
	// POLYC(Z5,b4,{2,3,4,2,2,3});
	int p4 = 4;
	int p5 = 4;
	POLYC(Z5,a5,{0,0,0,0,0,0,0,0,1});
	POLYC(Z5,b5,{3,2,1,0,4,3,2,1});
	int s_int,t_int,g_int,a_int,b_int;
	a1.setprintdir(1);
	a2.setprintdir(1);
	a3.setprintdir(1);
	
	a_int = 24;b_int=18;
	cout << "a_int="<<a_int<<endl;
	cout << "b_int="<<b_int<<endl;
	gcd_int(a_int,b_int,g_int,s_int,t_int);
	cout <<"g_int="<<g_int<<endl;
	cout<<"s_int="<<s_int<<endl;
	cout<<"t_int="<<t_int<<endl<<endl;
	
	a_int = 966;b_int=851;
	cout << "a_int="<<a_int<<endl;
	cout << "b_int="<<b_int<<endl;
	gcd_int(a_int,b_int,g_int,s_int,t_int);
	cout <<"g_int="<<g_int<<endl;
	cout<<"s_int="<<s_int<<endl;
	cout<<"t_int="<<t_int<<endl<<endl;
	
	cout << "a_p=" << a_p << endl;
   cout << "b_p=" << b_p << endl;
   gcd(a_p,b_p,g_p,s_p,t_p);
   cout << "g_p=" << g_p << endl; 
   cout << "s_p=" << s_p << endl;
   cout << "t_p=" << t_p << endl;
   cout << "\n";
	
	cout << "apd=" << apd << endl;
   cout << "bpd=" << bpd << endl;
   gcd(apd,bpd,gpd,spd,tpd);
   cout << "gpd=" << gpd << endl; 
   cout << "spd=" << spd << endl;
   cout << "tpd=" << tpd << endl;
   cout << "\n";
	

   cout << "a1=" << a1 << endl;
   cout << "b1=" << b1 << endl;
   gcd(a1,b1,g1,s1,t1);
   cout << "g1=" << g1 << endl; 
   cout << "s1=" << s1 << endl;
   cout << "t1=" << t1 << endl;
   cout << "\n";
	
	cout << "a2=" << a2 << endl;
   cout << "b2=" << b2 << endl;
   gcd(a2,b2,g2,s2,t2);
   cout << "g2=" << g2 << endl; 
   cout << "s2=" << s2 << endl;
   cout << "t2=" << t2 << endl;
   cout << "\n";
	
	cout << "a3=" << a3 << endl;
   cout << "b3=" << b3 << endl;
   gcd(a3,b3,g3,s3,t3);
   cout << "g3=" << g3 << endl; 
   cout << "s3=" << s3 << endl;
   cout << "t3=" << t3 << endl;
	cout<<"\n";
	
	polynomialT<Z5>inverse4 = polynomialT<Z5>(1);
	// cout << "a4=" << a4 << endl;
   // cout << "b4=" << b4 << endl;
   sugiyama(a4,b4,g4,s4,t4,p4);
   cout << "r4=" << g4*inverse4 << endl; 
   cout << "s4=" << s4*inverse4 << endl;
   cout << "t4=" << t4*inverse4 << endl;
	cout << "b4*t4="<<t4*inverse4*b4<<endl<<endl;
	
	cout << "n=0, s4 = (-2*0 - 4*1 - 2*2 - 1*3) mod 5 = 4" << endl;
	cout << "n=1, s5 = (-2*4 - 4*0 - 2*1 - 1*2) mod 5 = 3" << endl;
	cout << "n=2, s6 = (-2*3 - 4*4 - 2*0 - 1*1) mod 5 = 2" << endl;
	cout << "n=3, s7 = (-2*2 - 4*3 - 2*4 - 1*0) mod 5 = 1" << endl;
	
	polynomialT<Z5>inverse5 = polynomialT<Z5>(1);
	sugiyama(a5,b5,g5,s5,t5,p5);
   cout << "r5=" << g5*inverse5 << endl; 
   cout << "s5=" << s5*inverse5 << endl;
   cout << "t5=" << t5*inverse5 << endl;
	cout << "b5*t5="<<t5*inverse5*b5<<endl;

	cout << "n=0, s2 = (2*2 + 4*3) mod 5 = 1" << endl;
	cout << "n=1, s3 = (2*1 + 4*2) mod 5 = 0" << endl;
	cout << "n=2, s4 = (2*0 + 4*1) mod 5 = 4" << endl;
	cout << "n=3, s5 = (2*4 + 4*0) mod 5 = 3" << endl;
	cout << "n=4, s6 = (2*3 + 4*4) mod 5 = 2" << endl;
	cout << "n=5, s7 = (2*2 + 4*3) mod 5 = 1" << endl;

}

/*
Local Variables:
compile-command:"g++ -o testpolygcd -g testpolygcd.cc"
End:
*/


