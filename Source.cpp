#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

int thresh = 50, N = 5;

double angle(Point pt1, Point pt2, Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

void findSquares(const Mat& image, vector<vector<Point> >& squares) {
    squares.clear();

    Mat timg(image);

    medianBlur(image, timg, 9);

    Mat gray0(timg.size(), CV_8U), gray;

    vector<vector<Point> > contours;

    for (int c = 0; c < 3; c++) {

        int ch[] = { c, 0 };
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        for (int l = 0; l < N; l++) {
            if (l == 0) {
                
                Canny(gray0, gray, 5, thresh, 5);
                dilate(gray, gray, Mat(), Point(-1, -1));
            }
            else {
                gray = gray0 >= (l + 1) * 255 / N;
            }
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            
            for (size_t i = 0; i < contours.size(); i++)
            {
                
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

                if (approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)))
                {
                    double maxCosine = 0;

                    for (int j = 2; j < 5; j++)
                    {
                        
                        double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    
                    if (maxCosine < 0.3)
                        squares.push_back(approx);
                }
            }
        }
    }
}

void  findCircles(Mat& image) {
    Mat out = image.clone();
    cvtColor(out.clone(), out, COLOR_BGR2GRAY);
    medianBlur(out, out, 5);

    vector<Vec3f> circles;
    HoughCircles(out, circles, HOUGH_GRADIENT, 1, out.rows / 16, 100, 30, 1, 30);

    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
       
        circle(image, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        
        int radius = c[2];
        circle(image, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }
    imshow("C", image);

}

void drawSquares(Mat& image, const vector<vector<Point> >& squares) {
    for (size_t i = 0; i < squares.size(); i++)
    {
        const Point* p = &squares[i][0];

        int n = (int)squares[i].size();
        
        if (p->x > 3 && p->y > 3)
            polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
    }

    imshow("Traffic signs", image);
}


Mat computeImage(Mat& image) {

	Mat outputImage = image.clone();

	cvtColor(outputImage.clone(), outputImage, COLOR_BGR2GRAY);
	GaussianBlur(outputImage.clone(), outputImage, Size(7, 7), 2.0, 2.0);

	return outputImage;
}

int main() {

    const char* pics[] = {"square.jpg", "speed.jpg"};
    namedWindow("Traffic signs", 1);
    vector<vector<Point> > squares;

    for (int i = 0; pics[i] != 0; i++) {
        Mat image = imread(pics[i], 1);
        findSquares(image, squares);
        drawSquares(image, squares);
        waitKey(0);
    }

    Mat ig = imread("circle.jpg");
    findCircles(ig);
    waitKey(0);

    return 0;
}