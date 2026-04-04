//  Program: BinLFSR.cc
//
//  Todd K. Moon
//
// Copyright 2019 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include <iostream>
using namespace std;

extern "C" {
#include <stdio.h>
}

#define PROB1

#include "BinLFSR.h"

int main()
{
	#ifdef PROB1
   unsigned int g = 0x17; // 10111 = 1+d+d^2+d^4
	#endif
	#ifdef PROB2
   int g = 0x13; // 10011 = 1+d+d^4
	#endif
   int Nsteps = 4;
   BinLFSR X(g,4,13);
   int i;
   char out;
   unsigned int state;
   /*
   for(i = 0; i < Nsteps; i++) {
	  out = X.step(state);
	  cout << out << " " << state << "\n";
   }*/
   char *outputs = new char[Nsteps + 1];
   X.steps(Nsteps, outputs);
   for(int i = 0; i < Nsteps; i++)
   {
		cout << "output[" << i << "] = " << static_cast<int>(outputs[i]) << " " <<endl;
   }
}


/*
Local Variables:
compile-command: "g++ -o testBinLFSR -g testBinLFSR.cc BinLFSR.cc"
End:
*/


