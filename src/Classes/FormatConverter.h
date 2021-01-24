#ifndef FORMATCONVERTER_H
#define FORMATCONVERTER_H

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class FormatConverter {
private:

public:
	FormatConverter();
	Mat rgb_to_yuv420(Mat &frame);
	Mat yuv420_to_rgb(Mat &frame);
	Mat rgb_to_yuv422(Mat &frame);
	Mat yuv422_to_rgb(Mat &frame);
	Mat rgb_to_yuv444(Mat &frame);
	Mat yuv444_to_rgb(Mat &frame);
};
#endif