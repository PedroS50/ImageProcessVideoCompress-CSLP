#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>
#include <getopt.h>

using namespace cv;

string encode(string video_path) {
	VideoCapture video = VideoCapture(video_path);
	string encoded_path = "Encoded.bin";
	GolombEncoder enc(encoded_path);
	IntraEncoder intra_enc(&enc, 8);

	Mat frame;
	Mat aux_frame;

	enc.encode(8);
	enc.encode(video.get(CAP_PROP_FRAME_WIDTH));
	enc.encode(video.get(CAP_PROP_FRAME_HEIGHT));
	enc.encode(video.get(CAP_PROP_FRAME_COUNT));

	int count = 0;
	while (true) {
		video >> frame;
		if (frame.empty()) {break;};
		
		intra_enc.encode(frame);
		
		count++;
		cout << "Frame " << count << " encoded." << endl;
	}

	enc.finishEncoding();
	return encoded_path;
}

void decode(string encoded_path) {
	GolombDecoder dec(encoded_path);

	int predictor = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	int n_frames = dec.decode();

	Mat frame;
	IntraDecoder intra_dec(&dec, predictor);

	while (n_frames > 0) {
		frame = Mat::zeros(height, width, CV_8UC3);
		intra_dec.decode(frame);
		
		imshow("Image", frame);
		if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
		
		cout << "Decoded frame " << n_frames-- << endl;
	}

}

int main() {
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
	
	
	//string encoded = encode("Resources/bgrVideo.avi");
	//decode(encoded);
}
