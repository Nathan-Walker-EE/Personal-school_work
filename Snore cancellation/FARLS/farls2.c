/* Jake Gunther */
/* March 2004 */

/* modified by T.K. Moon, 29 Dec. 2006 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// to run:  farls2 mic_sp.dat spk_sp.dat ec_sp2.dat

#define DOUBLEACCUM


typedef float FLOAT;
#define HYPOT hypotf

//typedef double FLOAT;
//#define HYPOT hypot

//typedef long double FLOAT;
//#define HYPOT hypotl

// choose one of the following options:
//#define HOUSE //do the householder transformation (otherwise, do just Givens)
#define GIVENS // do the givens rotation
// #define BOTH   // do both householder and Givens
// #define TRIPLE   // householder - givens - householder

#define SIGN(x) (x >= 0 ? 1 : -1)
#ifndef MIN
#define MIN(a,b)  (a <= b ? a : b)
#endif

#define M 400        /* Filter order */

FLOAT lamrt;
FLOAT lamrtM;
FLOAT etart;

#define BUFF_SIZE 8192

#define START 0
#define STEP 1
unsigned long cnt1 = 0;
unsigned long cnt2 = STEP;
unsigned long cnt3 = 0;

void init_A(FLOAT A[][3]);
void reinit_A(FLOAT A[][3]);
void givens(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s);
void givens_mixed(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s);
int hyper(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s);
int hyper_mixed(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s);
int hyper_OD(FLOAT a, FLOAT b, FLOAT *c, FLOAT *s);
void hyper_OD_fix(FLOAT* a, FLOAT* b);
void apply_circ_rot(FLOAT* a, FLOAT* b, FLOAT c, FLOAT s);
void apply_circ_mixed_rot(FLOAT* a, FLOAT* b, FLOAT c, FLOAT s);
void apply_hyper_rot(FLOAT* a, FLOAT* b, FLOAT ch, FLOAT sh);
void apply_hyper_mixed_rot(FLOAT* a, FLOAT* b, FLOAT ch, FLOAT sh);
void apply_hyper_OD(FLOAT* a, FLOAT* b, FLOAT c, FLOAT s);

int hyperhyper(FLOAT g, FLOAT b, FLOAT* w, FLOAT *x, FLOAT *y, FLOAT *z);
void apply_hyperhyper_rot(FLOAT* g, FLOAT* b, FLOAT w, FLOAT x, FLOAT y, FLOAT z);
FLOAT hyperhouse_parm(FLOAT a0,FLOAT a1,FLOAT a2,FLOAT theta[][3], long int tt,
    int *which, FLOAT *g0, FLOAT *g1, FLOAT *g2, FLOAT *diff);
void applyhouse(FLOAT *a, FLOAT *b, FLOAT *c, FLOAT theta[][3]);
void negtheta(FLOAT theta[][3],FLOAT A[][3]);
void fixA(FLOAT A[][3], FLOAT *xs, FLOAT *cf);
void fixApost(int j,FLOAT A[][3], FLOAT Jen1);
void solvequad(FLOAT a, FLOAT b, FLOAT c, FLOAT *r1, FLOAT *r2);


