/*****************************************
Assignment: Numerical Solution of Differential Equations
:The Zero-Input Solution

pseudocode for first order equations
	
	DEFINE delta_t = time stepping
	

	get initial conditions
	get constant a (equation 1)
	
	set yo as initial conditions
	
	open output file
	
	create integers y_now and y_now_delta_t and t
	
	y_now = yo
	t = 0
	
	loop
		y_now_delta_t = (1 + a*delta_t)y_now
		store y_now in text file
		store t in text file
		
		t = t + delta_t
		y_now = y_now_delta_t
		
	while (counter < 10/delta_t)
		
	close output file
	


******************************************/



#include <fstream>
#include <iostream>
using namespace std;

/*****************************************
Assignment: Numerical Solution of Differential Equations
:The total solution
	
	lengthy = lengthf + lengthh
	loop
		total = f[i]*h[n - i]
		y[n] += total
	end loop when i = n

******************************************/

int conv(double *f1, int len1, double *f2, int len2, double *y){
	int lengthy = len1 + len2;
	int total
	int n = 0;
	for(int j = 0; j < lengthy; j++){
		total = 0;
		for(int i = 0; i <= n; i ++){
			total = total + (f1[i] * f2[i]);
		}
		y[n] = total;
		n++;
	}
	return lengthy;
}

int main(void){
	
	int a, yo;
	float t;
	float Delta_t = .0001;
	
	cout << "please enter in a: ";
	cin >> a;
	cout << "please enter in yo: ";
	cin >> yo;
	
	float y_now, y_next;
	a = -a;
	
	ofstream outfile("approximation");
	
	y_now = yo;
	
	for(t = 0; t < 10; t = t + Delta_t){
		y_next = (1 + a * Delta_t)*y_now;
		outfile << t << " " << y_now << endl;
		y_now = y_next;
	}
	
	outfile.close();
	
	return 0;
}

