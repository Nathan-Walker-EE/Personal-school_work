/* Jake Gunther */
/* March 2004 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define M 250        /* Filter order */

float lamrt;
float lamrtM;
float etart;

#define BUFF_SIZE 1

#define START 0
#define STEP 1
unsigned long cnt1 = 0;
unsigned long cnt2 = STEP;
unsigned long cnt3 = 0;

void init_A(float A[][3]);
void reinit_A(float A[][3]);
void givens(float a, float b, float* c, float* s);
void givens_mixed(float a, float b, float* c, float* s);
int hyper(float a, float b, float* c, float* s);
int hyper_mixed(float a, float b, float* c, float* s);
int hyper_OD(float a, float b, float *c, float *s);
void hyper_OD_fix(float* a, float* b);
void apply_circ_rot(float* a, float* b, float c, float s);
void apply_circ_mixed_rot(float* a, float* b, float c, float s);
void apply_hyper_rot(float* a, float* b, float ch, float sh);
void apply_hyper_mixed_rot(float* a, float* b, float ch, float sh);
void apply_hyper_OD(float* a, float* b, float c, float s);

int hyperhyper(float g, float b, float* w, float *x, float *y, float *z);
void apply_hyperhyper_rot(float* g, float* b, float w, float x, float y, float z);
#define Fs 8000
#define PI 3.1415

int main(int argc, char *argv[])
{
   FILE* fe; /* = fopen("ec.dat","wb"); */
   fe = fopen("rt_error.dat", "wb");
   
   int cnt=0;
   int num_samples = 10*Fs;
   srand(time(NULL));  // Seed the random number generator
   float ebuff[num_samples];
   
   /* Filter variables */
   float A[M+2][3]; /* Pre & post array */
   float u[M+1];
   float w[M]; /* Filter taps */
   float system[M];
   float system_buf[M];
   float s, c; /* Circular rotation parameters */
   float sh, ch; /* Hyperbolic rotation parameters */
   float e_new=0.0;    /* A priori prediction error */
   float e_old=0.0;
   float g_old=0.0;
   float d;    /* Desired signal */
   int j, t;

   /* Filter parameters */
   float lam = 1.0;
   float lamrtinv;
   float eta = 1.0;
   lamrt = sqrt(lam);
   etart = sqrt(eta);
   lamrtM = pow(lamrt,(float)M);
   lamrtinv = 1.0/lamrt;
   
   init_A(A); /* Initialize filter variables */
   memset(system, '\0', sizeof(system[0]*M));
   memset(system_buf, '\0', sizeof(system[0]*M));
   memset(u,'\0',sizeof(u[0])*(M+1)); /* Initialize tap input vector */
   memset(w,'\0',sizeof(w[0])*M);     /* Initialize filter weights */
   for(int k = 0; k < M; k++)
   {
	   system[k] = sin(2 * PI * .3642158 * k);
   }
   system[M/2] += 10;
   int i = 0;

   printf("opening the loop\n");
   while(i < num_samples)
   {
	   /* Input a pair of new samples */
	   float x = (2.0 * rand() / RAND_MAX) - 1.0;
	   d = 0;
	   for(int k = M-1; k > 0; k--)
	   {
			system_buf[k] = system_buf[k-1];
			d += system_buf[k] * system[k];
	   }
	   system_buf[0] = x;
	   d += system[0] * system_buf[0];
	   u[0] = x;

	   /* Initialize for this iteration */
	   A[0][1] += u[0]*A[1][1];
	   A[0][2] += u[0]*A[1][2];

	 g_old = A[M+1][0]; /* Save this value in the old kalman gain vector */
	 /* Compute rotations and apply them to the array */
	 /* Compute the a priori error during this pass */

	 givens_mixed(A[0][0], A[0][1], &c, &s);
	 apply_circ_mixed_rot( &(A[0][0]),&(A[0][1]), c,  s);
	 apply_circ_mixed_rot( &(A[M+1][0]),&(A[M+1][1]), c,  s);

	 //hyper_OD_fix(&A[0][0],&A[0][2]);
	 hyper_OD( A[0][0],A[0][2], &ch, &sh);
	 //hyper_OD_fix(&A[M+1][0],&A[M+1][2]);
	 //hyper_OD( A[M+1][0],A[M+1][2], &ch, &sh);
	 apply_hyper_OD(&(A[0][0]),&(A[0][2]), ch, sh);
	 apply_hyper_OD(&(A[M+1][0]),&(A[M+1][2]), ch, sh);

	 /* Complete last iteration and then work from the bottom up in the loop */
	 A[M+1][1] *= lamrtinv;
	 A[M+1][2] *= lamrtinv;

	 /* Initialize other variables */
	 e_new = d;
	 
	 A[0][1] = 0.0; /* This should already be zero */
	 A[0][2] = 0.0; /* This should already be zero */
	 //printf("starting adaptive loop\n");	
	 for(j=M; j>0; j--)
	 {
	     w[j-1] += g_old*e_old;  /* Update filter weights for previous iteration */
	     e_new -= u[j-1]*w[j-1]; /* Compute a priori error */
	     
	     /* Apply the circular and hyperbolic rotations */
	     g_old = A[j][0]; /* Save this value in the old kalman gain vector */
	     apply_circ_mixed_rot( &(A[j][0]),&(A[j][1]), c,  s);
	     apply_hyper_OD(&(A[j][0]),&(A[j][2]), ch, sh);

	     A[j][1] *= lamrtinv;
	     A[j][2] *= lamrtinv;
		 
	     /* Shift the input data vector */
	     u[j]=u[j-1];
	     /* Accumulate the product [u(i) u_{i-1]]*L_{i-1} */
	     A[0][1] += u[j]*A[j+1][1];
	     A[0][2] += u[j]*A[j+1][2];

	     A[j+1][0] = A[j][0]; /* Shift kalman gain g */
		 }
	 e_old = e_new/A[0][0];
	 A[1][0] = 0.0;

	 /* Save posterior filter error */
	 ebuff[i] = e_old/A[0][0];
     i++;
   }
   
   FILE* fh = fopen("filter.dat","wb");
   fwrite(w, sizeof(w[0]), M, fh);
   fwrite(ebuff, sizeof(ebuff[0]), num_samples, fe);
   
   //fclose(fd); fclose(fx); fclose(fe); fclose(fm); fclose(fh);
   
   return 0;
}

