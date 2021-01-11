#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>
#include <getopt.h>

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
	cout << "Vou sair"<< endl;
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
	string encoded = encode("Resources/bgrVideo.avi");
	cout << "Fudi-me" << endl;
	decode(encoded);
}