int main(int argc, char *argv[])
{
    FILE* fd; /* = fopen("mic.dat","rb"); */
    FILE* fx; /* = fopen("spk.dat","rb"); */
    FILE* fe; /* = fopen("ec.dat","wb"); */
	/* all these files are size 'float', not 'double' */
    FILE* fm0 = fopen("mon0.dat","wb");  /* A[0][0] (post) */
    FILE* fm1 = fopen("mon1.dat","wb");  /* A[0][1] (post) */
    FILE* fm2 = fopen("mon2.dat","wb");  /* A[0][2] (post) */
	FILE* fm3 = fopen("mon3.dat","wb");  /* integer file */
	FILE* fm4 = fopen("mon4.dat","wb");  /* g0 or ch */
	FILE* fm5 = fopen("mon5.dat","wb");  /* g1 or sh */
	FILE* fm6 = fopen("mon6.dat","wb");  /* g2 or A[M+1][0] (post) */
	FILE* fm7 = fopen("mon7.dat","wb");  /* A[0][0] (prior) */
	FILE* fm8 = fopen("mon8.dat","wb");  /* A[0][1] (prior) */
	FILE* fm9 = fopen("mon9.dat","wb");  /* A[0][2] (prior) */
	FILE* fm10 = fopen("mon10.dat","wb");  /* diff or xs */
	FILE* fm11 = fopen("mon11.dat","wb");  /* correction factor */
	FILE* fm12 = fopen("mon12.dat","wb");  /* Jen0 - Jen0_2 */
	FILE* fm13 = fopen("mon13.dat","wb");  /* JenM1 - JenM1_2 */
	FILE* fm14 = fopen("mon14.dat","wb");  /* average Jeni - Jeni_2 */
	float x;
	int isnanflag = 0;

	FLOAT g0, g1, g2;
	FLOAT a0,a1,a2;
	FLOAT a0save,a1save,a2save;
	int which;
	int which2;
   
   /* I/O buffers and variables */
   float dbuff[BUFF_SIZE];
   float xbuff[BUFF_SIZE];
   float ebuff[BUFF_SIZE];
   int cnt=0;
   FLOAT diff;
   
   /* Filter variables */
   FLOAT A[M+2][3]; /* Pre & post array */
   FLOAT u[M+1];
   FLOAT w[M]; /* Filter taps */
   FLOAT s, c; /* Circular rotation parameters */
   FLOAT sh, ch; /* Hyperbolic rotation parameters */
   FLOAT e_new=0.0;    /* A priori prediction error */
   FLOAT e_old=0.0;
   FLOAT g_old=0.0;
   FLOAT d;    /* Desired signal */
   int j, t;
   long int totalt = 0;
   FLOAT housetheta[3][3];		/* householder transformation */
   FLOAT housetheta2[3][3];
   FLOAT givtheta[3][3],delta[3]; /* the actual theta matrix for Givens */
   FLOAT d1,e,f,c1,den, aM0save,aM1save,aM2save;
   FLOAT g02,g12,g22,diff2;
   FLOAT t1,t2,t3;
   /* Filter parameters */
   FLOAT lam = 0.99995;   // 1.0;
   FLOAT lamrtinv;
   FLOAT eta = 1.0;
   FLOAT alpha;
   FLOAT xs, cf;
   FLOAT Jen1, Jen2, Jen3;
   FLOAT Jen0, Jen0_2, JenM1, JenM1_2, Jendifavg;
   int i;
#ifdef DOUBLEACCUM
   double A01, A02;
#endif
   lamrt = sqrt(lam);
   etart = sqrt(eta);
   lamrtM = pow(lamrt,(FLOAT)M);
   lamrtinv = 1.0/lamrt;
   

   /* Process command line args */
   if(argc != 4) {
	  fprintf(stderr,"USAGE: %s <mic_in.dat> <spk_in.dat> <ec_out.dat>\n",argv[0]);
	  return 0;
   }
   if((fd = fopen(argv[1],"rb")) == NULL) {
	  fprintf(stderr,"Can't open microphone input file: %s\n",argv[1]);
	  return 0;
   }
   if((fx = fopen(argv[2],"rb")) == NULL) {
	  fprintf(stderr,"Can't open speaker input file: %s\n",argv[2]);
	  return 0;
   }
   if((fe = fopen(argv[3],"wb")) == NULL) {
	  fprintf(stderr,"Can't open echo canceled output file: %s\n",argv[3]);
	  return 0;
   }

   /* Initialize I/O buffers */
   memset(dbuff,'\0',sizeof(dbuff[0]));
   memset(xbuff,'\0',sizeof(dbuff[0]));
   memset(ebuff,'\0',sizeof(dbuff[0]));
   
   init_A(A); /* Initialize filter variables */
   memset(u,'\0',sizeof(u[0])*(M+1)); /* Initialize tap input vector */
   memset(w,'\0',sizeof(w[0])*M);     /* Initialize filter weights */

#ifdef HOUSE
   printf("householder transformation form: lambda=%g\n",lam);
#endif
#ifdef BOTH
   printf("both householder and givens rotations: lambda=%g\n",lam);
#endif
#ifdef GIVENS
   printf("Givens rotations forms: lambda=%g\n",lam);
#endif
#ifdef TRIPLE
   printf("Triple: lambda=%g\n",lam);
#endif

   /* Assume same amount read in from both files */
   cnt = fread(dbuff, sizeof(dbuff[0]), BUFF_SIZE, fd);
   cnt = fread(xbuff, sizeof(xbuff[0]), BUFF_SIZE, fx);
   totalt = 0;
   while(cnt && !isnanflag) {
// printf("new buffer read\n");
      for(t=0; t<cnt; t++) {
		 totalt++;
		 /* Input a pair of new samples */
		 d = dbuff[t];
		 u[0] = xbuff[t];
		 
		 /* Initialize for this iteration */
		 A[0][1] += u[0]*A[1][1];
		 A[0][2] += u[0]*A[1][2];

		 // fixA(A,&xs,&cf);		/* try to fix A */
		 g_old = A[M+1][0]; /* Save this value in the old kalman gain vector */
		 /* Compute rotations and apply them to the array */
		 /* Compute the a priori error during this pass */

		 a0 = A[0][0];  a1 = A[0][1];  a2 = A[0][2];
		 a0save = a0; a1save = a1; a2save = a2;

#ifdef HOUSE
		 alpha = hyperhouse_parm(A[0][0],A[0][1],A[0][2],housetheta,totalt,
								 &which,&g0, &g1, &g2,&diff);
		 applyhouse(&(A[0][0]),&(A[0][1]),&(A[0][2]),housetheta);
		 if(A[0][0] < 0) negtheta(housetheta,A);

		 applyhouse(&(A[M+1][0]),&(A[M+1][1]),&(A[M+1][2]),housetheta);

		 x = A[0][0]; fwrite(&x,sizeof(float),1,fm0);
		 x = A[0][1]; fwrite(&x,sizeof(float),1,fm1);
		 x = A[0][2]; fwrite(&x,sizeof(float),1,fm2);
		 fwrite(&which,sizeof(int),1,fm3);
		 x = g0;  fwrite(&x,sizeof(float),1,fm4);
		 x = g1; fwrite(&x,sizeof(float),1,fm5);
		 x = g2; fwrite(&x,sizeof(float),1,fm6);
		 x = a0save; fwrite(&x,sizeof(float),1,fm7);
		 x = a1save; fwrite(&x,sizeof(float),1,fm8);
		 x = a2save; fwrite(&x,sizeof(float),1,fm9);
		 x = diff; fwrite(&x,sizeof(float),1,fm10);
		 x = cf; fwrite(&x,sizeof(float),1,fm11);

#endif

#ifdef BOTH

		 givens_mixed(A[0][0], A[0][1], &c, &s);
		 apply_circ_mixed_rot( &(A[0][0]),&(A[0][1]), c,  s);
		 apply_circ_mixed_rot( &(A[M+1][0]),&(A[M+1][1]), c,  s);

		 hyper_OD( A[0][0],A[0][2], &ch, &sh);
		 apply_hyper_OD(&(A[0][0]),&(A[0][2]), ch, sh);
		 apply_hyper_OD(&(A[M+1][0]),&(A[M+1][2]), ch, sh);

		 alpha = hyperhouse_parm(A[0][0],A[0][1],A[0][2],housetheta,totalt,
								 &which,&g0, &g1, &g2,&diff);
		 applyhouse(&(A[0][0]),&(A[0][1]),&(A[0][2]),housetheta);
		 if(isnan(A[0][0]) || isnan(A[0][1]) || isnan(A[0][2])) {
			printf("NaN encountered at t=%ld\n",totalt);
			isnanflag = 1;
			break;
		 }
		 if(A[0][0] < 0) negtheta(housetheta,A);
		 applyhouse(&(A[M+1][0]),&(A[M+1][1]),&(A[M+1][2]),housetheta);


		 x = A[0][0]; fwrite(&x,sizeof(float),1,fm0);
		 x = A[0][1]; fwrite(&x,sizeof(float),1,fm1);
		 x = A[0][2]; fwrite(&x,sizeof(float),1,fm2);
		 fwrite(&which,sizeof(int),1,fm3);
		 x = g0;  fwrite(&x,sizeof(float),1,fm4);
		 x = g1; fwrite(&x,sizeof(float),1,fm5);
		 x = g2; fwrite(&x,sizeof(float),1,fm6);
		 x = a0save; fwrite(&x,sizeof(float),1,fm7);
		 x = a1save; fwrite(&x,sizeof(float),1,fm8);
		 x = a2save; fwrite(&x,sizeof(float),1,fm9);
		 x = diff; fwrite(&x,sizeof(float),1,fm10);
		 x = cf; fwrite(&x,sizeof(float),1,fm11);

#endif

#ifdef TRIPLE
		 alpha = hyperhouse_parm(A[0][0],A[0][1],A[0][2],housetheta,totalt,
								 &which,&g0, &g1, &g2,&diff);
		 applyhouse(&(A[0][0]),&(A[0][1]),&(A[0][2]),housetheta);
		 if(isnan(A[0][0]) || isnan(A[0][1]) || isnan(A[0][2])) {
			printf("NaN encountered at t=%ld\n",totalt);
			isnanflag = 1;
			break;
		 }
		 if(A[0][0] < 0) negtheta(housetheta,A);
		 applyhouse(&(A[M+1][0]),&(A[M+1][1]),&(A[M+1][2]),housetheta);

		 givens_mixed(A[0][0], A[0][1], &c, &s);
		 apply_circ_mixed_rot( &(A[0][0]),&(A[0][1]), c,  s);
		 apply_circ_mixed_rot( &(A[M+1][0]),&(A[M+1][1]), c,  s);

		 hyper_OD( A[0][0],A[0][2], &ch, &sh);
		 apply_hyper_OD(&(A[0][0]),&(A[0][2]), ch, sh);
		 apply_hyper_OD(&(A[M+1][0]),&(A[M+1][2]), ch, sh);

		 alpha = hyperhouse_parm(A[0][0],A[0][1],A[0][2],housetheta2,totalt,
								 &which2,&g02, &g12, &g22,&diff2);
		 applyhouse(&(A[0][0]),&(A[0][1]),&(A[0][2]),housetheta2);
		 if(isnan(A[0][0]) || isnan(A[0][1]) || isnan(A[0][2])) {
			printf("NaN encountered at t=%ld\n",totalt);
			isnanflag = 1;
			break;
		 }
		 if(A[0][0] < 0) negtheta(housetheta2,A);
		 applyhouse(&(A[M+1][0]),&(A[M+1][1]),&(A[M+1][2]),housetheta2);

		 x = A[0][0]; fwrite(&x,sizeof(float),1,fm0);
		 x = A[0][1]; fwrite(&x,sizeof(float),1,fm1);
		 x = A[0][2]; fwrite(&x,sizeof(float),1,fm2);
		 fwrite(&which,sizeof(int),1,fm3);
		 x = g0;  fwrite(&x,sizeof(float),1,fm4);
		 x = g1; fwrite(&x,sizeof(float),1,fm5);
		 x = g2; fwrite(&x,sizeof(float),1,fm6);
		 x = a0save; fwrite(&x,sizeof(float),1,fm7);
		 x = a1save; fwrite(&x,sizeof(float),1,fm8);
		 x = a2save; fwrite(&x,sizeof(float),1,fm9);
		 x = diff; fwrite(&x,sizeof(float),1,fm10);
		 x = cf; fwrite(&x,sizeof(float),1,fm11);

#endif


#ifdef GIVENS
		 Jen0 = A[0][0]*A[0][0] + A[0][1]*A[0][1] - A[0][2]*A[0][2];

		 givens_mixed(A[0][0], A[0][1], &c, &s);
		 apply_circ_mixed_rot( &(A[0][0]),&(A[0][1]), c,  s);  /* (opt.) */
		 a0save = A[0][0];		/* save after the first rotation */
		 a1save = A[0][1];
		 a2save = A[0][2];

		 hyper_OD( A[0][0],A[0][2], &ch, &sh);
		 apply_hyper_OD(&(A[0][0]),&(A[0][2]), ch, sh);
		 Jen0_2 = A[0][0]*A[0][0] + A[0][1]*A[0][1] - A[0][2]*A[0][2];

/* 		 /\* build an explicit representation of theta *\/ */
/* 		 givtheta[0][0] = 1; givtheta[0][1] = 0; givtheta[0][2] = 0; */
/* 		 apply_circ_mixed_rot(&(givtheta[0][0]),&(givtheta[0][1]),c,s); */
/* 		 apply_hyper_OD(&(givtheta[0][0]),&(givtheta[0][2]),ch,sh); */
/* 		 givtheta[1][0] = 0; givtheta[1][1] = 1; givtheta[1][2] = 0; */
/* 		 apply_circ_mixed_rot(&(givtheta[1][0]),&(givtheta[1][1]),c,s); */
/* 		 apply_hyper_OD(&(givtheta[1][0]),&(givtheta[1][2]),ch,sh); */
/* 		 givtheta[2][0] = 0; givtheta[2][1] = 0; givtheta[2][2] = 1; */
/* 		 apply_circ_mixed_rot(&(givtheta[2][0]),&(givtheta[2][1]),c,s); */
/* 		 apply_hyper_OD(&(givtheta[2][0]),&(givtheta[2][2]),ch,sh); */
/* //		 printf("[%g %g %g\n%g %g %g\n%g %g %g]\n", */
/* //				givtheta[0][0],givtheta[0][1], givtheta[0][2], */
/* //				givtheta[1][0],givtheta[1][1], givtheta[1][2], */
/* //				givtheta[2][0],givtheta[2][1], givtheta[2][2]); */

/* 		 d1 = A[M+1][0];  e = A[M+1][1];  f = A[M+1][2]; */
/* 		 c1 = givtheta[0][0]*d1 + givtheta[1][0]*e + givtheta[2][0]*f; */
/* 		 den = givtheta[0][0]*givtheta[0][0] + givtheta[1][0]*givtheta[1][0]  */
/* 			+ givtheta[2][0]*givtheta[2][0]; */
/* 		 delta[0] = -c1*givtheta[0][0]/den; */
/* 		 delta[1] = -c1*givtheta[1][0]/den; */
/* 		 delta[2] = -c1*givtheta[2][0]/den; */

/* 		 aM0save = A[M+1][0]; aM1save = A[M+1][1];  aM2save = A[M+1][2]; */
/* 		 for(i = M+1; i < M+2; i++) { */
/* 			A[i][0] += delta[0]; */
/* 			A[i][1] += delta[1]; */
/* 			A[i][2] += delta[2]; */
/* 		 } */
/* 		 Jen1 = A[M+1][0]*A[M+1][0]+A[M+1][1]*A[M+1][1]-A[M+1][2]*A[M+1][2]; */

		 if(isnan(A[0][0]) || isnan(A[0][1]) || isnan(A[0][2])) {
			printf("NaN encountered at t=%ld\n",totalt);
			isnanflag = 1;
			break;
		 }

/* 		 /\* see how the correction did *\/ */
/* 		 apply_circ_mixed_rot( &(aM0save),&(aM1save), c,  s); */
/* 		 apply_hyper_OD(&(aM0save),&(aM2save), ch, sh); */
		 


		 JenM1 = A[M+1][M+1]*A[0][0] + A[M+1][1]*A[M+1][1] 
			- A[M+1][2]*A[M+1][2];

		 apply_circ_mixed_rot( &(A[M+1][0]),&(A[M+1][1]), c,  s);
		 apply_hyper_OD(&(A[M+1][0]),&(A[M+1][2]), ch, sh);

		 JenM1_2 = A[M+1][0]*A[M+1][0] + A[M+1][1]*A[M+1][1] 
			- A[M+1][2]*A[M+1][2];

//printf("Jen0=%g  Jen0_2=%g  dif0=%g\nJenM1=%g JenM1_2=%g  difM1=%g\n",
//			Jen0,Jen0_2,Jen0-Jen0_2,JenM1,JenM1_2,JenM1-JenM1_2);

/* 		 Jen2 = A[M+1][0]*A[M+1][0]+A[M+1][1]*A[M+1][1]-A[M+1][2]*A[M+1][2]; */

/* printf("delta=[%g   %g   %g]    z(unmod)=%g    z=%g   diff=%g\n",delta[0],delta[1],delta[2],aM0save,A[M+1][0],aM0save-A[M+1][0]); */
/* printf("Jen1=%g  Jen2=%g  diff=%g\n",Jen1,Jen2,Jen1-Jen2); */

		 x = A[0][0];  fwrite(&x,sizeof(float),1,fm0);
		 x = A[0][1]; fwrite(&x,sizeof(float),1,fm1);
		 x = A[0][2]; fwrite(&x,sizeof(float),1,fm2);
		 x = ch; fwrite(&x,sizeof(float),1,fm4);
		 x = sh; fwrite(&x,sizeof(float),1,fm5);
		 x = A[M+1][0]; fwrite(&x,sizeof(float),1,fm6);
		 x = a0save; fwrite(&x,sizeof(float),1,fm7);
		 x = a1save; fwrite(&x,sizeof(float),1,fm8);
		 x = a2save; fwrite(&x,sizeof(float),1,fm9);
		 x = xs;  fwrite(&x,sizeof(float),1,fm10);
		 x = cf; fwrite(&x,sizeof(float),1,fm11);
		 x = Jen0 - Jen0_2; fwrite(&x,sizeof(float),1,fm12);
		 x = JenM1 - JenM1_2; fwrite(&x,sizeof(float),1,fm13);

#endif
		 /* Complete last row, and then work from the bottom up in the
            loop */
		 A[M+1][1] *= lamrtinv;
		 A[M+1][2] *= lamrtinv;

		 /* Initialize other variables */
		 e_new = d;

		 // A[0][0] = alpha;
//		 A[0][1] = 0.0; /* This should already be zero */
//		 A[0][2] = 0.0; /* This should already be zero */
// if(A[0][0] < 1) A[0][0] = 1;
// A[M+1][0] = 0;
#ifdef DOUBLEACCUM
	     A01 = 0;
		 A02 = 0;
#endif
		 Jendifavg = 0;
		 for(j=M; j>0; j--)	 {
			w[j-1] += g_old*e_old;  
			e_new -= u[j-1]*w[j-1]; /* Compute a priori error */
	     
			g_old = A[j][0]; /* Save value in the old kalman gain vector */

#ifdef HOUSE
			applyhouse(&(A[j][0]),&(A[j][1]),&(A[j][2]),housetheta);
#endif
#ifdef BOTH
			Jen1 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];

			apply_circ_mixed_rot( &(A[j][0]),&(A[j][1]), c,  s);
			apply_hyper_OD(&(A[j][0]),&(A[j][2]), ch, sh);
			applyhouse(&(A[j][0]),&(A[j][1]),&(A[j][2]),housetheta);

			Jen2 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
			if(Jen1 != Jen2) {
			   // fixApost(j,A,Jen1);
			}

			Jen3 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
/* if(Jen1 != Jen2) { */
/*  printf("Jen1=%g  Jen2=%g  Jen3=%g  dif(%d)=%g  dif=%g\n", */
/* 		Jen1,Jen2,Jen3,j,Jen1-Jen2,Jen1-Jen3); */
/* } */
			Jendifavg += fabs(Jen1 - Jen2);

#endif
#ifdef TRIPLE
			applyhouse(&(A[j][0]),&(A[j][1]),&(A[j][2]),housetheta);

			apply_circ_mixed_rot( &(A[j][0]),&(A[j][1]), c,  s);
			apply_hyper_OD(&(A[j][0]),&(A[j][2]), ch, sh);

			applyhouse(&(A[j][0]),&(A[j][1]),&(A[j][2]),housetheta2);

#endif

#ifdef GIVENS
			/* Apply the circular and hyperbolic rotations */
			Jen1 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
			apply_circ_mixed_rot( &(A[j][0]),&(A[j][1]), c,  s);
			apply_hyper_OD(&(A[j][0]),&(A[j][2]), ch, sh);
			Jen2 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
			if(Jen1 != Jen2) {
			   // fixApost(j,A,Jen1);
			}
			Jen3 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
/* if(Jen1 != Jen2) { */
/*  printf("Jen1=%g  Jen2=%g  Jen3=%g  dif(%d)=%g  dif=%g\n", */
/* 		Jen1,Jen2,Jen3,j,Jen1-Jen2,Jen1-Jen3); */
/* } */
			Jendifavg += fabs(Jen1 - Jen2);
#endif
			A[j][1] *= lamrtinv;
			A[j][2] *= lamrtinv;

			/* Shift the input data vector */
			u[j]=u[j-1];
			/* Accumulate the product [u(i) u_{i-1]]*L_{i-1} */
#ifdef DOUBLEACCUM
	        A01 += u[j]*A[j+1][1];
			A02 += u[j]*A[j+1][2];
#else
			A[0][1] += u[j]*A[j+1][1];
			A[0][2] += u[j]*A[j+1][2];
#endif		
			A[j+1][0] = A[j][0]; /* Shift kalman gain g */
		 }
#ifdef DOUBLEACCUM
	     A[0][1] = A01;
		 A[0][2] = A02;
#endif
		 Jendifavg /= M;
		 x =  Jendifavg; fwrite(&x,sizeof(float),1,fm14);


		 e_old = e_new/A[0][0];
		 A[1][0] = 0.0;

		 /* Save posterior filter error */
		 ebuff[t] = e_old/A[0][0];

	  } // for t
      fwrite(ebuff, sizeof(ebuff[0]), cnt, fe);

      cnt = fread(dbuff, sizeof(dbuff[0]), BUFF_SIZE, fd);
      cnt = fread(xbuff, sizeof(xbuff[0]), BUFF_SIZE, fx);
   }
   printf("totalt=%ld\n",totalt);
   fclose(fd); fclose(fx); fclose(fe); /* fclose(fm); */

   fclose(fm0);  fclose(fm1);  fclose(fm2);
   fclose(fm3);
   fclose(fm4); fclose(fm5); fclose(fm6);
   fclose(fm7); fclose(fm8); fclose(fm9); fclose(fm10); fclose(fm11);
   fclose(fm12);  fclose(fm13); fclose(fm14);
   return 0;
}

