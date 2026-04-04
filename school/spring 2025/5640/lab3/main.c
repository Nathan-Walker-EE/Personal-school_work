#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 9

int main(void)
{
	float ref_coeffs[N];
	float nu_coeffs[N];
	float alpha_m[N];
	float alpha_m_[N-1];
	float C_m[N];
	float C_m_[N-1];
	
	float b[] = {0.0003, 0, -0.0012, 0, 0.0018, 0, -0.0012, 0, 0.0003};
	float a[] = {1.0000, -4.6448, 11.6303, -18.7119, 21.2052, -17.0418, 9.6459, -3.5069, 0.6882};
	float k = 0.1888404;
	
	
	for(int i = 0; i < N; i++)
	{
		alpha_m[i] = a[i];
		C_m[i] = b[i];
	}
	
	for(int m = N-1; m > 0; m--)
	{
		nu_coeffs[m] = C_m[m];
		ref_coeffs[m] = alpha_m[m];
		for(int i = 0; i < m; i++)
		{
			alpha_m_[i] = 0;
			C_m_[i] = 0;
		}
		alpha_m_[0] = 1;
		C_m_[0] = 1;
		for(int k = 1; k < m+1; k++)
		{
			alpha_m_[k] = alpha_m[k] - alpha_m[m] * alpha_m[m - k];
			alpha_m_[k] = alpha_m_[k]/(1 - (alpha_m[m]*alpha_m[m]));
		}
		for(int k = 0; k < m+1;k++)
		{
			C_m_[k] = C_m[k] - nu_coeffs[m] * alpha_m[m - k];
		}
		for(int i = m - 1; i >= 0; i--)
		{
			alpha_m[i] = alpha_m_[i];
			C_m[i] = C_m_[i];
			printf("%f ", C_m_[i]);
		}
		printf("\n");
	}
	nu_coeffs[0] = C_m[0];
	
	for(int i = 0; i < N; i++)
	{
		printf("ref: %f, nu: %f\n", ref_coeffs[i], nu_coeffs[i]);
	}
	printf("\n\n");
	
	float G_buf1[] = {0, 0, 0, 0};
	float G_buf2[] = {0, 0, 0, 0};
	float x[] = {1,0,0,0,0,0,0,0,0,0};
	int buf_idx = 0;
	float f = 0;
	
	for(int i = 0; i < 10; i++)
	{
		float curr = x[i];
		f = curr;
		if((buf_idx&1) == 0)
			G_buf1[0] = curr;
		else
			G_buf2[0] = curr;
		for(int n = 1; n < N+1; n++)
		{
			if((buf_idx&1) == 0)
				G_buf1[n] = ref_coeffs[n]*f + G_buf2[n-1];
			else
				G_buf2[n] = ref_coeffs[n]*f + G_buf1[n-1];
			if((buf_idx&1) == 0)
				f = ref_coeffs[n]*G_buf2[n-1] + f;
			else
				f = ref_coeffs[n]*G_buf1[n-1] + f;
		}
		buf_idx++;
		printf("x[%d] = %f, f = %f\n", i, x[i], f);
	}
	
	return(0);
}

		/*F[0] = x[i];
		for(int j = 1; j < N + 1; j++)
		{
			F[j] = F[j-1] + (ref_coeffs[j+1] * G_buf[j]);
		}
		for(int j = N; j > 0; j--)
		{
			G_buf[j] = F[j] * ref_coeffs[j] + G_buf[j-1];
		}
		G_buf[0] = x[i];*/