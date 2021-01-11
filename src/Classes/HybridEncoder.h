#ifndef HYBRIDENCODER_H
#define HYBRIDENCODER_H

#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"

using namespace std;
using namespace cv;

class HybridEncoder {
private:
	GolombEncoder *enc;
	IntraEncoder intra_enc;
	InterEncoder inter_enc;
	VideoCapture video;
	int width;
	int height;
	int n_frames;

public:
	HybridEncoder(VideoCapture video, string video_name);

	void encode();
};

class HybridDecoder {
private:
	GolombDecoder *dec;
	IntraDecoder intra_dec;
	InterDecoder inter_dec;
	int width;
	int height;
	int n_frames;

public:
	HybridDecoder(string input_file);

	void decode();
};

#endif