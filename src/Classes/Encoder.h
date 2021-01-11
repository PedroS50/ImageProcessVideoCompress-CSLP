#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

using namespace cv;
using namespace std;

class IntraEncoder {
private:
	int predictor;
	GolombEncoder *enc;

public:
	IntraEncoder(GolombEncoder *enc, int predictor);

	IntraEncoder() = default;

	int get_predictor();

	/** \fn 	intraEncoder
	 *	\brief 	Encode a frame using intraframe predictive coding, based on Golomb codes.
	 *	
	 *	\param frame 	Frame that will be encoded.
	 */
	void encode(Mat &frame);
};

class IntraDecoder {
private:
	int predictor;
	GolombDecoder *dec;

public:
	IntraDecoder(GolombDecoder *dec, int predictor);

	IntraDecoder() = default;

	/**	\fn	intraDecode
	 * 	\brief	Decodes a frame previously encoded using intraframe encoding.
	 * 			Stores the decoded data on a Mat passed to the function.
	 * 
	 * 	\param frame	Mat where the decoded data is stored.
	 */
	void decode(Mat &frame);
};

class InterEncoder {
private:
	int block_size;
	int block_range;
	GolombEncoder *enc;

public:
	InterEncoder(GolombEncoder *enc, int block_size, int block_range);

	InterEncoder() = default;

	int get_block_size();

	int get_block_range();

	void encode(Mat old_frame, Mat curr_frame);

};

class InterDecoder {
private:
	int block_size;
	int block_range;
	GolombDecoder *dec;

public:
	InterDecoder(GolombDecoder *dec, int block_size, int block_range);

	InterDecoder() = default;

	void decode(Mat old_frame, Mat &curr_frame);

};

class HybridEncoder {
private:
	VideoCapture video;
	int width;
	int height;
	int n_frames;
	int predictor;
	int block_size;
	int block_range;

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