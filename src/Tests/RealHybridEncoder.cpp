#include "opencv2/opencv.hpp"
#include "Encoder.h"
#include <iostream>

int main() {
	string video_path = "Resources/bgrVideo.avi";
	VideoCapture video = VideoCapture(video_path);

	HybridEncoder h_enc(video, video_path);
	h_enc.encode();
}