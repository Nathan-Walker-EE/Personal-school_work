#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void processArray(double* arr, size_t size) {
    // Process the array elements
    for (size_t i = 0; i < size; ++i) {
        // Do something with arr[i]
        cout << arr[i] << " ";
    }
    cout << endl;
}

int Conv(double* f1, int len1, double* f2, int len2, double*& y) {
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
		if(y[k] != 0){
			cout << "y[" << k << "] = " << y[k] << endl;
		}
    }
	return kmax;
}

int main() {
	int t;
	int leny;
	double* result;
	
    double f2[] = {-2, -2, -2, -2, -2, -2, -2};
    int len2 = 7;

    double f3[] = {1, -1, 1, -1};
    int len3 = 4;
	
	double f1[] = {0,1,2,3,2,1};
	int len1 = 6;

	double f4[] = {0,0,0,-3,-3};
	int len4 = 5;
	
	ofstream outfile1("convolution_1");
	ofstream outfile2("convolution_2");
	ofstream outfile3("convolution_3");
	ofstream outfile4("convolution_4");
	ofstream outfile5("convolution_5");
	ofstream outfile6("convolution_6");

    
	
    leny = Conv(f1, len1, f1, len1, result);
	for(t = 0; t < leny; t++){
		outfile1 << t + 1 << " " <<  result[t] << endl;
	}
	delete[] result;
	
	leny = Conv(f1, len1, f2, len2, result);
	for(t = 0; t < leny; t++){
		outfile2 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	leny = Conv(f1, len1, f3, len3, result);
	for(t = 0; t < leny; t++){
		outfile3 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	leny = Conv(f3, len3, f2, len2, result);
	for(t = 0; t < leny; t++){
		outfile4 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	leny = Conv(f1, len1, f4, len4, result);
	for(t = 0; t < leny; t++){
		outfile5 << t + 1<< " " <<  result[t] << endl;
	}
    delete[] result; // Don't forget to free the allocated memory

	
    return 0;
}

int t[6/.001];
	h = -.3339 * exp(2.6038*-t) + .4202 * exp(-1.1981*t) .* cos(2.0787*t - deg2rad(-37.386));
	f = sin(3 * pi * t);