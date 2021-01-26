/** \file	HybridEncoder_Class.cpp
 *	\brief	Hybrid encoder which utilizes the HybridEncoder and HybridDecoder classes
 *	
 *	File used for showcasing hybrid encoding classes, both encode and decode
 *	methods are used, the encoding time is shown after the entire video is encoded.
 */
#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <chrono>
#include <iostream>

/**	\fn main
 *	\brief Main function used to demonstrate HybridEncoder methods.
 *	Both the output file and video are set, the video is then encoded
 *	using default values for video type and quantization.
 *	After the encoding is complete the elapsed time is printed (in seconds) 
 *	and the decoding begins.
 */
int main() {
	string output = "Encoded.bin";
	VideoCapture video = VideoCapture("Resources/akiyo_cif.y4m");

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// Encode video
	HybridEncoder enc(video, "yuv420", 3);
	enc.encode(output);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s" << endl;

	// Decode encoded file
	HybridDecoder dec(output);
	dec.decode();
}
