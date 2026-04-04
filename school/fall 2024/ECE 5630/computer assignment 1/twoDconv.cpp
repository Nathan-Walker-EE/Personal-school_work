/*******************************************************************
* ECE 5630 Program 1
*
*	Two Dimensional Convolution
* 
* Author: AJ Womack
* Date: 10/07/2022
*******************************************************************/

using namespace std;

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

float** twoDconv(float** x, int widthX, int heightX, float** h, int widthH, int heightH, int* widthY, int* heightY);

int main(int argc, char** argv) {
	cout << "Open file 'image.pgm'" << endl;
	ifstream image_file("C:/Users/aaron/OneDrive/Desktop/School/FALL_2022/twoDconvolution/image.pgm", ios::binary);

	int im_rows, im_cols;
	string line;
	stringstream ss;
	uint8_t temp;

	//Parse out the version of pgm file
	getline(image_file, line);

	//Remove all commented lines
	for (int i = 0; i < 19; i++)
	{
		//getline(image_file, line);
	}

	//Read in the line of rows/cols of the image
	getline(image_file, line);
	ss << line;
	ss >> im_rows >> im_cols;
	cout << "Rows in Image: " << im_rows << endl;
	cout << "Cols in Image: " << im_cols << endl;

	//Read in the resolution
	int resolution;
	image_file >> noskipws;
	image_file >> resolution;
	cout << "Resolution: " << resolution << endl;

	//Read in the image array
	float** image = new float*[im_rows];
	for (int i = 0; i < im_rows; i++)
	{
		image[i] = new float[im_rows];
		for (int j = 0; j < im_cols; j++)
		{
			image_file >> temp;
			image[i][j] = (float)temp;
		}
	}
	cout << "\nImage array generated. Rows: " << im_rows << " Cols: " << im_cols << endl;

	/*******************************************************************
	* Problem 2: convolve image with LPF (Blur)
	*******************************************************************/
	/*
	//Initialize the low pass filter (h1)
	int widthH1 = 5;
	int heightH1 = 5;
	float** h1 = new float*[widthH1];
	for (int i = 0; i < widthH1; i++)
	{
		h1[i] = new float[heightH1];
		h1[i][0] = 1;
		h1[i][1] = 2;
		h1[i][2] = 3;
		h1[i][3] = 2;
		h1[i][4] = 1;
	}
	for (int i = 0; i < widthH1; i++)
	{
		int scale = 1;
		if ((i == 1) || (i == 3)) {
			scale = 2;
		}
		if ((i == 2))
		{
			scale = 3;
		}
		for (int j = 0; j < heightH1; j++)
		{
			h1[i][j] = (h1[i][j] * scale) / 81;
		}
	}

	//Convolve h1 with image
	int y1_rows, y1_cols;
	float** y1 = twoDconv(image, im_rows, im_cols, h1, widthH1, heightH1, &y1_rows, &y1_cols);

	//Print the result for Problem 2 to a file
	ofstream output1("lowpass.pgm", ios::binary);
	int offsetH = (heightH1 - 1) / 2;
	int offsetW = (widthH1 - 1) / 2;
	string version = "P5";
	output1 << version << "\n";
	output1 << im_rows << "\n" << im_cols << "\n" << 255 << "\n";

	cout << "Printing y1 to a file\n" << endl;
	for (int i = offsetH; i < im_rows + offsetH; i++)
	{
		for (int j = offsetW; j < im_cols + offsetW; j++)
		{
			output1 << (uint8_t)y1[i][j];
		}
	}
	cout << "y1 printed successfully." << endl;
	/********************************************************************
	* Problem 3: Sobel filter for edge detection
	*********************************************************************/
	/*
	//initialize sobel filters
	int s_rows = 3;
	int s_cols = 3;
	float** s1 = new float* [s_rows];
	float** s2 = new float* [s_rows];
	for (int i = 0; i < s_rows; i++)
	{
		s1[i] = new float[s_cols];
		s2[i] = new float[s_cols];
	}
	for (int i = 0; i < s_rows; i++)
	{
		s1[0][i] = 1;
		s1[1][i] = 0;
		s1[2][i] = -1;
		s2[i][0] = 1;
		s2[i][1] = 0;
		s2[i][2] = -1;
	}
	for (int i = 0; i < s_rows; i++)
	{
		s1[i][1] *= 2;
		s2[1][i] *= 2;
	}

	//Convolve s1 with the image and s2 with the image to get g1 and g2
	int g_rows, g_cols;
	float** g1 = twoDconv(image, im_rows, im_cols, s1, 3, 3, &g_rows, &g_cols);
	float** g2 = twoDconv(image, im_rows, im_cols, s2, 3, 3, &g_rows, &g_cols);

	//add absoluate value of g1 and g2 to get m
	float** m = new float* [g_rows];
	for (int i = 0; i < g_rows; i++)
	{
		m[i] = new float[g_cols];
		for (int j = 0; j < g_cols; j++)
		{
			m[i][j] = abs(g1[i][j]) + abs(g2[i][j]);
			if (m[i][j] > 255) {
				m[i][j] = 255;
			}

		}
	}

	//Print the result for Problem 3 to a file
	ofstream output2("edge_detect.pgm", ios::binary);
	int offsetR = (s_rows - 1) / 2;
	int offsetC = (s_cols - 1) / 2;
	output2 << version << "\n";
	output2 << im_rows << "\n" << im_cols << "\n" << 255 << "\n";

	cout << "Printing m to a file\n" << endl;
	for (int i = offsetR; i < im_rows + offsetR; i++)
	{
		for (int j = offsetC; j < im_cols + offsetC; j++)
		{
			output2 << (uint8_t)m[i][j];
		}
	}
	
	/************************************************************************
	* Problem 4: Correlation filter for feature detection
	*************************************************************************/

	//Read in the filter .pgm file
	int f_rows, f_cols, f_res;
	ifstream filter_file("C:/Users/aaron/OneDrive/Desktop/School/FALL_2022/twoDconvolution/filter_final.pgm");
	line = "";
	ss.str(std::string());
	getline(filter_file, line); //Version, not needed
	filter_file >> f_rows;
	filter_file >> f_cols;
	filter_file >> f_res;

	float** filter = new float*[f_rows];
	filter_file >> noskipws;
	for (int i = 0; i < f_rows; i++)
	{
		filter[i] = new float[f_cols];
		for (int j = 0; j < f_cols; j++)
		{
			filter_file >> temp;
			filter[i][j] = (float)temp;
		}
	}
	cout << "\nFilter array generated. Rows: " << f_rows << "\tCols: " << f_cols << endl;
	
	//Part 4a: square all of the image pixel values
	float** image_sq = new float* [im_rows];
	for (int i = 0; i < im_rows; i++)
	{
		image_sq[i] = new float[im_cols];
	}
	for (int i = 0; i < im_rows; i++)
	{
		for (int j = 0; j < im_cols; j++)
		{
			image_sq[i][j] = image[i][j] * image[i][j];
		}
	}
	cout << "Image_sq generated successfully." << endl;

	//Part 4b: Create a filter the same size as filter.pgm. All the values should be 1.0. Convolve with image_sq
	float** my_filter = new float* [f_rows];
	for (int i = 0; i < f_rows; i++)
	{
		my_filter[i] = new float[f_cols];
		for (int j = 0; j < f_cols; j++)
		{
			my_filter[i][j] = 1.0;
		}
	}
	cout << "My_filter generated successfully." << endl;

	int final_rows, final_cols;
	float** filter_sq = twoDconv(image_sq, im_rows, im_cols, my_filter, f_rows, f_cols, &final_rows, &final_cols);

	//Part 4c: Convolve filter_final.pgm and image.pgm, then divide by 4b pixel by pixel
	float** filter_final = twoDconv(image, im_rows, im_cols, filter, f_rows, f_cols, &final_rows, &final_cols);
	for (int i = 0; i < final_rows; i++)
	{
		for (int j = 0; j < final_cols; j++)
		{
			filter_final[i][j] = filter_final[i][j] / filter_sq[i][j];
		}
	}
	//Part 4d: Scale the filtered image from step 4c so that max value is 255 (and min is 0). Save the resulting image in .pgm format
	float max_val = filter_final[0][0];
	float min_val = filter_final[0][0];
	for (int i = 0; i < final_rows; i++)
	{
		for (int j = 0; j < final_cols; j++)
		{
			if (filter_final[i][j] > max_val)
			{
				max_val = filter_final[i][j];
			}
			if (filter_final[i][j] < min_val)
			{
				min_val = filter_final[i][j];
			}
		}
	}
	float scale = (max_val - min_val) / 255;
	for (int i = 0; i < final_rows; i++)
	{
		for (int j = 0; j < final_cols; j++)
		{
			filter_final[i][j] = (filter_final[i][j] - min_val) / scale;
		}
	}

	//Print the final output for Part 4
	ofstream output3("correlation.pgm", ios::binary);
	int offsetFr = (f_rows - 1) / 2;
	int offsetFc = (f_cols - 1) / 2;
	string version = "P5";
	output3 << version << "\n";
	output3 << im_rows << "\n" << im_cols << "\n" << 255 << "\n";

	cout << "Printing final_filter to a file\n" << endl;
	for (int i = offsetFr; i < im_cols + offsetFr; i++)
	{
		for (int j = offsetFc; j < im_rows + offsetFc; j++)
		{
			output3 << (uint8_t)filter_final[j][i];
			cout << (uint8_t)filter_final[j][i];
		}
		cout << endl;
	}

	cout << "Program 1 Complete!" << endl;

	return 0;
}




