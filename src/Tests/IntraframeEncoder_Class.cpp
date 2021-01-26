/** \file	IntraframeEncoder_Class.cpp
 *	\brief	Intraframe encoder which utilizes the IntraframeEncoder classe
 *	
 *	File used for showcasing intraframe encoding class, both encode and decode
 *	methods are used, the encoding time is shown after the entire video is encoded.
 */
#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include "Encoder.h"
#include <iostream>
#include <getopt.h>
#include "FormatConverter.h"

using namespace cv;

/** \fn encode
 *	\brief Method used for encoding a video using Intraframe Encoding.
 *	The chosen video is encoded and written to the file "IntraframeEncoded.bin".
 *	All necessary information for decoding is stored in the encoded file.
 *	
 *	\param video_path Path to the video that will be encoded
 *	\param format Video format in which the video will be encoded, default is YUV420
 *	\param shift Shift value used for lossy encoding, default is 0 (Lossless)
 *	
 *	\return Path to the encoded file.
 */
string encode(string video_path, int format=3, int shift=0) {
	VideoCapture video = VideoCapture(video_path);
	string encoded_path = "IntraframeEncoded.bin";
	GolombEncoder enc(encoded_path);
	IntraEncoder intra_enc(&enc, 8, shift);
	FormatConverter conv;

	Mat frame;
	Mat aux_frame;

	enc.encode(format);
	enc.encode(8);
	enc.encode(shift);
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

/** \fn decode
 *	\brief Method used for decoding a file encoded using Intraframe Encoding.
 *	The chosen video is read from the file and decoded frame by frame, showing the decoded frame.
 *	All necessary information for decoding is read from the encoded file.
 *	
 *	\param encoded_path Path to the encoded file
 */
void decode(string encoded_path) {
	GolombDecoder dec(encoded_path);
	FormatConverter conv;

	int format = dec.decode();
	int predictor = dec.decode();
	int shift = dec.decode();
	int n_frames = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	cout << format << endl;

	Mat frame;
	IntraDecoder intra_dec(&dec, predictor, shift);
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

/**	\fn main
 *	\brief Main function used to encode and decode a video.
 *	The chosen video is encoded using the default parameters
 *	of the encode() method, after fully encoded the elapsed time is shown.
 *	The encoded file is then decoded.
 */
int main() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();	
	string encoded = encode("Resources/ducks50.y4m");
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s" << endl;

	decode(encoded);
}
