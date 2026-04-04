#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

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
   
	return (sqDiff / n);
}

int main(void){
	FILE* fp = fopen("x_data.txt", "r");
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
	//float sigma_meas = compute_std(ax_data, n);
	float sigma_meas = 48.9307;
	printf("sigma_meas = %f", sigma_meas);
	float R = sigma_meas * sigma_meas;
	//float R =  1.0f;
	printf("R = %f", R);
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
	free(xest);
	
	return(0);
}