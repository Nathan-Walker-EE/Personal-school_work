#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPS 1e-12   // tolerance for singular detection
#define MAX_LINE 1024

void matrix_transpose(float** matrix, float** T_matrix, int row, int col);
float scalar_kalman(float y, float xtt, float *Ptt, float A, float C, 
float Q, float R);
float average(float* data, int n);
float compute_std(float* data, int n);
void test_matrix_transpose_inverse(void);
void test_matrix_arithmetic(void);
int matrix_inverse(float **A, float **Ainv, int N);
int matrix_multiply(float **A, int row_A, int col_A, float **B, 
int row_B, int col_B, float** C, int row_C, int col_C);
int matrix_addition(float **A, int row_A, int col_A, float **B, 
int row_B, int col_B, float** C, int row_C, int col_C);
float* matrix_kalman
	(float* y, int row_y, int col_y,
	float* xtt, int row_xtt, int col_xtt,
	float** Ptt, int row_Ptt, int col_Ptt,
	float** A, int row_A, int col_A,
	float** C, int row_C, int col_C,
	float** Q, int row_Q, int col_Q,
	float** R, int row_R, int col_R);
	
float* matrix_kalman
	(float* y, int row_y, int col_y,
	float* xtt, int row_xtt, int col_xtt,
	float** Ptt, int row_Ptt, int col_Ptt,
	float** A, int row_A, int col_A,
	float** C, int row_C, int col_C,
	float** Q, int row_Q, int col_Q,
	float** R, int row_R, int col_R)
{	
	float** C_transpose = (float**)malloc(sizeof(float*)*col_C);
	if(C_transpose == NULL){
		return(0);
	}
	for(int i = 0; i < col_C; i++){
		C_transpose[i] = (float*)malloc(sizeof(float)*row_C);
		if(C_transpose[i] == NULL){
			return(0);
		}
	}
	
	float** temp1 = (float**)malloc(sizeof(float*)*row_Ptt);
	if(temp1 == NULL){
		return(0);
	}
	for(int i = 0; i < row_Ptt; i++){
		temp1[i] = (float*)malloc(sizeof(float)*row_C);
		if(temp1[i] == NULL){
			return(0);
		}
	}
	
	float** temp2 = (float**)malloc(sizeof(float*)*row_C);
	if(temp2 == NULL){
		return(0);
	}
	for(int i = 0; i < row_C; i++){
		temp2[i] = (float*)malloc(sizeof(float)*row_C);
		if(temp2[i] == NULL){
			return(0);
		}
	}
	
}

int main(void){
	test_matrix_transpose_inverse();
	test_matrix_arithmetic();
	
	
	
	return(0);
}

int matrix_addition(float **A, int row_A, int col_A, float **B, 
int row_B, int col_B, float** C, int row_C, int col_C){
	if(row_A != row_B || row_A != row_C 
	|| col_A != col_B || col_A != col_C)return(0);
	
	for(int i = 0; i < row_A; i++){
		for(int j = 0; j < col_A; j++){
			C[i][j] = A[i][j]+B[i][j];
		}
	}
}


