#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>
#include <getopt.h>

string encode(string video_path) {
	string encoded_path = "Encoded.bin";
	VideoCapture video = VideoCapture(video_path);

	GolombEncoder enc(encoded_path);
	InterEncoder inter_enc(&enc, 32, 5);
	IntraEncoder intra_enc(&enc, 8);

	Mat curr_frame;
	Mat old_frame;

	enc.encode(8);
	enc.encode(32);
	enc.encode(10);
	enc.encode(video.get(CAP_PROP_FRAME_WIDTH));
	enc.encode(video.get(CAP_PROP_FRAME_HEIGHT));
	enc.encode(video.get(CAP_PROP_FRAME_COUNT));

	int count = 0;
	while (true) {
		video >> curr_frame;
		if (curr_frame.empty()) {break;};
		if ( count==0 ) {
			curr_frame.copyTo(old_frame);

			intra_enc.encode(curr_frame);
			
			cout << "Frame " << count++ << " encoded." << endl;
		} else {
			inter_enc.encode(old_frame, curr_frame);

			cout << "Frame " << count++ << " encoded." << endl;
		}
	}

	enc.finishEncoding();
	return encoded_path;
}

void decode(string encoded_path) {
	GolombDecoder dec(encoded_path);

	int predictor = dec.decode();
	int block_size = dec.decode();
	int block_range = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	int n_frames = dec.decode();

	Mat curr_frame;
	Mat old_frame;
	IntraDecoder intra_dec(&dec, predictor);
	InterDecoder inter_dec(&dec, block_size, block_range);

	int count = 1;
	while (count <= n_frames) {
		curr_frame = Mat::zeros(height, width, CV_8UC3);
		if ( count==1 ) {
			intra_dec.decode(curr_frame);
			curr_frame.copyTo(old_frame);
		} else {
			inter_dec.decode(old_frame, curr_frame);			
		}
		imshow("Image", curr_frame);
		if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
		cout << "Decoded frame " << count++ << endl;
	}
}

int main() {
	/*
	Mat testMat = Mat::zeros(2, 3, CV_8UC3);
	Mat testMat2 = Mat::zeros(2, 3, CV_8UC3);
	Mat testMat3 = Mat::zeros(2, 3, CV_8UC3);
	int data[18] = {1,2,2,3,2,2, 4,3,2,4,5,1, 4,1,2,3,4,6};
	int data2[18] = {2,3,3,4,3,3, 5,4,3,5,6,2, 5,2,3,4,5,7};
	int count = 0;
	for (int ch = 0; ch < 3; ch++) {
		for (int i = 0; i < 2; i++) {
			for (int n = 0; n < 3; n++) {
				testMat.at<Vec3b>(i, n).val[ch] = data[count];
				testMat2.at<Vec3b>(i, n).val[ch] = data2[count];
				count++;
			}
		}
	}
	cout << testMat << endl;
	cout << testMat2 << endl;
	for (int i=0; i<testMat.rows; i++)
		for (int n=0; n<testMat.cols; n++)
			for (int ch=0; ch<3; ch++)
				testMat3.at<Vec3b>(i,n).val[ch] = testMat2.at<Vec3b>(i,n).val[ch] - testMat.at<Vec3b>(i,n).val[ch];
	cout << testMat3 << endl;
	*/
	string encoded = encode("Resources/bgrVideo.avi");
	decode(encoded);
}
