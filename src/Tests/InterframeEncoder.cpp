#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include <iostream>

using namespace cv;
using namespace std;

void interEncoder(Mat frame, GolombEncoder &g, int block_dim, int range) {
	Mat curr_block, old_block, block_diff, min_block_diff;

	int min_diff, min_x, min_y;
	int max_x = (frame.cols - block_dim + 1), max_y = (frame.rows - block_dim + 1);
	
	for (int curr_y = 0; curr_y < max_y; curr_y += block_dim) {
		for (int curr_x = 0; curr_x < max_x; curr_x += block_dim) {
			min_diff = 100000;
			curr_block = Mat(frame, Rect(curr_x, curr_y, block_dim, block_dim));
			cout << curr_block << endl;

			for (int old_y = ( (curr_y-range < 0) ? 0 : curr_y-range );
				old_y < ( (curr_y+range > max_y) ? max_y : curr_y+range ); old_y++) {
					cout << "old_y = " << old_y << endl; 
					for (int old_x = ( (curr_x-range < 0) ? 0 : curr_x-range );
						old_x < ( (curr_x+range > max_x) ? max_x : curr_x+range ); old_x++) {
							
							old_block = Mat(frame, Rect(old_x, old_y, block_dim, block_dim));
							block_diff = curr_block - old_block;

							if (sum(sum(block_diff))[0] < min_diff) {
								min_x = old_x;
								min_y = old_y;
								min_block_diff = block_diff.clone();
								min_diff = sum(sum(min_block_diff))[0];
							}
					}
				}

			//cout << "(" << min_x << ", " << min_y << ")" << endl;
			//cout << min_block_diff << endl;
			//cout << "Sum: " << min_diff << endl;
			//cout << endl;
			// Encode blocks coordinates
			g.encode(min_x);
			g.encode(min_y);
			// Encode difference between 
			for (int i = 0; i < min_block_diff.rows; i++) {
				for (int j = 0; j < min_block_diff.cols; j++) {
					for (int ch = 0; ch < 3; ch++) {
						g.encode(min_block_diff.at<Vec3b>(i,j).val[ch]);
					}
				}
			}
		}
	}
}

int main() {
	String file_name = "akiyo_cif.yuv";
	fstream fp = fstream(file_name, fstream::in );
	char c;
	Mat frame = Mat::zeros(352, 288, CV_8UC3);
	for (int i = 0; i < frame.rows; i++) {
		for (int j = 0; j < frame.cols; j++) {
			fp.read(reinterpret_cast<char*>(&c), sizeof(char));
			frame.at<Vec3b>(i, j).val[0] = c;
			fp.read(reinterpret_cast<char*>(&c), sizeof(char));
			frame.at<Vec3b>(i, j).val[1] = c;
			fp.read(reinterpret_cast<char*>(&c), sizeof(char));
			frame.at<Vec3b>(i, j).val[2] = c;
		}
	}
	imshow("Whatever", frame);
	if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window


	/*************************************************************
	 * Tests 
	Mat testMat = Mat::zeros(4, 12, CV_8UC1);
	//int data[36] = {1,2,2,3,2,1,1,1,2, 4,3,2,4,5,1,2,4,5,1, 4,1,2,3,4,6,1,2,2,1,1, 3,2,3,1,3,4};
	
	int data[50] = {1,2,3,4,5,6,7,8,9,10,11,15, 
					10,9,8,7,6,5,4,3,2,1,14,63,
					11,1,2,34,5,166,1,2,3,11,23,45,
					11,10,20,4,50,112,10,20,30,110,76,111};
	
	int count = 0;
	count = 0;
	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 12; n++) {
			testMat.at<uchar>(i, n) = data[count];
			count++;
		}
	}

	//cout << testMat << endl;

	//interEncoder(testMat, 2, 2);
	
	*************************************************************/
}