void fixA(FLOAT A[][3], FLOAT *xs, FLOAT *cf)
{
   int i;
   FLOAT p1,p2;
   FLOAT eta;


   *xs = A[0][0]*A[M+1][0] + A[0][1]*A[M+1][0] - A[0][2]*A[M+1][2];
   if(*xs != 0) {
	  i = 1;
   }

#if 0
   // method 1: scale the 3rd column
   p1 = A[0][2]*A[M+1][2];
   if(p1) {
	  eta = sqrt(*xs/p1 + 1);
*cf = eta;
	  for(i = 0; i < M+2; i++) {
		 A[i][2] *= eta;
	  }
   }
   else {
*cf = eta;
   }
#endif 

#if 0
   // Method 2: scale the first column
   p1 = A[0][0] * A[M+1][0];
   if(p1) {
	  eta = sqrt(1 - *xs/p1);
*cf = p1;
      for(i = 0; i < M+2; i++) {
		 A[i][0] *= eta;
	  }
   }
   else {
*cf = p1;
   }
#endif

#if 1
   // Method 2: scale the first column
   p1 = A[0][2] * A[M+1][2] - A[0][1]*A[M+1][1];
   p2 = A[0][0] * A[M+1][0];
   if(p1*p2 < 0) {
	  i = 1;
   }
   if(p1) {
	  eta = p2/p1;
*cf = eta;
      for(i = 1; i < M+2; i++) {
		 A[i][1] *= eta;
		 A[i][2] *= eta;
	  }
   }
   else {
*cf = 0;
   }
#endif


}


