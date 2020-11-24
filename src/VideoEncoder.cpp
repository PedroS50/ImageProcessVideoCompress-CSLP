#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include <iostream>

using namespace cv;
using namespace std;

void encodeFrame(Mat frame, Golomb &g) {
	/*unsigned char data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Mat testMat = Mat(3, 3, CV_8UC1, data);
	Mat col = Mat::zeros(3, 1, CV_8UC1);
	Mat row = Mat::zeros(1, testMat.cols+1, CV_8UC1);
	Mat result;
	cout << testMat.rows << ", " << row << endl;
	hconcat(col, testMat, result);
	vconcat(row, result, result);
	cout << result << endl;*/
	/***************************************************************/
	int ch, i, n, colorVal, prim, err;
	// Padding the frame with outter zeros
	Mat image;
	Mat col = Mat::zeros(frame.rows, 1, CV_8UC3);
	Mat row = Mat::zeros(1, frame.cols+1, CV_8UC3);
	hconcat(col, frame, image);
	vconcat(row, image, image);

	for (ch = 0; ch < 3 ; ch++) {
		for (i = 1; i < image.cols; i++) {
			for (n = 1; n < image.rows; n++) {
				// Primitive = previous pixel value
				prim = image.at<Vec3b>(i, n-1).val[ch];
				// image.at<Vec3b>(i,n) -> Retrieve pixel at location (i, n)
				// .val[ch] -> Retrieve color value from pixel [b,g,r]
				colorVal = image.at<Vec3b>(i, n).val[ch];
				// Error = estimate - real value
				err = prim - colorVal;
				cout << "Encoding pixel... (" << i << ", " << n << ", " << ch << "): " << err << endl;
				g.encode(err);
				//cout << "(" << prim << ",";
				//cout << colorVal << ",";
				//cout << err << "); ";
			}
			//cout << endl;
		}
	}
}

int main() {
	string filename = "Resources/ducks_take_off_1080p50.y4m";
	VideoCapture video = VideoCapture(filename);
	Mat frame;
	
	Golomb g("encoded.bin", 2, int(video.get(CAP_PROP_FRAME_WIDTH)), int(video.get(CAP_PROP_FRAME_HEIGHT)), int(video.get(CAP_PROP_FRAME_COUNT)));

	while (true) {
		video >> frame;
		encodeFrame(frame, g);
		if (frame.empty()) {destroyAllWindows();break;}
	}
}