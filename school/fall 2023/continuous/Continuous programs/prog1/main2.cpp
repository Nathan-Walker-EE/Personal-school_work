
/*****************************************
Assignment: Numerical Solution of Differential Equations
:The Zero-Input Solution

pseudocode for third order equations

	DEFINE delta_t = time stepping
	

	get initial conditions
	get constant a0, a1, a2 (equation 1)
	
	set yo, yo', yo'' as initial conditions
	
	open output file
	
	create integers y_now and y_now_delta_t and t
	
	X = [yo, yo', yo'']
	t = 0
	
	loop
		y_now_delta_t = (1 + a*delta_t)y_now
		store y_now in text file
		store t in text file
		
		t = t + delta_t
		y_now = y_now_delta_t
		
	while (counter < 10/delta_t)
		
	close output file
*****************************************/

#include <fstream>
#include <iostream>

using namespace std;

void mattimes(double t, double a[][3],double m[][3])
{
	int i,j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			m[i][j] = t*a[i][j];
}
void matadd(double a[][3],double b[][3],double c[][3])
{
	int i,j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			c[i][j] = a[i][j]+b[i][j];
}

void matvecmult(double m[][3], double *v, double *prod)
{
	double sum;
	int i,j;
	for(i = 0; i < 3; i++) {
		sum = 0;
		for(j = 0; j < 3; j++) {
			sum += m[i][j]*v[j];
		}
	prod[i] = sum;
	}
}


int main(void){
	
	double yo, yoD, yoD2;
	double t = 0;
	double delta_t = .0001;
	double a0, a1, a2;
	double X_next[3] = {0};
	
	ofstream outfile("approximation");
	
	cout << "enter a0: ";
	cin >> a0;
	cout << "enter a1: ";
	cin >> a1;
	cout << "enter a2: ";
	cin >> a2;
	cout << "enter yo: ";
	cin >> yo;
	cout << "enter yo': ";
	cin >> yoD;
	cout << "enter yoD: ";
	cin >> yoD2;
	
	double X_now[3] = {yo, yoD, yoD2};
	
	double a[3][3] = {{0, 1, 0}, {0, 0, 1}, {-a0, -a1, -a2}};
	double I[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	double A[3][3] = {0};
	double temp[3][3] = {0};
	double *Prod1;
	double Prod2[3][3] = {0};
	mattimes(delta_t, a, A);
	
	for(t = 0; t < 10; t = t + delta_t){
		matadd(A, I, temp);
		matvecmult(temp, X_now, X_next);
		outfile << t << " " << X_now[0] << endl;
		for(int i = 0; i < 3; i++){
			X_now[i] = X_next[i];
		}
	}
	
	outfile.close();
	
	return 0;
}