void fixApost(int j,FLOAT A[][3], FLOAT Jen1)
{
	FLOAT Jen2, Delta, delta, b, c, D, r1, r2, Dsqr;
	FLOAT g[5];					/* coefficients of polynomial */
	FLOAT p, p1;				/* polynomial and derivative (for Newton) */
	FLOAT lambda,oolambda, oolambda2;
	int nNewt, i;
	FLOAT Jen2Jen1, Jen4, Jen4Jen1;
	Jen2 = A[j][0]*A[j][0] + A[j][1]*A[j][1] - A[j][2]*A[j][2];
	Delta = Jen1 - Jen2;
	FLOAT delta3[3];
	if(!Delta) return;  // nothing to do!

#if 0
	// fix column 0
	b = 2*A[j][0];
	c = -Delta;
	D = b*b - 4*c;
	if(D >= 0) {				/* we can do this */
	   Dsqr = sqrt(D);
	   r1 = -b - Dsqr;
	   r2 = -b + Dsqr;
	   if(fabs(r1) < fabs(r2)) {
		  delta = r1/2;
	   }
	   else {
		  delta = r2/2;
	   }
	   A[j][0] += delta;
	}
	else {
	   printf("negative discriminant on row %d; column 0 fix\n",j);
	}
#endif

#if 0
	// fix column 1
	b = 2*A[j][1];
	c = -Delta;
	D = b*b - 4*c;
	if(D >= 0) {				/* we can do this */
	   Dsqr = sqrt(D);
	   r1 = -b - Dsqr;
	   r2 = -b + Dsqr;
	   if(fabs(r1) < fabs(r2)) { // choosing the other way breaks quickly
		  delta = r1/2;
	   }
	   else {
		  delta = r2/2;
	   }
	   A[j][1] += delta;
	}
	else {
	   printf("negative discriminant on row %d; column 1 fix\n",j);
	}
#endif

#if 0
	// fix column 2
	b = -2*A[j][2];
	c = Delta;
	D = b*b - 4*c;
	if(D >= 0) {				/* we can do this */
	   Dsqr = sqrt(D);
	   r1 = -b - Dsqr;
	   r2 = -b + Dsqr;
	   if(fabs(r1) < fabs(r2)) {
		  delta = r1/2;
	   }
	   else {
		  delta = r2/2;
	   }
	   A[j][2] += delta;
	}
	else {
	   printf("negative discriminant on row %d; column 2 fix\n",j);
	}
#endif

#if 0  // fix the whole row
	Jen2Jen1 = Jen2/Jen1;
	g[0] = 1 - Jen2Jen1;
	g[1] = -2*(A[j][0]*A[j][0] + A[j][1]*A[j][1] + A[j][2]*A[j][2])/Jen1;
	g[2] = -2 - Jen2Jen1;
	g[3] = 0;
	g[4] = 1;

	lambda = 0;					/* initial Lagrange multiplier */
	for(nNewt = 0; nNewt < 5; nNewt++){
	   p = g[4]*lambda + g[3];
	   p1 = g[4];
	   for(i=2; i >= 0; i--) {
		  p1 = p + p1*lambda;
		  p = g[i] + p * lambda;
	   }
	   if(p1 == 0) {
		  printf("Error: 0 derivative in Newton-Raphson\n");
	   }
	   lambda -= p/p1;
	}
	// printf("lambda=%g   ",lambda);
	oolambda = lambda/(1-lambda);
	oolambda2 = -lambda/(1+lambda);
	delta3[0] = oolambda*A[j][0];
	delta3[1] = oolambda*A[j][1];
	delta3[2] = oolambda2*A[j][2];
	A[j][0] += delta3[0];
	A[j][1] += delta3[1];
	A[j][2] += delta3[2];
	
#endif

#if 0  // fix first two elements on the row
	g[0] = Jen1 - (A[j][0]*A[j][0] + A[j][1]*A[j][1]);
	g[1] = -2*Jen1;
	g[2] = Jen1;
	solvequad(g[2],g[1],g[0],&r1,&r2);
	lambda = MIN(fabs(r1),fabs(r2));

	// printf("lambda=%g   ",lambda);
	oolambda = lambda/(1-lambda);
	delta3[0] = oolambda*A[j][0];
	delta3[1] = oolambda*A[j][1];
	A[j][0] += delta3[0];
	A[j][1] += delta3[1];
#endif


#if 1  // fix second and third elements  [1,2]

    // Jen2Jen1 = Jen2/Jen1;
	Jen4 = A[j][1]*A[j][1] - A[j][2]*A[j][2];
	Jen4Jen1 = Jen4/Jen1;
	g[0] = 1 - Jen4Jen1;   // Jen1 - Jen4;
	g[1] = -2*(A[j][1]*A[j][1] + A[j][2]*A[j][2])/Jen1;  // !! tkm
	// g[1] = -2*(A[j][0]*A[j][0] + A[j][2]*A[j][2]);  // !! tkm
	g[2] = -2 - Jen4Jen1;  // -2*Jen1 - Jen4;
	g[3] = 0;
	g[4] = 1; // Jen1;

	lambda = 0;					/* initial Lagrange multiplier */
	for(nNewt = 0; nNewt < 2; nNewt++){
	   p = g[4]*lambda + g[3];
	   p1 = g[4];
	   for(i=2; i >= 0; i--) {
		  p1 = p + p1*lambda;
		  p = g[i] + p * lambda;
	   }
	   if(p1 == 0) {
		  printf("Error: 0 derivative in Newton-Raphson\n");
	   }
	   lambda -= p/p1;
	}
	printf("lambda=%g   ",lambda);
	oolambda = lambda/(1-lambda);
	oolambda2 = -lambda/(1+lambda);
	delta3[0] = oolambda*A[j][0];
	delta3[2] = oolambda2*A[j][2];
	A[j][0] += delta3[0];
	A[j][2] += delta3[2];
	
#endif

#if 0  // fix first and third elements

    // Jen2Jen1 = Jen2/Jen1;
	Jen4 = A[j][0]*A[j][0] - A[j][2]*A[j][2];
	g[0] = Jen1 - Jen4;
	g[1] = -2*(A[j][0]*A[j][0] + A[j][2]*A[j][2]);
	g[2] = -2*Jen1 - Jen4;
	g[3] = 0;
	g[4] = Jen1;

	lambda = 0;					/* initial Lagrange multiplier */
	for(nNewt = 0; nNewt < 5; nNewt++){
	   p = g[4]*lambda + g[3];
	   p1 = g[4];
	   for(i=2; i >= 0; i--) {
		  p1 = p + p1*lambda;
		  p = g[i] + p * lambda;
	   }
	   if(p1 == 0) {
		  printf("Error: 0 derivative in Newton-Raphson\n");
	   }
	   lambda -= p/p1;
	}
	// printf("lambda=%g   ",lambda);
	oolambda = lambda/(1-lambda);
	oolambda2 = -lambda/(1+lambda);
	delta3[0] = oolambda*A[j][0];
	delta3[2] = oolambda2*A[j][2];
	A[j][0] += delta3[0];
	A[j][2] += delta3[2];
	
#endif


	   
	
}
	   
	

