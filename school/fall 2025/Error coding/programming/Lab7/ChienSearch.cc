//  Program: ChienSearch.cc
//  Todd K. Moon

#include "ChienSearch.h"
#include <vector>

GFNUM2m *
ChienSearch::Search(GFNUM2m *poly, int nu, int & nroots)
// Lambda(x) = poly[0] + poly[1] x + ... + poly[nu]x^nu
{
	nroots = 0;
	GFNUM2m* roots = new GFNUM2m[16];
	for(int i = 0; i < 15; i++)
	{
		GFNUM2m evaluation = A^i;
		GFNUM2m sum = GFNUM2m(0);
		for(int j = 0; j <= nu ; j++)
		{
			GFNUM2m temp_evaluation = evaluation;
			temp_evaluation ^= j;
			sum += temp_evaluation * poly[j];
		}
		if(sum == 0)
		{
			//cout << evaluation << endl<<endl;
			roots[nroots++] = evaluation;
		}
	}
	// setnroots(nroots);
   // GFNUM2m *roots = v_roots.data();
	//nroots -= 1;
   return roots;
}
	  


/*
Local Variables:
compile-command: "g++ -c -g ChienSearch.cc"
End:
*/


