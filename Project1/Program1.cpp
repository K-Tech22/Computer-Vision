#define _USE_MATH_DEFINES
#include "Image.h"
#include <iostream>
#include <math.h>
#include <cmath>

Image func(const Image & input, double& scale_x, double& scale_y, double& tX, double& tY, double& theta, double& K) {
	Image output(input.getRows(), input.getCols());
	
	double r = input.getRows();     // total number of rows
	double c = input.getCols();     // total number of cols
	double center_x = c / 2.0;      // X coordinate of center
	double center_y = r / 2.0;      // Y coordinate of center
	theta = theta * M_PI / 180;     // theta comes out in degrees

	double pX, pY, qX, qY = 0.0;   

	for (int row = 0; row < input.getRows(); row++) {        // looping over rows
		double qY = double(row) - tX;						 // applying translation
		for (int col = 0; col < input.getCols(); col++) {    // looping over cols
			double qX = double(col) -tY;					 // applying translation 

			pX = ((qX - center_x) * (cos(theta) / scale_x)) + ((qY - center_y) * (((-K * cos(theta)) / scale_y) + (sin(theta) / scale_y))) + center_x; // applying Scale, Shear, & Rotation to cols
			pY = ((qX - center_x) * (-sin(theta) / scale_x)) + ((qY - center_y) * (((K * sin(theta)) / scale_y) + (cos(theta) / scale_y))) + center_y; // applying Scale, Shear, & Rotation to rows

			if (pX >= 0 && pX < input.getCols() && pY >= 0 && pY < input.getRows()) {													// pixel will be taken iff pX/pY is greater than 0 and less than total rows & columns
				
				output.setPixel(row, col, input.getPixel(pY, pX));																		// Pixel is set on the output image
			}
		}
	}

	return output;
}

int main(int argc, char* argv[])
{
	Image input("test1.gif");
	double scale_x = 1.0;
    double scale_y = 1.0;
    double tX = 0.0;
    double tY = 0.0;
    double theta = 0.0;
    double K = 0.0;
	sscanf_s(argv[1], "%lf", &scale_x);
	sscanf_s(argv[2], "%lf", &scale_y);
	sscanf_s(argv[3], "%lf", &tX);
	sscanf_s(argv[4], "%lf", &tY);
	sscanf_s(argv[5], "%lf", &theta);
	sscanf_s(argv[6], "%lf", &K);
	Image output = func(input, scale_x, scale_y, tX, tY, theta, K);
	output.writeImage("output.gif");
	return 0;
}