void solvequad(FLOAT a, FLOAT b, FLOAT c, FLOAT *r1, FLOAT *r2)
{
   FLOAT d,q;

   d = b*b - 4*a*c;
   if(d < 0) {
	  printf("negative discriminant\n");
	  *r1 = 0;
	  *r2 = 0;
	  return;
   }
   q = -0.5*(b + SIGN(b)*sqrt(d));
   *r1 = q/a;
   *r2 = c/q;
}

void init_A(FLOAT A[][3])
{
   int i,j;

   /* Initialize filter */
   for(i=0; i<M+2; i++)
   {
      for(j=0; j<3; j++)
      {
         A[i][j] = 0.0;
      }
   }
   /*memset(A,'\0',(M+2)*3*sizeof(A[0][0]));*/
   A[0][0] = 1; /* Gamma */
//   A[0][0] = 1; /* Gamma */
   A[1][1] = etart*lamrt;            /* L initialization */
   A[M+1][2] = etart*lamrt*lamrtM;   /* L initialization */
}
void reinit_A(FLOAT A[][3])
{
   int i,j;

   /* Initialize filter */
   for(i=0; i<M+2; i++)
   {
      for(j=1; j<3; j++)
      {
         A[i][j] = 0.0;
      }
   }
   /*memset(A,'\0',(M+2)*3*sizeof(A[0][0]));*/
   A[0][0] = 1.01; /* Gamma */
   A[1][1] = etart*lamrt;            /* L initialization */
   A[M+1][2] = etart*lamrt*lamrtM;   /* L initialization */
}