float** twoDconv(float** x, int widthX, int heightX, float** h, int widthH, int heightH, int* widthY, int* heightY)
{
	// Find the size of y, allocate memory
	*widthY = widthX + widthH - 1;
	*heightY = heightX + heightH - 1;
	float** y = new float* [*widthY];
	for (int i = 0; i < *widthY; i++)
	{
		y[i] = new float[*heightY];
		for (int j = 0; j < *heightY; j++)
		{
			y[i][j] = 0;
		}
	}
	cout << "Size of X: " << widthX << " by " << heightX << endl;
	cout << "Size of H: " << widthH << " by " << heightH << endl;
	cout << "Size of Y: " << *widthY << " by " << *heightY << endl;

	//perform the convolution
	int lower_m = 0;
	int lower_n = 0;
	int upper_m = 0;
	int upper_n = 0;

	for (int l = 0; l <= *heightY; l++)
	{
		upper_m = 0;
		lower_m = 0;		
		if (l > heightH-1) {
			lower_n++;
		}
		for (int k = 0; k <= *widthY; k++)
		{
			if (k > widthH-1) {
				lower_m++;
			}
			for (int n = lower_n; n <= upper_n; n++)
			{
				for (int m = lower_m; m <= upper_m; m++)
				{
					y[k][l] += x[m][n] * h[k-m][l-n];
				}
			}
			if (upper_m < widthX-1) {
				upper_m++;
			}
		}
		if (upper_n < heightX-1) {
			upper_n++;
		}
	}

	cout << "End of twoDconv function." << endl;

	return y;
}
