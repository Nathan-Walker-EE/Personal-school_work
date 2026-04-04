#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//implementation of convolution using array, makes it easier for larger arrays
void Convolution(const vector<double>& f1, const vector<double>& f2, vector<double>& result, int T) {
	//find length 1
    int len1 = f1.size();
	//find length 2
    int len2 = f2.size();
	//find length 3
    int kmax = len1 + len2 - 1;
	
	//resize vector
    result.resize(kmax);

    for (int k = 0; k < kmax; ++k) {
		//sets result to 0
        result[k] = 0; 
		//perform the algebra for single time
        for (int m = 0; m < len1; ++m) {
            int index = k - m;
			//checks if it is within bounds
            if (index >= 0 && index < len2) {
				//sum of multiplication
                result[k] += f1[m] * f2[index];
            }
        }
    }
}

//functions same as above but uses arrays rather than vector and vector functions
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
    }
	return kmax;
}


int main() {
    // Read data from the first text file
	
	int t;
	int leny;
	double* result;
	
	//all the different arrays given in PDF
    double f2[] = {-2, -2, -2, -2, -2, -2, -2};
    int len2 = 7;

    double f3[] = {1, -1, 1, -1};
    int len3 = 4;
	
	double f1[] = {0,1,2,3,2,1};
	int len1 = 6;

	double f4[] = {0,0,0,-3,-3};
	int len4 = 5;
	
	//all output files for matlab
	ofstream outfile1("convolution_1");
	ofstream outfile2("convolution_2");
	ofstream outfile3("convolution_3");
	ofstream outfile4("convolution_4");
	ofstream outfile5("convolution_5");
	ofstream outfile6("convolution_6");

    
	//computes A
    leny = Conv(f1, len1, f1, len1, result);
	for(t = 0; t < leny; t++){
		//outputs result and time to output file
		outfile1 << t + 1 << " " <<  result[t] << endl;
	}
	delete[] result;
	
	//computes B
	leny = Conv(f1, len1, f2, len2, result);
	for(t = 0; t < leny; t++){
		//outputs result and time to output file
		outfile2 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	//computes C
	leny = Conv(f1, len1, f3, len3, result);
	for(t = 0; t < leny; t++){
		//outputs result and time to output file
		outfile3 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	//computes D
	leny = Conv(f3, len3, f2, len2, result);
	for(t = 0; t < leny; t++){
		//outputs result and time to output file
		outfile4 << t + 1<< " " <<  result[t] << endl;
	}
	delete[] result;
	
	//computes E
	leny = Conv(f1, len1, f4, len4, result);
	for(t = 0; t < leny; t++){
		//outputs result and time to output file
		outfile5 << t + 1<< " " <<  result[t] << endl;
	}
    delete[] result; // Don't forget to free the allocated memory
	
	//opens file that has transfer function values
    ifstream file1("output.txt");
	//transfer function
    vector<double> h;
	//dummy variable
    double value;
	//dummy variable
	int T = 1;

	//while value is still reading from file
    while (file1 >> value) {
		//gets value for transfer function
        h.push_back(value);
    }

    //opens file that has sin function values
    ifstream file2("output2.txt");
	//sin function
    vector<double> f;

	//while value is still reading from file
    while (file2 >> value) {
		//gets value for transfer function
        f.push_back(value);
    }

    //creates convolution solution
    vector<double> convolutionResult;
	//computes convolution
    Convolution(h, f, convolutionResult, T);

    // Write the result to an output text file
    ofstream outputFile("convolution_result.txt");
	//sets current time and step value
    double currentTime = 0.0;
    double timeStep = .001;

	//f[k]*h[k] = T * sum(f[k] x h[k])
	for(int i = 0; i < convolutionResult.size(); i++){
		convolutionResult[i] *= .001;
	}
	
	//gets value from program 1 for zero-input solution
	ifstream file3("approximation2");
	vector<double> zero_input;

    for (const auto& resultValue : convolutionResult) {
		//gets value from program 1
		file3 >> value;
		//adds yZS and yZI
		double temp = resultValue + value;
		//outputs time and value
        outputFile << currentTime << " " << temp << endl;
		//increment time
        currentTime += timeStep;
    }

    return 0;
}