void apply_circ_rot(FLOAT* a, FLOAT* b, FLOAT c, FLOAT s)
{
   FLOAT tmp = c*(*a) + s*(*b);
   *b = s*(*a) - c*(*b);
   /* *b = c*(*b) - s*(*a); */
   *a = tmp;
}

void apply_hyper_rot(FLOAT* a, FLOAT* b, FLOAT ch, FLOAT sh)
{
   FLOAT tmp = ch*(*a) - sh*(*b);
   *b = ch*(*b) - sh*(*a);
   *a = tmp;
}

void givens(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s)
{
   FLOAT rho = 0.0;
   if(b == 0.0)
   {
      *c = 1.0;
      *s = 0.0;
   }
   else if(a == 0.0)
   {
      *c = 0.0;
      *s = 1.0;
   }
   else
   {
      rho = b/a;
      *c = 1.0/sqrt(1.0+rho*rho);;
      *s = rho*(*c);
   }
}

void givens_mixed(FLOAT a, FLOAT b, FLOAT* rho, FLOAT* rootoneplusrhosquared)
{
   if(b == 0.0)
   {
      *rho = 0.0;
      *rootoneplusrhosquared = 1.0;
   }
   else if(a == 0.0)
   {
       fprintf(stderr,"Problem with givens rotation\n");
   }
   else
   {
      if(a < FLT_EPSILON*3.0)
      {
         fprintf(stderr,"Dividing by small number %20.10f\n",a);
      }
      *rho = b/a;
      *rootoneplusrhosquared = sqrt(1.0+(*rho)*(*rho));
   }
}

void apply_circ_mixed_rot(FLOAT* a, FLOAT* b, FLOAT rho, FLOAT rootoneplusrhosquared)
{
    *a = ((*a) + rho*(*b))/rootoneplusrhosquared;
    *b = rootoneplusrhosquared*(*b) - rho*(*a);
}


int hyperhyper(FLOAT g, FLOAT b, FLOAT* w, FLOAT* x, FLOAT* y, FLOAT* z)
{
    int flag = 0; // flag = 0

    FLOAT s0 = g*g;
    FLOAT s1 = b*b;
    FLOAT t0 = s0 + s1;
    FLOAT t1 = 1.01;
    FLOAT t2 = s0 - s1;
    FLOAT gp, bp, rho;

    if(t2 < t1)
    {
	flag = 1;
	// Endpoint of circular rotation
	gp = sqrt(0.5*(t0+t1));
	bp = sqrt(0.5*(t0-t1));

	// Compute circular rotation
	*w = (g*gp + b*bp)/t0;
	*x = (b*gp - g*bp)/t0;
    }
    else
    {
	gp = g;
	bp = b;
	*w = 1.0;
	*x = 0.0;
    }

    // Compute hyperbolic rotation
    rho = bp/gp;
    *y = 1.0/sqrt(1.0-rho*rho);
    *z = -rho*(*y);
    
    return flag;
}

void apply_hyperhyper_rot(FLOAT* g, FLOAT* b, FLOAT w, FLOAT x, FLOAT y, FLOAT z)
{
    // This function rotates toward the x-axis
    FLOAT s, t;

    // Circular rotation part
#if 1
    s = (*g) * (*b);
    if(fabs(s)>0.0)
    {
	s /= fabs(s);
	x *= s;
    }
#endif
    s = w*(*g)+x*(*b);
    t = w*(*b)-x*(*g);
    
    // Hyperbolic rotation part
    *g = s*y + t*z;
    *b = s*z + t*y;
}

int hyper(FLOAT a, FLOAT b, FLOAT* c, FLOAT* s)
{
   int flag = 1;
   FLOAT rho = 0.0;
   if(b == 0.0)
   {
      *c = 1.0;
      *s = 0.0;
   }
   else if(fabs(b) < fabs(a))
   {
      rho = b/a;
      *c = 1.0/sqrt(1.0-rho*rho);
      *s = rho*(*c);
      /*fprintf(stderr,"%f < %f\n",b,a);*/
      flag = 0;
   }
   else if(fabs(b) > fabs(a))
   {
      rho = a/b;
      *c = 1.0/sqrt(1.0-rho*rho);
      *s = rho*(*c);
   }
   return flag;
}

int hyper_mixed(FLOAT a, FLOAT b, FLOAT* rho, FLOAT* rootoneminusrhosquared)
{
   int flag = 1;
   if(b == 0.0)
   {
      *rho = 0.0;
      *rootoneminusrhosquared = 1.0;
   }
   else if(fabs(b) < fabs(a))
   {
      *rho = b/a;
      *rootoneminusrhosquared = sqrt(1.0 - (*rho)*(*rho));
      /*fprintf(stderr,"%f < %f\n",b,a);*/
      flag = 0;
   }
   else if(fabs(b) > fabs(a))
   {
      *rho = a/b;
      *rootoneminusrhosquared = sqrt(1.0 - (*rho)*(*rho));
   }
   return flag;
}

void apply_hyper_mixed_rot(FLOAT* a, FLOAT* b, FLOAT rho, FLOAT rootoneminusrhosquared)
{ /* Mixed downdating form I, Jojanczyk, et al., 1987 */
    *a = ((*a) - rho*(*b))/rootoneminusrhosquared;
    *b = rootoneminusrhosquared*(*b) - rho*(*a);
}

