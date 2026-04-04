//  Program: ChienSearch.cc
//  Todd K. Moon

#include "ChienSearch.h"
#include <vector>

GFNUM2m *
ChienSearch::Search(GFNUM2m *poly, int nu, int & nroots)
// Lambda(x) = poly[0] + poly[1] x + ... + poly[nu]x^nu
{
	nroots = 0;
	GFNUM2m* roots = new GFNUM2m[17];
	for(int i = 0; i < 255; i++)
	{
		//cout << "got in the loop i = " << i << endl;
		GFNUM2m evaluation = A^i;
		GFNUM2m sum = GFNUM2m(0);
		for(int j = 1; j <= nu ; j++)
		{
			GFNUM2m temp_evaluation = evaluation;
			temp_evaluation ^= j;
			//cout << "A^i*j = " << temp_evaluation << " poly[j] = " << poly[j] << " prod = " << temp_evaluation*poly[i] << endl;
			sum += temp_evaluation * poly[j];
			//cout << "i = " << i << " j = " << j << " sum = " << sum << endl;
			//cout << "j = " << j << endl;
		}
		sum += poly[0];
		//cout << "After the first loop i = " << i << endl;
		if(sum == 0)
		{
			//cout << evaluation << endl;
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


