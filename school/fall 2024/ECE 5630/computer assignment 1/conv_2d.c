#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
typedef struct PGMImage {
    char pgmType[3];
    float** data;
    unsigned int width;
    unsigned int height;
    unsigned int maxValue;
} PGMImage;
 
void read_pgm(PGMImage* pgm, const char* filename)
{
    FILE* pgmfile = fopen(filename, "rb");
 
    if (pgmfile == NULL) {
        printf("File does not exist\n");
        return;
    }
 
    fscanf(pgmfile, "%s", pgm->pgmType);
 
    fscanf(pgmfile, "%d %d", &(pgm->width), &(pgm->height));
 
    fscanf(pgmfile, "%d", &(pgm->maxValue));
 
    pgm->data = malloc(pgm->height * sizeof(float*));
	unsigned char temp[pgm->width];
    for (int i = 0; i < pgm->height; i++) {
        pgm->data[i] = malloc(pgm->width * sizeof(float));
 
        if (pgm->data[i] == NULL) {
            fprintf(stderr, "malloc failed\n");
			exit(1);
        }
        fread(temp, sizeof(unsigned char), pgm->width, pgmfile);
		for(int j = 0; j < pgm->width; j++){
			pgm->data[i][j] = (float)temp[j];
		}
    }

    fclose(pgmfile);
}

void write_pgm(PGMImage* pgm, const char* filename)
{
	FILE* output_pgm;
	output_pgm = fopen(filename, "wb");
	
	fprintf(output_pgm, "P5\n");
	fprintf(output_pgm, "%d %d\n", pgm->width, pgm->height);
	fprintf(output_pgm, "%d \n", pgm->maxValue);
	for(int i = 0; i < pgm->height; i++){
		for(int j = 0; j < pgm->width; j++){
			unsigned char pixel = (unsigned char)(pgm->data[i][j]);
			fwrite(&pixel, sizeof(unsigned char), 1, output_pgm);
		}
	}
	fclose(output_pgm);
}

void conv_2d(float** image, int prow, int pcol,
 float** filter, int frow, int fcol,
 float** output_image, double scale)
{
	int k = prow + 2*(frow)-1;
	int l = pcol + 2*(fcol)-1;
	int m = prow;
	int n = pcol;
	
	unsigned char imagezp[k][l];
	//unsigned char output_image[m][n];
	
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			output_image[i][j] = 0;
		}
	}
	for(int i = 0; i < k; i++){
		for(int j = 0; j < l; j++){
			imagezp[i][j] = 0;
		}
	}	
	for(int i = 0; i < prow; i++){
		for(int j = 0; j < pcol; j++){
			imagezp[(k-prow-frow) + i-1][(l-pcol-fcol) + j-1] = image[i][j];
		}
	}
	float sum = 0;
	float temp = 0;
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			sum = 0;
			temp = 0;
			for(int x = 0; x < frow; x++){
				for(int y = 0; y < fcol; y++){
					temp += imagezp[i + x + frow/2 -1][j + y + fcol/2 - 1] * filter[x][y];
				}
			}
			sum = temp * scale;
			output_image[i][j] = sum;
		}
	}
}