void hyper_OD_fix(FLOAT* a, FLOAT* b)
{
    /* Fix any problems */
    if(fabs(*b) > fabs(*a))
    {
	if((*a)*(*b) > 0.0)
	{
	    /* Same sign */
	    *a = (*b)*(1.0 + 3.0*FLT_EPSILON);
	}
	else
	{
	    /* Opposite signs */
	    *a = -(*b)*(1.0 + 3.0*FLT_EPSILON);
	}
    }
/*    if(*a < 1.0)
    {
	*a = 1.0;
    }*/
}

int hyper_OD(FLOAT a, FLOAT b, FLOAT *c, FLOAT *s)
{
    int flag = 1;

    if(a-b == 0.0)
    {
       fprintf(stderr,"Divide by zero %f,%f\n",a,b);
    }
	if(a-b < 0) {
	   fprintf(stderr,"Negative argument of square root\n");
	}
    *c = sqrt((a+b)/(a-b))*0.5;
    *s = sqrt((a-b)/(a+b))*0.5;

    return flag;
}

void apply_hyper_OD(FLOAT* a, FLOAT* b, FLOAT c, FLOAT s)
{
    FLOAT x, y;
    x = (*a)-(*b);
    y = (*a)+(*b);
    x *= c;
    y *= s;
    (*a) = y+x;
    (*b) = y-x;
}

FLOAT hyperhouse_parm(FLOAT a0,FLOAT a1,FLOAT a2,FLOAT theta[][3],long int tt,
					 int *which, FLOAT *g0o, FLOAT *g1o, FLOAT *g2o, FLOAT *diff)
{
   FLOAT g0,g1,g2;
   FLOAT as,bs;
   FLOAT alpha;
   FLOAT g00,g01,g02,g11,g12,g22;
   FLOAT g01sqrt, g02sqrt, g12sqrt;
   FLOAT den, den2;
   FLOAT thetat[3][3];
   FLOAT t20, t20_2, t02_2, t21, t21_2, t12_2, t10, t10_2, t01, t01_2;
   FLOAT s102, s201, s120, s021, s012, s012_, dennorm1, dennorm2;

   int usenew = 0;  /* use the newer (more complicated) expressions */

   as = HYPOT(a0,a1);    /* sqrt(a0*a0 + a1*a1); */

   *diff = as-fabs(a2);

   if(fabs(a2) > as) {
	  alpha = sqrt(as+fabs(a2))*sqrt(fabs(a2)-as);
	  g0 = a0;
	  g1 = a1;
	  g2 = a2 - alpha*SIGN(a2);

	  *g0o = g0;  *g1o = g1;  *g2o = g2;

	  g00 = g0*g0;
	  g01 = g0*g1;
	  g02 = g0*g2;
	  g11 = g1*g1;
	  g12 = g1*g2;
	  g22 = g2*g2;

	  g01sqrt = HYPOT(g0,g1);
	  g12sqrt = HYPOT(g1,g2);
	  g02sqrt = HYPOT(g0,g2);

	  den = 1/((g01sqrt+g2)*(g01sqrt-g2));
	  den2 = 2*den;

	  t20 = g2/g0;  t20_2 = t20*t20;  t02_2 = 1/t20_2;
	  t21 = g2/g1;  t21_2 = t21*t21;  t12_2 = 1/t21_2;
	  t01 = g0/g1;
	  t10 = g1/g0;  t10_2 = t10*t10;  t01_2 = 1/t10_2;
	  s102 = g1/g02sqrt;
	  s201 = g2/g01sqrt;
	  s021 = g02sqrt/g1;
	  s012 = g01sqrt/g2;
	  s012_ = g0/g12sqrt;
	  s120 = g12sqrt/g0;
	  dennorm1 = (1+s201)*(1-s201);
	  dennorm2 = (s012+1)*(s012-1);
	  if(g2==0) {
		 thetat[0][0] = 0;
		 thetat[0][1] = 2*g01/(g00+g11);
		 thetat[0][2] = -(g1+g0)*(g1-g0)/(g00 + g11);
		 thetat[1][0] = 0;
		 thetat[1][1] = thetat[0][0];
		 thetat[2][0] = -1;
		 *which = -1;
	  }
	  else if(fabs(g2) < g01sqrt) {  // pull out the g01sqrt factor
		 thetat[0][0] = 2*g2/((g0 + t10*g1)*dennorm1);
		 thetat[0][1] = 2/((t01+t10)*dennorm1);
	     thetat[0][2] = -(1+t20_2)/(1+t10_2)  * (s102 + 1)*(s102-1)/dennorm1;
		 // (g1+g02sqrt)*(g1-g02sqrt)*den;  //1-g00*gJg;
		 thetat[1][0] = 2*g2/((g1 + t01*g0)*dennorm1);
		 thetat[1][1] = -(1+s120)*(1-s120)/((1+t10_2)*dennorm1);
		 thetat[2][0] = -(1 + g22/(g00+g11))/dennorm1;
		 *which = -2;
	  }
	  else { // g2 > g01sqrt
		 thetat[0][0] = 2/(t20*dennorm2);  //   -g02*den2;
		 thetat[0][1] = 2/(t10*t20*dennorm2); // -g01*den2;
		 thetat[0][2] = -(t02_2+1)*(s102+1)*(s102-1)/dennorm2;
		 thetat[1][0] = 2/(t21*dennorm2);   // -g12*den2;
		 thetat[1][1] = -(t12_2+1)*(s012_+1)*(s012_ -1)/dennorm2;
	     thetat[2][0] = -(t02_2 + t12_2 + 1)/dennorm2;   // (g00+g11+g22)*den;    // 1+g22*gJg;
		 *which = -3;
	  }

	  thetat[1][2] = thetat[0][1];
	  thetat[2][2] = -thetat[0][0];  // g02*den2;
	  thetat[2][1] = -thetat[1][0];  // g12*den2;



	  /* gJg = 2/(g00 + g11 - g22); */
	  theta[0][0] = g02*den2;
	  theta[0][1] = g01*den2;
	  theta[0][2] = (g02sqrt+g1)*(g02sqrt-g1)*den;   // -(1-g00*gJg);

	  theta[1][0] = g12*den2;
	  theta[1][1] = (g12sqrt+g0)*(g12sqrt-g0)*den;   // -(1-g11*gJg);
	  theta[1][2] = g01*den2;
	  
	  theta[2][0] = -(g00 + g11 + g22)*den;         // -(1+g22*gJg);
	  theta[2][1] = -g12*den2;
	  theta[2][2] = -g02*den2;

/* printf("totalt=%ld\n",tt); */
/* printf("%g (%g)  %g (%g) %g (%g)\n",thetat[0][0],theta[0][0], */
/* 	   thetat[0][1],theta[0][1],thetat[0][2],theta[0][2]); */
/* printf("%g (%g)  %g (%g) %g (%g)\n",thetat[1][0],theta[1][0], */
/* 	   thetat[1][1],theta[1][1],thetat[1][2],theta[1][2]); */
/* printf("%g (%g)  %g (%g) %g (%g)\n",thetat[2][0],theta[2][0], */
/* 	   thetat[2][1],theta[2][1],thetat[2][2],theta[2][2]); */


   }
   else {
	  FLOAT t;
	  double t0,t1,t2,asd,alphad;

	  // t0 = a0; t1 = a1; t2 = a2;  asd = hypot(a0,a1); 
	  // alphad = as*sqrt(1+a2/as)*sqrt(1-a2/as); alpha = (float)alphad;

	  // t = a2/as;  alpha = as*sqrt(1 - t*t);
	  // alpha = as*sqrt(1+a2/as)*sqrt(1-a2/as);
	  alpha = sqrt(as + a2)*sqrt(as-a2);
	  if(alpha < 1) alpha = 1.00001;
	  // alpha = sqrt(a0*a0 + a1*a1 - a2*a2);

	  g0 = a0 + alpha*SIGN(a0);
	  g1 = a1;
	  g2 = a2;


	  *g0o = g0;  *g1o = g1;  *g2o = g2;

	  g00 = g0*g0;
	  g01 = g0*g1;
	  g02 = g0*g2;
	  g11 = g1*g1;
	  g12 = g1*g2;
	  g22 = g2*g2;
	  g01sqrt = HYPOT(g0,g1);  // sqrt(g0^2 + g1^2);
	  g02sqrt = HYPOT(g0,g2);  // sqrt(g0^2 + g2^2);
	  g12sqrt = HYPOT(g1,g2);  // sqrt(g1^2 + g2^2);
	  den = 1/((g01sqrt+g2)*(g01sqrt-g2));
	  den2 = 2*den;
	  /* gJg = 2/(g00 + g11 - g22); */
	  t20 = g2/g0;  t20_2 = t20*t20;  t02_2 = 1/t20_2;
	  t21 = g2/g1;  t21_2 = t21*t21;  t12_2 = 1/t21_2;
	  t01 = g0/g1;
	  t10 = g1/g0;  t10_2 = t10*t10;  t01_2 = 1/t10_2;
	  s102 = g1/g02sqrt;
	  s201 = g2/g01sqrt;
	  s021 = g02sqrt/g1;
	  s012 = g01sqrt/g2;
	  s012_ = g0/g12sqrt;
	  s120 = g12sqrt/g0;
	  dennorm1 = (1+s201)*(1-s201);
	  dennorm2 = (s012+1)*(s012-1);
	  if(g2==0) {
		 thetat[0][0] = (g1+g0)*(g1-g0)/(g00 + g11);
		 thetat[0][1] = -2*g01/(g00+g11);
		 thetat[0][2] = 0;
		 thetat[1][1] = -thetat[0][0];
		 thetat[1][2] = 0;
		 thetat[2][2] = 1;
		 *which = 0;
	  }
	  else if(fabs(g2) < g01sqrt) {  // pull out the g01sqrt factor
	     thetat[0][0] = (1+t20_2)/(1+t10_2)  * (s102 + 1)*(s102-1)/dennorm1;
		 // (g1+g02sqrt)*(g1-g02sqrt)*den;  //1-g00*gJg;
		 thetat[0][1] = -2/((t01+t10)*dennorm1);
		 thetat[0][2] = -2*g2/((g0 + t10*g1)*dennorm1);
		 thetat[1][1] = (1+s120)*(1-s120)/((1+t10_2)*dennorm1);
		 thetat[1][2] = -2*g2/((g1 + t01*g0)*dennorm1);
		 thetat[2][2] = (1 + g22/(g00+g11))/dennorm1;
		 *which = 1;
	  }
	  else { // g2 > g01sqrt
		 thetat[0][0] = (t02_2+1)*(s102+1)*(s102-1)/dennorm2;
		 thetat[0][1] = -2/(t10*t20*dennorm2); // -g01*den2;
		 thetat[0][2] = -2/(t20*dennorm2);  //   -g02*den2;
		 thetat[1][1] = (t12_2+1)*(s012_+1)*(s012_ -1)/dennorm2;
		 thetat[1][2] = -2/(t21*dennorm2);   // -g12*den2;
	     thetat[2][2] = (t02_2 + t12_2 + 1)/dennorm2;   // (g00+g11+g22)*den;    // 1+g22*gJg;
		 *which = 2;
	  }

	  thetat[1][0] = thetat[0][1];
	  thetat[2][0] = -thetat[0][2];  // g02*den2;
	  thetat[2][1] = -thetat[1][2];  // g12*den2;


	  theta[0][0] = (g1+g02sqrt)*(g1-g02sqrt)*den;  //1-g00*gJg;
	  theta[0][1] = -g01*den2;
	  theta[0][2] = -g02*den2;

	  theta[1][0] = -g01*den2;
	  theta[1][1] = (g0+g12sqrt)*(g0-g12sqrt)*den;   // 1-g11*gJg;
	  theta[1][2] = -g12*den2;
	  
	  theta[2][0] = g02*den2;
	  theta[2][1] = g12*den2;
	  theta[2][2] = (g00+g11+g22)*den;    // 1+g22*gJg;

   }

   if(usenew) {
theta[0][0] = thetat[0][0];  theta[0][1] = thetat[0][1]; theta[0][2] = thetat[0][2];
theta[1][0] = thetat[1][0];  theta[1][1] = thetat[1][1]; theta[1][2] = thetat[1][2];
theta[2][0] = thetat[2][0];  theta[2][1] = thetat[2][1]; theta[2][2] = thetat[2][2];

   }
   return fabs(alpha);
}


