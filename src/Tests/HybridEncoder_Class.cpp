/*! \file	HybridEncoder_Class.cpp
 *	\brief	Hybrid encoder which utilizes the HybridEncoder and HybridDecoder classes
 *	
 *	File used for showcasing hybrid encoding classes, encodes a video storing it in the output file
 *	so that it can then be decoded.
 */
#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <chrono>
#include <iostream>

int main() {
	string output = "Encoded.bin";
	VideoCapture video = VideoCapture("Resources/old_town_cross_420_720p50.y4m");

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// Encode video
	HybridEncoder enc(video, "yuv420", 0);
	enc.encode(output);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s" << endl;

	// Decode encoded file
	HybridDecoder dec(output);
	dec.decode();
}
