#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 9

int main(void)
{
	float ref_coeffs[N+1];
	float nu_coeffs[N+1];
	float alpha_m[N+1];
	float alpha_m_[N];
	//float C_m[N+1];
	//float C_m_[N];
	
	float b[] = {0.0003, 0, -0.0012, 0, 0.0018, 0, -0.0012, 0, 0.0003};
	float a[] = {};
	float k = 0.1888404;
	
	
	for(int i = 0; i < N; i++)
	{
		alpha_m[i+1] = a[i];
		//C_m[i+1] = b[i];
	}
	alpha_m[0] = 1;
	//C_m[0] = 1;
	
	for(int m = N+1; m > 0; m--)
	{
		//nu_coeffs[m] = C_m[m];
		ref_coeffs[m] = alpha_m[m];
		for(int i = 0; i < m; i++)
		{
			alpha_m_[i] = 0;
			//C_m_[i] = 0;
		}
		alpha_m_[0] = 1;
		//C_m_[0] = 1;
		for(int k = 1; k <= m; k++)
		{
			alpha_m_[k] = alpha_m[k] - alpha_m[m] * alpha_m[m - k];
			alpha_m_[k] = alpha_m_[k]/(1 - (alpha_m[m]*alpha_m[m]));
			//C_m_[k] = C_m[k] - nu_coeffs[m] * alpha_m[m - k];
		}
		for(int i = m - 1; i >= 0; i--)
		{
			alpha_m[i] = alpha_m_[i];
			//C_m[i] = C_m_[i];
			//printf("%f ", C_m[i]);
		}
		//printf("\n");
	}
	
	for(int i = 0; i < N+1; i++)
	{
		printf("ref: %f, nu: %f\n", ref_coeffs[i], nu_coeffs[i]);
	}
	printf("\n\n");
	
	return(0);
}
