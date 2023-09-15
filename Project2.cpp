#define _USE_MATH_DEFINES 
#include <cmath>
#include "Image.h"
#include <iostream>

using namespace std;

// Preconditions: value is passed in the function with its border (height or width).
// Postconditions: returns a value depending upon if it is inside the border or outside.
float checkBoundryInterpolation(const float& val, const int& border) {

	if (val < 0) { return 0.0f; }
	else if (val > (border - 1)) { return border - 1; }
	return val;
}

// Preconditions: Input image, corresponding kernel, and values of x & y are passed.
// Postconditions: Returns the output image with linear filtering. 
Image filtering(const Image& input, const Image& krnl, const int& coord_x, const int& coord_y) {

	Image output(input.getRows(), input.getCols());

	int var_x = 0;
	int var_y = 0;
	float conv = 0.0f;

	// Nested for loop through the image.
	for (int i = 0; i < input.getRows(); i++) {
		for (int j = 0; j < input.getCols(); j++) {

			//Nested for loop through the kernel.
			for (int krnl_i = krnl.getRows() - 1; krnl_i >= 0; krnl_i--) {
				for (int krnl_j = krnl.getCols() - 1; krnl_j >= 0; krnl_j--) {

					// Checking if var_x and var_y are valid points.
					var_x = int(checkBoundryInterpolation(float(j + (coord_x - krnl_j)), float(input.getCols())));//j + (coord_x - krnl_j);
					var_y = int(checkBoundryInterpolation(float(i + (coord_y - krnl_i)), float(input.getRows())));//i + (coord_y - krnl_i);

					// convolving pixels
					conv += input.getFloat(var_y, var_x) * krnl.getFloat(krnl_i, krnl_j);
				}
			}
			output.setFloat(i, j, conv);
			conv = 0.0f;
		}
	}
	return output;
}

// Preconditions: Prompts for an inout image and iterations for smoothing the image.
// Postconditions: Returns the smooth image
Image smoothImage(const Image& input, const int& iterations) {

	// Creating x and y kernels for smoothing
	Image sX(1, 3);
	Image sY(3, 1);

	sX.setFloat(0, 0, .25);
    sX.setFloat(0, 1, .5);
    sX.setFloat(0, 2, .25);

    sY.setFloat(0, 0, .25);
    sY.setFloat(1, 0, .5);
    sY.setFloat(2, 0, .25);
	
	// Setting the output image as the input image before smoothing.
	Image output = input;

	// Goes through the whole image depending upon the number of iterations.
	for (int i = 0; i < iterations; i++) {
		// Smooths using x & y kernels
		output = filtering(output, sX, 1, 0);
		output = filtering(output, sY, 0, 1);
	}
	return output;
}

// Preconditions: Prompts for an input image to get its floating point.
// Postconditions: Returns the floating image.
Image floatPoint(const Image& input) {

	// Creats an alternate image, output.
	Image output(input.getRows(), input.getCols());

	// Loops through the image.
	for (int i = 0; i < input.getRows(); i++) {
		for (int j = 0; j < input.getCols(); j++) {

			// alters the output image.
			output.setFloat(i, j, input.getPixel(i, j).grey);
		}
	}
	return output;
}

// Preconditions: Prompts for an input image.
// Postconditions: Returns the grey image.
Image greyScale(const Image& input) {
	
	Image output(input.getRows(), input.getCols());

	for (int i = 0; i < input.getRows(); i++) {
		for (int j = 0; j < input.getCols(); j++) {

			// Sets grey pixel.
			output.setGrey(i, j, static_cast<byte>(input.getFloat(i, j)));
		}
	}
	return output;
}

