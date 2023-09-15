//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//
//const int size = 4; // global variable that holds the size
//const int bucketSize = 256 / size; // global variable for bucketSize
//
//// preconditins: foregorund image
//// postconditions: finds the most commor color in the foreground image and finds the histogram bin with most votes 
//void histogram(const Mat &Fimage) {
//	int dims[] = { size, size, size };
//	Mat hist(3, dims, CV_32S, Scalar::all(0));
//
//	// loops over foregorund image
//	for (int i = 0; i < Fimage.rows - 1; i++) {
//		for (int j = 0; j < Fimage.cols - 1; j++) {
//
//			// receives the pixel
//			Vec3b pix = Fimage.at<Vec3b>(i, j);
//
//			// updates rgb values
//			uchar red = pix.val[2];
//			uchar green = pix.val[1];
//			uchar blue = pix.val[0];
//
//			// increments bucket
//			int r = red / bucketSize;
//			int g = green / bucketSize;
//			int b = blue / bucketSize;
//
//			hist.at<int>(r, g, b)++;
//			
//		}
//	}
//
//	int votes = 0;
//
//	// loops over histogram
//	for (int r = 0; r < size; r++) {
//		for (int g = 0; g < size; g++) {
//			for (int b = 0; b < size; b++) {
//
//				// votes at current index in 3D
//				int newVote = hist.at<int>(r, g, b);
//
//				if (newVote > votes) {
//					// updates bin
//					cRed = r * bucketSize + bucketSize / 2;
//					cGreen = g * bucketSize + bucketSize / 2;
//					cBlue = b * bucketSize + bucketSize / 2;
//					votes = newVote; // sets max votes to votes
//				}
//			}
//		}
//	}
//}
//
//// preconditions: prompts for color red
//// postconditions: returns true if red is in BucketSize
//bool helperRed(uchar color) {
//	if (color >= cRed - bucketSize && color <= cRed + bucketSize) {
//		return true;
//	}
//	return false;
//}
//
//// preconditions: prompts for color green
//// postconditions: returns true if green is in BucketSize
//bool helperGreen(uchar color) {
//	if (color >= cGreen - bucketSize && color <= cGreen + bucketSize) {
//		return true;
//	}
//	return false;
//}
//
//// preconditions: prompts for color blue
//// postconditions: returns true if blue is in BucketSize
//bool helperBlue(uchar color) {
//	if (color >= cBlue - bucketSize && color <= cBlue + bucketSize) {
//		return true;
//	}
//	return false;
//}
//
//// preconditions: foreground and background images
//// postconditions: acts as agreen screen and background image becomes the background of foreground image and returns that particular image
//Mat overlayOutput(const Mat& image, const Mat& background) {
//	Mat outputImage = image.clone();
//
//	// loops over foreground image
//	for (int i = 0; i < outputImage.rows - 1; i++) {
//		for (int j = 0; j < outputImage.cols - 1; j++) {
//
//			// gets the current pixel
//			Vec3b pix = outputImage.at<Vec3b>(i, j);
//			// gets color of each pixel
//			uchar red = pix.val[2];
//			uchar green = pix.val[1];
//			uchar blue = pix.val[0];
//			
//			// calls the helper methods for each color
//			if (helperRed(red) && helperGreen(green) && helperBlue(blue)) {
//				// replaces foreground pixel with background pixel
//				outputImage.at<Vec3b>(i, j) = background.at<Vec3b>(i % background.rows, j % background.cols);
//			}
//		}
//	}
//	return outputImage;
//}
//
//// preconditions: prompts for background image
//// postconditions: flips the image horizontally, converts to grayscale, blurs it, and detects edges using openCV methods
//Mat openCVmethods(const Mat& image) {
//	Mat outputImage = image.clone();
//
//	// openCV methods
//	flip(outputImage.clone(), outputImage, 1);
//	cvtColor(outputImage.clone(), outputImage, COLOR_BGR2GRAY);
//	GaussianBlur(outputImage.clone(), outputImage, Size(7, 7), 2.0, 2.0);
//	Canny(outputImage.clone(), outputImage, 20, 60);
//
//	return outputImage;
//}
//
//int cRed = 0, cGreen = 0, cBlue = 0; // global variables for recording bin votes
//
//// precondition: reads foreground and background image
//// postconditions: creates histogram, replaces background pixels, outputs the resulted image, uses openCV methods to return a new image, and creates a personal myOutput image
//int main(int argc, char* argv[]) {
//
//	Mat foreground = imread("foreground.jpg");
//	Mat background = imread("background.jpg");
//
//
//	histogram(foreground);
//	Mat overlay = overlayOutput(foreground, background);
//
//	namedWindow("overlay", WINDOW_NORMAL);
//	resizeWindow("overlay", overlay.cols / 8, overlay.rows / 8);
//	imshow("overlay", overlay);
//	waitKey(0);
//	imwrite("overlay.jpg", overlay);
//
//	Mat output2 = openCVmethods(background);
//	imshow("output2", output2);
//	waitKey(0);
//	imwrite("output2.jpg", output2);
//
//	
//	Mat original_image = imread("foreground.jpg");
//	Mat grayscale_image;
//	Mat binary_image;
//	Mat outputImg = Mat::zeros(Size(original_image.cols, original_image.rows), original_image.type()); // sets the output image
//	Point2f inpMat[3];
//	Point2f outMat[3];
//
//	inpMat[0] = Point2f(0.0, 0.0);
//	inpMat[1] = Point2f(original_image.cols, 0);
//	inpMat[2] = Point2f(0, original_image.rows);
//
//	outMat[0] = Point2f(0, 200);
//	outMat[1] = Point2f(500, 100);
//	outMat[0] = Point2f(170, 520);
//
//	Mat M = getAffineTransform(inpMat, outMat); // performs affine transformation
//	warpAffine(original_image, outputImg, M, outputImg.size()); // performs image warping
//
//	cvtColor(outputImg, grayscale_image, COLOR_BGR2GRAY); // converts the color ot gray
//	threshold(grayscale_image, binary_image, 100, 255, THRESH_BINARY); // produces a binary image
//
//	namedWindow("Output_after_warp");
//	imshow("Output_after_warp", outputImg);  // image after warping
//	namedWindow("myOutput");
//	imshow("myOutput", binary_image); // image after warping to produce binary image
//	waitKey(0);
//	imwrite("myOutput.jpg", binary_image);
//
//	return 0;
//}