int main(void)
{
    PGMImage* input_pgm = malloc(sizeof(PGMImage));
	PGMImage* output_pgm = malloc(sizeof(PGMImage));
	PGMImage* temp_pgm = malloc(sizeof(PGMImage));
	PGMImage* filter_pgm = malloc(sizeof(PGMImage));
    const char* ipfile;
	const char* ffile;

	ipfile = "image.pgm";
	ffile = "filter_final.pgm";
    read_pgm(input_pgm, ipfile);
	read_pgm(output_pgm, ipfile);
	read_pgm(temp_pgm, ipfile);
	read_pgm(filter_pgm, ffile);
	
	int H1rows = 5;
	int H1cols = 5;
	float** H1 = (float **)malloc( H1rows * sizeof(float *));
    for (int i = 0; i < H1rows; i++) {
        H1[i] = (float*)malloc(H1cols * sizeof(unsigned char));
    }
	H1[0][0] = 1; H1[0][1] = 2; H1[0][2] = 3; H1[0][3] = 2; H1[0][4] = 1;
	H1[1][0] = 2; H1[1][1] = 4; H1[1][2] = 6; H1[1][3] = 4; H1[1][4] = 2;
	H1[2][0] = 3; H1[2][1] = 6; H1[2][2] = 9; H1[2][3] = 6; H1[2][4] = 3;
	H1[3][0] = 2; H1[3][1] = 4; H1[3][2] = 6; H1[3][3] = 4; H1[3][4] = 2;
	H1[4][0] = 1; H1[4][1] = 2; H1[4][2] = 3; H1[4][3] = 2; H1[4][4] = 1;
	
	conv_2d(input_pgm->data, input_pgm->height, input_pgm->width,
	H1, H1rows, H1cols, output_pgm->data, .01234568);
	printf("finished convolution\n");
	write_pgm(output_pgm, "problem_2.pgm");
	
	int H21rows = 3;
	int H21cols = 3;
	float**H21 = (float**)malloc( H21rows * sizeof(float*));
    for (int i = 0; i < H21rows; i++) {
        H21[i] = (float*)malloc(H21cols * sizeof(float));
    }
	
	H21[0][0] = 1; H21[0][1] = 0; H21[0][2] = -1;
	H21[1][0] = 2; H21[1][1] = 0; H21[1][2] = -2;
	H21[2][0] = 1; H21[2][1] = 0; H21[2][2] = -1;
	
	int H22rows = 3;
	int H22cols = 3;
	float**H22 = (float**)malloc( H22rows * sizeof(float*));
    for (int i = 0; i < H22rows; i++) {
        H22[i] = (float *)malloc(H22cols * sizeof(float));
    }
	
	H22[0][0] = -1; H22[0][1] = -2; H22[0][2] = -1;
	H22[1][0] = 0; H22[1][1] = 0; H22[1][2] = 0;
	H22[2][0] = 1; H22[2][1] = 2; H22[2][2] = 1;
	
	conv_2d(input_pgm->data, input_pgm->height, input_pgm->width,
	H21, H21rows, H21cols, output_pgm->data, 1);
	
	conv_2d(input_pgm->data, input_pgm->height, input_pgm->width,
	H22, H22rows, H22cols, temp_pgm->data, 1);
	
	for(int i = 0; i < input_pgm->height; i++){
		for(int j = 0; j < input_pgm->width; j++){
			output_pgm->data[i][j] = abs(output_pgm->data[i][j]) + abs(temp_pgm->data[i][j]);
			if(output_pgm->data[i][j] > 255)
				output_pgm->data[i][j] = 255;
			
		}
	}
	write_pgm(output_pgm, "problem_3.pgm");
	
	
	float **imsq = (float **)malloc( input_pgm->height* sizeof(float *));
    for (int i = 0; i < input_pgm->height; i++) {
        imsq[i] = (float *)malloc(input_pgm->width * sizeof(float));
    }
	
	float **im5 = (float **)malloc( input_pgm->height* sizeof(float *));
    for (int i = 0; i < input_pgm->height; i++) {
        im5[i] = (float *)malloc(input_pgm->width * sizeof(float));
    }
	
	for(int i = 0; i < input_pgm->height; i++){
		for(int j = 0; j < input_pgm->width; j++){
			imsq[i][j] = input_pgm->data[i][j] * input_pgm->data[i][j];
		}
	}
	
	float **H4 = (float **)malloc( filter_pgm->height* sizeof(float *));
    for (int i = 0; i < filter_pgm->height; i++) {
        H4[i] = (float *)malloc(filter_pgm->width * sizeof(float));
		for(int j = 0; j < filter_pgm->width; j++){
			H4[i][j] = 1.0;
		}
    }
	
	conv_2d(imsq, input_pgm->height, input_pgm->width,
	H4, filter_pgm->height, filter_pgm->width, im5, 1);
	conv_2d(input_pgm->data, input_pgm->height, input_pgm->width, 
	filter_pgm->data, filter_pgm->height, filter_pgm->width, output_pgm->data, 1);
	float max_val = 0;
	float min_val = 255;
	for(int i = 0; i < output_pgm->height; i++){
		for(int j = 0; j < output_pgm->width; j++){
			output_pgm->data[i][j] = output_pgm->data[i][j]/im5[i][j];
			if(output_pgm->data[i][j] > max_val){
				max_val = output_pgm->data[i][j];
			}
			if(output_pgm->data[i][j] < min_val){
				min_val = output_pgm->data[i][j];
			}
		}
	}
	float scale = 255/max_val;
	for(int i = 0; i < output_pgm->height; i++){
		for(int j = 0; j < output_pgm->width; j++){
			output_pgm->data[i][j] = output_pgm->data[i][j] * scale;
			output_pgm->data[i][j] -= 255;
			output_pgm->data[i][j] = abs(output_pgm->data[i][j]);
		}
	}
	
	write_pgm(output_pgm, "problem_4.pgm");
	
    return 0;
}