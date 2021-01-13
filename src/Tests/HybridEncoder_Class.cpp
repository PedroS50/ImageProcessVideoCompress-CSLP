#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>

int main() {
	string output = "Encoded.bin";
	VideoCapture video = VideoCapture("Resources/bgrVideo.avi");

	// Encode video
	HybridEncoder enc(video);
	enc.encode(output);

	// Decode encoded file
	HybridDecoder dec(output);
	dec.decode();
}
