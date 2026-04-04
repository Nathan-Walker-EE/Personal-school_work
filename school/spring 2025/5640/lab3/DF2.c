#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 3

int main(void)
{
	float b[] = {1, 0, -1};
	float a[] = {1, -0.864336695, 0.94033489927};
	float k = 0.1888404;
	int samples = 20;
	float y[20] = {0};
	float d[3] = {0};
	
	float x[20] = {0};
	x[0] = 1;
	
	for(int i = 0; i < samples; i++)
	{
		d[0] = x[i] - a[1] * d[1] - a[2] * d[2];
		y[i] = b[0] * d[0] + b[1] * d[1] + b[2] * d[2];
		y[i] *= k;
		for(int j = 2; j > 0; j--)
		{
			d[j] = d[j-1];
		}
		printf("i=%d, %lf \n", i, y[i]);
	}
	
	return(0);
}
