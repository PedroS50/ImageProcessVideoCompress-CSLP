#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void convertPixel(double y, double u, double v, unsigned char &r, unsigned char &g, unsigned char &b) {
	b = y + 2.032*u;
	g = y - 0.395*u - 0.581*v;
	r = y + 1.140*v;
	//r = y + (1.4065 * (v - 128));
    //g = y - (0.3455 * (u - 128)) - (0.7169 * (v - 128));
    //b = y + (1.7790 * (u - 128));

}

Mat yuvTorgb(Mat frame) {
	Mat channels[3];

	split(frame, channels);

	int height = frame.rows;
	int width = frame.cols;
	//double y, u, v;
	int y, u, v;
	double r, g, b;


	Mat result = Mat::zeros(frame.size(), CV_8UC3);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			//cout << "(" << row << ", " << col << ")" << endl;
			cout << channels[0] << endl;
			return result;
			y = channels[0].at<int>(row, col);
			v = channels[1].at<int>(row, col);
			u = channels[2].at<int>(row, col);
			//convertPixel(y, u, v, r, g, b);

			b = y + 2.032*u;
			g = y - 0.395*u - 0.581*v;
			r = y + 1.140*v;

			if (r < 0) r = 0;
			else if (r > 255) r = 255;
			if (g < 0) g = 0;
			else if (g > 255) g = 255;
			if (b < 0) b = 0;
			
			result.at<Vec3b>(row, col).val[0] = r;
			result.at<Vec3b>(row, col).val[1] = g;
			result.at<Vec3b>(row, col).val[2] = b;
				
			//result.at<Vec3b>(row, col).val[2] = (r < 0) ? 0 : (r > 255) ? 255 : r;
			//result.at<Vec3b>(row, col).val[1] = (g < 0) ? 0 : (g > 255) ? 255 : g;
			//result.at<Vec3b>(row, col).val[0] = (b < 0) ? 0 : (b > 255) ? 255 : b;
			//cout << "(" << int(result.at<Vec3b>(row, col).val[2]) << ", " << int(result.at<Vec3b>(row, col).val[1]) << ", " << int(result.at<Vec3b>(row, col).val[0]) << ")" << endl; 
		}
	}

	return result;
}

int main() {
	//int width = 352, height = 288, n_frames = 300;
	//string filename = "Resources/akiyo_cif.yuv";
	//fstream fp(filename, fstream::in | fstream::binary);
	//unsigned char y, u, v;

	//while (true) {
		//cout << fp.read(width*height) << endl;
	//}
	return 0;

}