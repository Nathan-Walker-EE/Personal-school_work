/*****************************************
Assignment: Numerical Solution of Differential Equations
:The total solution
	
	lengthy = lengthf + lengthh
	loop
		total = f[i]*h[n - i]
		y[n] += total
	end loop when i = n

******************************************/



#include <fstream>
#include <iostream>
using namespace std;


//ryans
/*float Conv(double* f1, int len1, double* f2, int len2, double* y)
{
	int kmax = len1 + len2-1;
	y = new double[kmax + 1];
	for (int k = 0; k <= kmax; k++){
		y[k] = 0;
		for (int m = 0; m < len1; m++){
			if (k - m >= 0 && k-m < len2) {
				y[k] += f1[m] * f2[k - m];
			}
		}
		printf("y[%d] = %f\n", k, y[k]);
	}
return (float) kmax + 1;
}*/
	
void Conv(double* f1, int len1, double* f2, int len2, double*& y) {
    int kmax = len1 + len2;
    y = new double[kmax];

    for (int k = 0; k < kmax; k++) {
        y[k] = 0;
        for (int m = 0; m < len1; m++) {
            int index = k - m;
            if (index >= 0 && index < len2) {
                y[k] += f1[m] * f2[index];
            }
        }
        std::cout << "y[" << k << "] = " << y[k] << std::endl;
    }
}

//mine
/*int conv(double *f1, int len1, double *f2, int len2, double *&y){
	int lengthy = len1 + len2;
	y = new double[lengthy];
	double total;
	for(int j = 0; j <= lengthy; j++){
		total = 0;
		for(int i = 0; i <= j; i++){
			if(j-i >= 0 && j-i < len1 && j-i < len2 && i < len1){
				total += f1[i] * f2[j-i];
			}
		}
		y[j] = total;
		cout << "y[" << j << "] is " << total << endl;
	}
	
	cout << "got out";
	return lengthy;
}*/

int main(void){
	int t;
	double f1[] = {0,1,2,3,2,1};
	int len1 = 6;
	double f2[] = {-2,-2,-2,-2,-2,-2,-2};
	int len2 = 7;
	double f3[] = {1,-1,1,-1};
	int len3 = 4;
	double f4[] = {0,0,0,-3,-3};
	int len4 = 5;
	double *ys1;
	double *ys2;
	double *ys3;
	double* ys4;
	double *ys5;
	float leny1;
	float leny2;
	float leny3;
	float leny4;
	float leny5;
	
	cout << "got to point 1" << endl;
	ofstream outfile1("convolution_1");
	ofstream outfile2("convolution_2");
	ofstream outfile3("convolution_3");
	ofstream outfile4("convolution_4");
	ofstream outfile5("convolution_5");
	
	cout << "got to point 2" << endl;
	//leny1 = Conv(f1, len1, f1, len1, ys1);
	//leny2 = Conv(f1, len1, f2, len1, ys2);
	//leny3 = Conv(f1, len1, f3, len1, ys3);
	Conv(f2, len1, f3, len1, ys4);
	//leny5 = Conv(f1, len1, f4, len1, ys5);
	/*
	for(t = 0; t <= leny1; t++){
		cout << "got here";
		outfile1 << t + 1 << " " <<  ys1[t] << endl;
	}
	
	for(t = 0; t <= leny2; t++){
		outfile2 << t + 1<< " " <<  ys2[t] << endl;
	}
	for(t = 0; t <= leny3; t++){
		outfile3 << t + 1<< " " <<  ys3[t] << endl;
	}
	for(t = 0; t <= leny4; t++){
		outfile4 << t + 1<< " " <<  ys4[t] << endl;
	}
	for(t = 0; t <= leny5; t++){
		outfile5 << t + 1<< " " <<  ys5[t] << endl;
	}
	*/
	//delete[] ys1;
	//delete[] ys2;
	//delete[] ys3;
	delete[] ys4;
	//delete[] ys5;
	
	return 0;
}