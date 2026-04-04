// BinPolyDiv.h -- declarations for BinPolyDiv class
// Todd K. Moon

#include "BinPolyDiv.h"

BinPolyDiv::BinPolyDiv(unsigned int in_g, int in_p)
{
   p = in_p;
   mask = (1<<p)-1;
   g = in_g & mask;
   state = 0;
}

char BinPolyDiv::step(unsigned char input)
{
   char out = (state&mask) >> (p-1);
   //cout << "inside step() " << static_cast<int>(out) << endl;
	unsigned int dummy1 = (state << 1);
	unsigned int dummy2 = (g*out);
	unsigned int dummy3 = ((state<<1)^(g*out) ^ input);
	//cout << state << " " << dummy1 << " " << dummy2<< " "  << static_cast<int>(input)<< " "  << dummy3<< " "  << endl;
   state = ((state<<1)^(g*out) ^ input)&mask;
   return out;
}

unsigned int BinPolyDiv::div(unsigned char *d, int n, unsigned char *q, 
					int &quotientdegree, int &remainderdegree)
// divide d by g. Return quotient in q and remainder as return value (integer)
// d has d0 first, so start at upper end of d and work back.
{
   int i,j,k;
   unsigned char out;
   int outdeg=n-p;
   quotientdegree=outdeg;
   state = 0;
	j = n + p;
	k = outdeg;
	//cout << "outdeg = " << outdeg<< endl;
	for(i = n; i >= 0; i--)
	{	
		j--;
		out = step(d[i]);
		if(i <= (n - p))
		{
			//cout << "n-p" << (n-p) <<endl;
			q[k--] = out;
		}
	}	
	//step(0);
	remainderdegree = j;

   // Fill in the blanks ...
   return state;
}

/*
Local Variables:
compile-command: "gcc -c -g BinPolyDiv.cc"
End:
*/
