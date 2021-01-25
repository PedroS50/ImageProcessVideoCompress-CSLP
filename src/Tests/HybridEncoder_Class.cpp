/*! \file	HybridEncoder_Class.cpp
 *	\brief	Hybrid encoder which utilizes the HybridEncoder and HybridDecoder classes
 *	
 *	File used for showcasing hybrid encoding classes, encodes a video storing it in the output file
 *	so that it can then be decoded.
 */
#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>

int main() {
	string output = "Encoded.bin";
	VideoCapture video = VideoCapture("Resources/ducks50.y4m");

	// Encode video
	HybridEncoder enc(video, "yuv422");
	enc.encode(output);

	// Decode encoded file
	HybridDecoder dec(output);
	dec.decode();
}