void
applyhouse(FLOAT *a, FLOAT *b, FLOAT *c, FLOAT theta[][3])
{
   FLOAT a0,b0,c0;
   a0 = *a;
   b0 = *b;
   c0 = *c;
   
   *a = a0*theta[0][0] + b0*theta[1][0] + c0*theta[2][0];
   *b = a0*theta[0][1] + b0*theta[1][1] + c0*theta[2][1];
   *c = a0*theta[0][2] + b0*theta[1][2] + c0*theta[2][2];
}


void
negtheta(FLOAT theta[][3],FLOAT A[][3])
{
   int i,j;
   for(i = 0; i < 3; i++) {
	  for(j = 0; j < 3; j++) {
		 theta[i][j] = -theta[i][j];
	  }
   }
   A[0][0] = -A[0][0];
   A[0][1] = -A[0][1];
   A[0][2] = -A[0][2];
}


/* 		 if(fabs(A[0][1]) > 0.1e-6) { /\* see if we can do something better *\/ */
/* 			FLOAT scalef = 0.01; */
/* 			printf("attempting to improve rotation: t=%ld  (1)a1=%g\n",totalt,A[0][1]); */
/* 			a0 = scalef*a0save; a1 = scalef*a1save; a2 = scalef*a2save; */
/* 			hyperhouse_parm(a0,a1,a2,housetheta,totalt,&which, */
/* 						 &g0, &g1, &g2, &diff); */
/* 			applyhouse(&a0,&a1,&a2,housetheta); */
/* 			printf("new (%g) a1=%g\n",scalef,a1/scalef); */
/*  			A[0][0] = a0/scalef;  A[0][1] = a1/scalef;  A[0][2] = a2/scalef; */
/* 		 } */

/*
Local Variables:
compile-command:"gcc -o farls2 -g farls2.c  -lm"
End:
*/
