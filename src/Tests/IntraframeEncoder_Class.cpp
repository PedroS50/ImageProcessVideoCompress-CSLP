#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>
#include <getopt.h>
#include "FormatConverter.h"

using namespace cv;

string encode(string video_path, int format=3) {
	VideoCapture video = VideoCapture(video_path);
	string encoded_path = "Encoded.bin";
	GolombEncoder enc(encoded_path);
	IntraEncoder intra_enc(&enc, 8);
	FormatConverter conv;

	Mat frame;
	Mat aux_frame;

	enc.encode(format);
	enc.encode(8);
	enc.encode(video.get(CAP_PROP_FRAME_COUNT));

	int count = 0;
	switch (format) {
		case 0: {
			while (true) {
				video >> frame;
				if (frame.empty()) {break;};

				if (count==0) {
					enc.encode(frame.cols);
					enc.encode(frame.rows);
				}

				intra_enc.encode(frame);

				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 1: {
			while (true) {
				video >> frame;
				if (frame.empty()) {break;};
				frame = conv.rgb_to_yuv444(frame);

				if (count==0) {
					enc.encode(frame.cols);
					enc.encode(frame.rows);
				}

				intra_enc.encode(frame);

				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 2: {
			while (true) {
				video >> frame;
				if (frame.empty()) {break;};
				frame = conv.rgb_to_yuv422(frame);

				if (count==0) {
					enc.encode(frame.cols);
					enc.encode(frame.rows);
				}

				intra_enc.encode(frame);

				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 3: {
			while (true) {
				video >> frame;
				if (frame.empty()) {break;};
				frame = conv.rgb_to_yuv420(frame);

				if (count==0) {
					enc.encode(frame.cols);
					enc.encode(frame.rows);
				}

				intra_enc.encode(frame);

				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
	}
	
	enc.finishEncoding();
	return encoded_path;
}

void decode(string encoded_path) {
	GolombDecoder dec(encoded_path);
	FormatConverter conv;

	int format = dec.decode();
	int predictor = dec.decode();
	int n_frames = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	cout << format << endl;

	Mat frame;
	IntraDecoder intra_dec(&dec, predictor);
	switch (format) {
		case 0: {
			while (n_frames > 0) {
				frame = Mat::zeros(height, width, CV_8UC3);
				intra_dec.decode(frame);
				
				imshow("Image", frame);
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				
				cout << "Decoded frame " << n_frames-- << endl;
			}
			break;
		}
		case 1: {
			while (n_frames > 0) {
				frame = Mat::zeros(height, width, CV_8UC3);
				intra_dec.decode(frame);

				imshow("Image", conv.yuv444_to_rgb(frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				
				cout << "Decoded frame " << n_frames-- << endl;
			}
			break;
		}
		case 2: {
			while (n_frames > 0) {
				frame = Mat::zeros(height, width, CV_8UC1);
				intra_dec.decode(frame);
				
				imshow("Image", conv.yuv422_to_rgb(frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				
				cout << "Decoded frame " << n_frames-- << endl;
			}
			break;
		}
		case 3: {
			while (n_frames > 0) {
				frame = Mat::zeros(height, width, CV_8UC1);
				intra_dec.decode(frame);
				
				imshow("Image", conv.yuv420_to_rgb(frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				
				cout << "Decoded frame " << n_frames-- << endl;
			}
			break;
		}
	}
	

}

int main() {
	/*
	float data[27] = {1,1,2,3,4,5,6,7,8, 1,2,34,5,6,1,6,7,7 ,7,8,9,4,5,6,1,2,3};
	Mat testMat = Mat::zeros(3, 3, CV_32FC3);
	int count = 0;
	
	for (int ch = 0; ch < 3; ch++) {
		for (int i = 0; i < 3; i++) {
			for (int n = 0; n < 3; n++) {
				testMat.at<Vec3f>(i, n).val[ch] = data[count];
				count++;
			}
		}
	}
	cout << testMat << endl;
	int histSize[3] = {32, 32, 32};
	float range[] = { -255, 256 }; //the upper boundary is exclusive
	int channels[3] = {0, 1, 2};
	Mat hist;
	const float* histRange[] = { range, range, range };
	calcHist(&testMat, 1, channels, Mat(), hist, 3, histSize, histRange, true, false);
	
	for (MatConstIterator_<double> it = hist.begin<double>(); it != hist.end<double>(); it++) {
            cout << "Value: " << *it << "\n";
        }
	*/
	
	
	string encoded = encode("Resources/akiyo_cif.y4m");
	decode(encoded);
}
