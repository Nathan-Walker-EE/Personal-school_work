#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void conv_2d(unsigned char** image, int prow, int pcol, unsigned char** filter, int frow, int fcol, /*unsigned char** output_image,*/ double scale)
{
	int k = prow + 2*(frow)-1;
	int l = pcol + 2*(fcol)-1;
	int m = prow;
	int n = pcol;
	
	unsigned char imagezp[k][l];
	unsigned char output_image[m][n];
	
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
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			for(int x = 0; x < frow; x++){
				for(int y = 0; y < fcol; y++){
					output_image[i][j] += imagezp[i + x + frow/2 -1][j + y + fcol/2 - 1] * filter[x][y] * scale;
					if(output_image[i][j] < 0){
						output_image = 0;
					}else if(output_image[i][j] > 255){
						output_image = 255;
					}
				}
			}
		}
	}
}

int main(void){
	int irows = 7;
	int icols = 7;
	unsigned char **image = (unsigned char **)malloc( irows * sizeof(unsigned char *));
    
    // Allocate memory for each row (columns)
    for (int i = 0; i < irows; i++) {
        image[i] = (unsigned char *)malloc(icols * sizeof(unsigned char));
    }
	for(int i = 0; i < irows; i++){
		for(int j = 0; j < icols; j++){
			image[i][j] = 1;
		}
	}
	
	int frows = 2;
	int fcols = 6;
	unsigned char **filter = (unsigned char **)malloc(frows * sizeof(unsigned char *));
    
    // Allocate memory for each row (columns)
    for (int i = 0; i < frows; i++) {
        filter[i] = (unsigned char *)malloc(fcols * sizeof(unsigned char));
    }
	for(int i = 0; i < frows; i++){
		for(int j = 0; j < fcols; j++){
			filter[i][j] = 1;
		}
	}
	unsigned char **output_image = (unsigned char **)malloc(irows * sizeof(unsigned char *));
    
    // Allocate memory for each row (columns)
    for (int i = 0; i < irows; i++) {
        output_image[i] = (unsigned char *)malloc(icols * sizeof(unsigned char));
    }
	for(int i = 0; i < irows; i++){
		for(int j = 0; j < icols; j++){
			output_image[i][j] = 0;
		}
	}
	conv_2d(image, irows,icols, filter, frows, fcols, output_image);
	
	return(0);
}