void init_A(float A[][3])
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
   A[0][0] = 1.01; /* Gamma */
   A[1][1] = etart*lamrt;            /* L initialization */
   A[M+1][2] = etart*lamrt*lamrtM;   /* L initialization */
}
void reinit_A(float A[][3])
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

void apply_circ_rot(float* a, float* b, float c, float s)
{
   float tmp = c*(*a) + s*(*b);
   *b = s*(*a) - c*(*b);
   /* *b = c*(*b) - s*(*a); */
   *a = tmp;
}

void apply_hyper_rot(float* a, float* b, float ch, float sh)
{
   float tmp = ch*(*a) - sh*(*b);
   *b = ch*(*b) - sh*(*a);
   *a = tmp;
}

void givens(float a, float b, float* c, float* s)
{
   float rho = 0.0;
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

void givens_mixed(float a, float b, float* rho, float* rootoneplusrhosquared)
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

void apply_circ_mixed_rot(float* a, float* b, float rho, float rootoneplusrhosquared)
{
    *a = ((*a) + rho*(*b))/rootoneplusrhosquared;
    *b = rootoneplusrhosquared*(*b) - rho*(*a);
}


int hyperhyper(float g, float b, float* w, float* x, float* y, float* z)
{
    int flag = 0; // flag = 0

    float s0 = g*g;
    float s1 = b*b;
    float t0 = s0 + s1;
    float t1 = 1.01;
    float t2 = s0 - s1;
    float gp, bp, rho;

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

void apply_hyperhyper_rot(float* g, float* b, float w, float x, float y, float z)
{
    // This function rotates toward the x-axis
    float s, t;

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

int hyper(float a, float b, float* c, float* s)
{
   int flag = 1;
   float rho = 0.0;
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

int hyper_mixed(float a, float b, float* rho, float* rootoneminusrhosquared)
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

void apply_hyper_mixed_rot(float* a, float* b, float rho, float rootoneminusrhosquared)
{ /* Mixed downdating form I, Jojanczyk, et al., 1987 */
    *a = ((*a) - rho*(*b))/rootoneminusrhosquared;
    *b = rootoneminusrhosquared*(*b) - rho*(*a);
}

void hyper_OD_fix(float* a, float* b)
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

int hyper_OD(float a, float b, float *c, float *s)
{
    int flag = 1;

    if(a-b == 0.0)
    {
       fprintf(stderr,"Divide by zero %f,%f\n",a,b);
    }
    *c = sqrt((a+b)/(a-b))*0.5;
    *s = sqrt((a-b)/(a+b))*0.5;

    return flag;
}

void apply_hyper_OD(float* a, float* b, float c, float s)
{
    float x, y;
    x = (*a)-(*b);
    y = (*a)+(*b);
    x *= c;
    y *= s;
    (*a) = y+x;
    (*b) = y-x;
}