// Preconditions: Prompts for an image and two values, xA & yA.
// Postconditions: Performs bilinear image
float interpolate(const Image& magnitude, const float& xA, const float& yA) {

	//Checks if the float values are crossing the border or just are invalid.
	float x = checkBoundryInterpolation(xA, magnitude.getCols());
	float y = checkBoundryInterpolation(yA, magnitude.getRows());

	// alpha & beta values from the lecture.
	float alpha = x - floor(x);
	float beta = y - floor(y);

	// Formula, hand written, from the lecture
	float interpolation = ((1 - alpha) * (1 - beta) * magnitude.getFloat(y, x)) + (alpha * (1 - beta) * magnitude.getFloat(ceil(y), x)) + ((1 - alpha) * beta * magnitude.getFloat(y, ceil(x))) + (alpha * beta * magnitude.getFloat(ceil(y), ceil(x)));

	return interpolation;

}

// Preconditions: Prompts for the magnitude image, kernels, and row and col.
// Postconditions: Identifies the edge point.
float edgeIdentifier(const Image& magnitude, const Image& gradeX, const Image& gradeY, int row, int col) {

	float krnlX = gradeX.getFloat(row, col) / magnitude.getFloat(row, col);
	float krnlY = gradeY.getFloat(row, col) / magnitude.getFloat(row, col);

	// Points.
	float rowX = col + krnlX, rowY = row + krnlY, pointX = col - krnlX, pointY = row - krnlY;
	
	// Performs the interpolation.
	float x = interpolate(magnitude, rowX, rowY);
	float y = interpolate(magnitude, pointX, pointY);

	float z = magnitude.getFloat(row, col);

	// checking if edge is valid.
	if (z > x && z > y) { return 255.0; }
	return 0.0;
}

// Preconditions: Prompts for an image, sets up kernels, and detects the edges of an image.
// Postconditions: Returns an image with only edges.
Image edgeDetection(const Image& input) {
	
	// Makes an identical image.
	Image output(input.getRows(), input.getCols());

	// Setting up x and y kernels.
	Image Ey(3, 1);
	Image Ex(1, 3);

	Ex.setFloat(0, 0, -1);
	Ex.setFloat(0, 1, 0);
	Ex.setFloat(0, 2, 1);

	Ey.setFloat(0, 0, -1);
	Ey.setFloat(1, 0, 0);
	Ey.setFloat(2, 0, 1);

	// Performs convolving.
	Image gradientX = filtering(input, Ex, 1, 0);
	Image gradientY = filtering(input, Ey, 0, 1);

	// Creates magitude image with gradient X & Y rows & cols.
	Image magnitude(gradientX.getRows(), gradientY.getCols());
	float sqX, sqY, r;

	int power = 2;

	// Loops through the magnitude image.
	for (int i = 0; i < gradientX.getRows(); i++) {
		for (int j = 0; j < gradientY.getCols(); j++) {
			sqX = pow(gradientX.getFloat(i, j), power);
			sqY = pow(gradientY.getFloat(i, j), power);
			r = sqrt(sqX + sqY);
			magnitude.setFloat(i, j, r);
		}
	}

	for (int i = 0; i < input.getRows(); i++) {
		for (int j = 0; j < input.getCols(); j++) {

			if (magnitude.getFloat(i, j) >= 10.0) {
			
				float q = edgeIdentifier(magnitude, gradientX, gradientY, i, j);
				output.setFloat(i, j, q);
				output.setFloat(i, j, q);
			}
			else {
				output.setFloat(i, j, 0.0);
			}
		}
	}

	return output;
}

// Main function. Alters the image with float point, linear filtering convolution, smoothing, edge detecttion, makes it appear grey.
int main() {
	int iterations = 5;

	Image input("Profile.gif");

	Image floatingImag = floatPoint(input);
	Image smoothing = smoothImage(floatingImag, iterations);
	Image edges = edgeDetection(smoothing);

	Image SmoothGrey = greyScale(smoothing);
	Image EdgesGrey = greyScale(edges);

	SmoothGrey.writeGreyImage("smooth.gif");
	EdgesGrey.writeGreyImage("edges.gif");

	return 0;
}