void test_matrix_arithmetic(void){
	printf("\n\nTesting matrix arithmetic\n\n");
	int row = 2;
	int col = 2;
	float** matrix = (float**)malloc(sizeof(float*)*row);
	if(matrix == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	for(int i = 0; i < row; i++){
		matrix[i] = (float*)malloc(sizeof(float)*col);
		if(matrix[i] == NULL){
			printf("MALLOC failed!!\n");
			exit(1);
		}
	}
	
	float** T_matrix = (float**)malloc(sizeof(float*)*col);
	if(T_matrix == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	for(int i = 0; i < col; i++){
		T_matrix[i] = (float*)malloc(sizeof(float)*row);
		if(T_matrix[i] == NULL){
			printf("MALLOC failed!!\n");
			exit(1);
		}
	}
	
	float** C_matrix = (float**)malloc(sizeof(float*)*row);
	if(C_matrix == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	for(int i = 0; i < row; i++){
		C_matrix[i] = (float*)malloc(sizeof(float)*row);
		if(C_matrix[i] == NULL){
			printf("MALLOC failed!!\n");
			exit(1);
		}
	}
	
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			matrix[i][j] = (float)(rand()&0xf);
			printf(" %.0f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	
	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			T_matrix[i][j] = (float)(rand()&0xf);
			printf(" %.0f ", T_matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	matrix_multiply(matrix, row, col, T_matrix, col, row,
	C_matrix, row, row);
	printf("\nmatrix multiply\n");
	for(int i = 0; i < row; i++){
		for(int j = 0; j < row; j++){
			printf(" %.0f ", C_matrix[i][j]);
		}
		printf("\n\n");
	}
	printf("\n\n");
	
	matrix_addition(matrix, row, col, T_matrix, col, row,
	C_matrix, row, row);
	printf("\nmatrix addition\n");
	for(int i = 0; i < row; i++){
		for(int j = 0; j < row; j++){
			printf(" %.0f ", C_matrix[i][j]);
		}
		printf("\n\n");
	}
	printf("\n\n");
	
}

int matrix_multiply(float **A, int row_A, int col_A,
                    float **B, int row_B, int col_B,
                    float **C, int row_C, int col_C)
{
    // Dimension checks
    if (col_A != row_B || row_C != row_A || col_C != col_B)
        return 0;   // invalid dimensions

    for (int i = 0; i < row_A; i++)
    {
        for (int j = 0; j < col_B; j++)
        {
            float sum = 0.0f;

            for (int k = 0; k < col_A; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }

    return 1;   // success
}

void matrix_transpose(float** matrix, float** T_matrix, int row, int col){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			T_matrix[j][i] = matrix[i][j];
		}
	}
}


float scalar_kalman(float y, float xtt, float *Ptt, float A, float C, 
float Q, float R){
	float Kt = (*Ptt)*C/((C * (*Ptt) * C) + R);
	float xhatt = xtt + (Kt *(y - (C*xtt)));
	float Pt = (1.0 - (Kt*C))*(*Ptt);
	float xhat = A*xhatt;
	float P = (A*Pt*A) + Q;
	*Ptt = P;
	return (xhat);
}


float average(float* data, int n){
	//int n = sizeof(data)/sizeof(data[0]);
	float sum = 0;
	for(int i = 0; i < n;i++){
		sum += data[i];
	}
	sum /= n;
	return(sum);
}


float compute_std(float* data, int n){
	float mean = average(data, n);
	
	double sqDiff = 0;
	//int n = sizeof(data)/sizeof(data[0]);
   for (int i = 0; i < n; i++) 
      sqDiff += (data[i] - mean) * (data[i] - mean);
   
	return sqrt((sqDiff / n));
}

void test_matrix_transpose_inverse(void){
	int row = 2;
	int col = 2;
	float** matrix = (float**)malloc(sizeof(float*)*row);
	if(matrix == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	for(int i = 0; i < row; i++){
		matrix[i] = (float*)malloc(sizeof(float)*col);
		if(matrix[i] == NULL){
			printf("MALLOC failed!!\n");
			exit(1);
		}
	}
	
	float** T_matrix = (float**)malloc(sizeof(float*)*col);
	if(T_matrix == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	for(int i = 0; i < col; i++){
		T_matrix[i] = (float*)malloc(sizeof(float)*row);
		if(T_matrix[i] == NULL){
			printf("MALLOC failed!!\n");
			exit(1);
		}
	}
	
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			matrix[i][j] = (float)(rand()&0xf);
			printf(" %f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	
	matrix_transpose(matrix, T_matrix, row, col);
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			printf(" %f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			printf(" %f ", T_matrix[i][j]);
		}
		printf("\n");
	}
	printf("\nTesting Matrix inversion\n");
	matrix_inverse(T_matrix, matrix, 2);
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			printf(" %f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			printf(" %f ", T_matrix[i][j]);
		}
		printf("\n");
	}	
}

int matrix_inverse(float **A, float **Ainv, int N)
{
    int i, j, k;

    // Create augmented matrix [A | I]
    double **aug = malloc(N * sizeof(double *));
    for(i = 0; i < N; i++)
        aug[i] = malloc(2 * N * sizeof(double));

    // Fill augmented matrix
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            aug[i][j] = A[i][j];

        for(j = 0; j < N; j++)
            aug[i][j + N] = (i == j) ? 1.0 : 0.0;
    }

    // Gauss-Jordan elimination
    for(i = 0; i < N; i++)
    {
        // Find pivot
        double pivot = aug[i][i];
        if(fabs(pivot) < EPS)
        {
            // Matrix is singular
            return 0;
        }

        // Normalize pivot row
        for(j = 0; j < 2 * N; j++)
            aug[i][j] /= pivot;

        // Eliminate other rows
        for(k = 0; k < N; k++)
        {
            if(k == i) continue;

            double factor = aug[k][i];
            for(j = 0; j < 2 * N; j++)
                aug[k][j] -= factor * aug[i][j];
        }
    }

    // Extract inverse from augmented matrix
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            Ainv[i][j] = aug[i][j + N];

    // Free memory
    for(i = 0; i < N; i++)
        free(aug[i]);
    free(aug);

    return 1;   // success
}
//scalar main function
/*FILE* fp = fopen("x_data.txt", "r");
	if(!fp){
		printf("Couldn't open the file\n");
		exit(1);
	}
	
	int n = 5304;
	char line[MAX_LINE];
	float ax_data[5304] = {0};
	int i = 0;
	
	while(fgets(line, MAX_LINE, fp)){
		int col = 0; 
		char *token = strtok(line, ", \t\n\r");
		
		while(token){
			float ax = atof(token);
			ax_data[i++] = atof(token);
			//printf("ax = %lf\n", ax);
			break;
		}
	}
	
	float xhat = ax_data[0];
	float P = 1;
	float A = 1;
	float C = 1;
	float sigma_meas = compute_std(ax_data, n);
	printf("sigma_meas = %f\n", sigma_meas);
	float R = sigma_meas * sigma_meas;
	printf("R = %f\n", R);
	float Q = 250.0;
	
	float* xest = (float*)malloc(sizeof(float)*n);
	if(xest == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	float* p_vec = (float*)malloc(sizeof(float)*n);
	if(p_vec == NULL){
		printf("MALLOC failed!!\n");
		exit(1);
	}
	p_vec[0] = P;
	xest[0] = xhat;
	
	for(i = 1; i < n; i++){
		//printf("ax = %lf\n", ax_data[i]);
		float y = ax_data[i];
		//float y = 1;
		xhat = scalar_kalman(y, xhat, &P, A, C, Q, R);
		p_vec[i] = P;
		xest[i] = xhat;
	}
	
	FILE* fo = fopen("estimated_state.txt", "w");
	if(!fo){
		printf("Couldn't open the file\n");
		exit(1);
	}
	
	FILE* fo2 = fopen("estimated_P.txt", "w");
	if(!fo){
		printf("Couldn't open the file\n");
		exit(1);
	}
	
	for(i = 0; i < n;i++){
		char buffer[20] = {0};
		sprintf(buffer, "%.5f", xest[i]);
		fprintf(fo, "%s\n", buffer);
		sprintf(buffer, "%.5f", p_vec[i]);
		fprintf(fo2, "%s\n", buffer);
	}
	
	fclose(fp);
	fclose(fo);
	fclose(fo2);
	free(xest);*/