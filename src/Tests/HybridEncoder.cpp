/*! \file	HybridEncoder.cpp
 *	\brief	Encoder which utilizes both interframe and intraframe encoding based on Golomb codes.
 *	
 *	File used to demonstrate a hybrid encoder which encodes a video file, storing it on the computer
	so that it can later decoded. Also demonstrates decoding, showing the decoded frames while decoding.
 *
 */

#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include <iostream>
#include <getopt.h>


using namespace cv;
using namespace std;
/** \fn 	interEncoder
 * 	\brief 	Encode a frame using interframe encoding (Motion Compensation), based on Golomb codes.
 * 
 * 	\param	old_frame 	Previous frame.
 * 	\param	curr_frame	Current frame which will be encoded.
 * 	\param	g	GolombEncoder instance that is being used to encode the frames.
 * 	\param	block_dim	Block size.
 * 	\param	range	Range in which blocks will be searched for in the previous frame.
 */
void interEncoder(Mat old_frame, Mat curr_frame, GolombEncoder &enc, int block_dim, int range) {
	/** Current frame's block. */
	Mat curr_block;
	/** Previous frame's block. */
	Mat old_block;
	/** Block with error values. */
	Mat block_diff;
	/** Block with error values which minizes min_diff. */
	Mat min_block_diff;

	/** Sum of matrix that corresponds to the minimum difference between current frame's block
	 *	and previous frame's block. */
	int min_diff;
	/** x coordinate of previous frame's block which minimizes error. */
	int min_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int min_y;
	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_dim);
	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_dim);
	
	/** Iterate through current frame's blocks. */
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_dim) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_dim) {
			min_diff = 100000;

			/** Get current block. */
			curr_block = Mat(curr_frame, Rect(curr_x, curr_y, block_dim, block_dim));

			/** Iterate through previous frame's blocks */
			for (int old_y = ( (curr_y-range < 0) ? 0 : (curr_y-range) );
				old_y < ( (curr_y+range >= max_y) ? max_y : (curr_y+range) ); old_y++) {
					for (int old_x = ( (curr_x-range < 0) ? 0 : (curr_x-range) );
						old_x < ( (curr_x+range >= max_x) ? max_x : (curr_x+range) ); old_x++) {
							
							old_block = Mat(old_frame, Rect(old_x, old_y, block_dim, block_dim));
							block_diff = curr_block - old_block;

							if (sum(sum(block_diff))[0] < min_diff) {
								min_x = old_x;
								min_y = old_y;
								block_diff.copyTo(min_block_diff);
								min_diff = abs(sum(sum(min_block_diff))[0]);
								/** If the difference between blocks is 0, no need to keep searching. */
								if (min_diff==0)
									break;
							}
					}
					/** If the difference between blocks is 0, no need to keep searching. */
					if (min_diff==0)
						break;
				}

			/** Encode blocks coordinates. */
			enc.encode(min_x);
			enc.encode(min_y);

			/** Encode error between blocks. */ 
			for (int i = 0; i < block_dim; i++) {
				for (int j = 0; j < block_dim; j++) {
					for (int ch = 0; ch < 3; ch++) {
						enc.encode(min_block_diff.at<Vec3b>(i,j).val[ch]);
					}
				}
			}
		}
	}
}

/**	\fn interDecode
 *	\brief 	Decodes a frame encoded with interframe encoding.
 *			Stores the decoded data in a Mat passed to the function.
 *	
 *	\param old_frame	Previous frame decoded
 *	\param curr_frame	Reference to a current frame Mat which will be filled with the decoded data
 *	\param dec	Instance of GolombDecoder used for decoding numbers from file
 *	\param block_dim	Dimension of macro blocks used during encoding
 */
void interDecode(Mat old_frame, Mat &curr_frame, GolombDecoder &dec, int block_dim) {
	/** Previous frame's block. */
	Mat old_block;
	
	int err, pred;

	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_dim);

	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_dim);

	/** x coordinate of previous frame's block which minimizes error. */
	int old_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int old_y;
	
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_dim) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_dim) {
			//cout << "A(" << curr_x << ", " << curr_y << ")" << endl;
			old_x = dec.decode();
			old_y = dec.decode();

			//old_block = Mat(old_frame, Rect(old_x, old_y, block_dim, block_dim));

			for (int i = curr_y; i < curr_y+block_dim; i++, old_y++) {
				for (int j = curr_x; j < curr_x+block_dim; j++, old_x++) {
					//cout << "(" << i << ", " << j << ")" << endl;
					//cout << "(" << old_x << ", " << old_y << ")" << endl;
					//cout << endl;
					for (int ch = 0; ch < 3; ch++) {
						err = dec.decode();
						pred = old_frame.at<Vec3b>(old_y, old_x).val[ch];
						curr_frame.at<Vec3b>(i,j).val[ch] =	pred + err;
					}
				}
				old_x-=block_dim;
			}

		}
	}
}

/** \fn 	intraEncoder
 *	\brief 	Encode a frame using intraframe predictive coding, based on Golomb codes.
 *	
 *	\param frame 	Frame that will be encoded.
 *	\param g 	Golomb encoder object.
 *	\param predictor 	Predictor type from 1-8, 8 is Jpeg-LS.
 */
void intraEncoder(Mat frame, GolombEncoder &enc, int predictor) {
	/** Predictor value. */
	int pred;
	/** Previous pixel color value. */
	int a;
	/** Above pixel color value. */
	int b;
	/** Diagonally previous color value. */
	int c;
	/** min(a, b) */
	int mini;
	/** max(a, b) */
	int maxi;

	/** Matrix where the padded frame is stored. */
	Mat image;
	hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
	vconcat(Mat::zeros(1, frame.cols+1, CV_8UC3), image, image);

	/** Frame data pointer. */
	unsigned char *pFrameData = (unsigned char*)(image.data);

	/** Iterate through height, width and channel. */
	for (int i = 1; i < image.rows; i++) {
		for (int n = 1; n < image.cols; n++) {
			for (int ch = 0; ch < 3 ; ch++) {
				/** Retrieve values from frame. */
				a = pFrameData[image.channels() * (image.cols * i + (n-1)) + ch];
				b = pFrameData[image.channels() * (image.cols * (i-1) + n) + ch];
				c = pFrameData[image.channels() * (image.cols * (i-1) + (n-1)) + ch];
				//a = image.at<Vec3b>(i, n-1).val[ch];
				//b = image.at<Vec3b>(i-1, n).val[ch];
				//c = image.at<Vec3b>(i-1, n-1).val[ch];

				switch (predictor) {
					case 1:
						pred = a;
						break;
					case 2:
						pred = b;
						break;
					case 3:
						pred = c;
						break;
					case 4:
						pred = a + b - c;
						break;
					case 5:
						pred = a + (b - c) / 2;
						break;
					case 6:
						pred = b + (a - c) / 2;
						break;
					case 7:
						pred = (a + b) / 2;
						break;
					case 8:
						pred = a+b-c;
						
						mini = min(a, b);
						maxi = max(a, b);

						if (c >= maxi) {
							pred = mini;
							break;
						}
						if (c <= mini)
							pred = maxi;
						break;
					default:
						cerr << "Invalid predictor mode." << endl;
						return;
				}

				/** Encode Error = estimate - real value. */
				enc.encode(int(pFrameData[image.channels() * (image.cols * i + n) + ch]) - pred);
			}
		}
	}

}

/**	\fn	intraDecode
 * 	\brief	Decodes a frame previously encoded using intraframe encoding.
 * 			Stores the decoded data on a Mat passed to the function.
 * 
 * 	\param frame	Mat where the decoded data is stored.
 * 	\param dec	GolombDecoder instance used for decoding numbers from file.
 * 	\param predictor	Predictor mode used during encoding.
 */
void intraDecode(Mat &frame, GolombDecoder &dec, int predictor) {
	int err;
	unsigned char a, b, c, pred, mini, maxi;

	unsigned char *pFrameData = (unsigned char*)(frame.data);

	for (int i = 0; i < frame.rows; i++) {
		for (int n = 0; n < frame.cols; n++) {
			for (int ch = 0; ch < 3; ch++){

				if (n == 0)
					a = 0;
				else
					a = pFrameData[frame.channels() * (frame.cols * i + (n-1)) + ch];
				
				if (i == 0)
					b = 0;
				else
					b = pFrameData[frame.channels() * (frame.cols * (i-1) + n) + ch];
				
				if (i == 0 || n == 0)
					c = 0;
				else
					c = pFrameData[frame.channels() * (frame.cols * (i-1) + (n-1)) + ch];
				
				switch (predictor) {
					case 1:
						pred = a;
						break;
					case 2:
						pred = b;
						break;
					case 3:
						pred = c;
						break;
					case 4:
						pred = a + b - c;
						break;
					case 5:
						pred = a + (b - c) / 2;
						break;
					case 6:
						pred = b + (a - c) / 2;
						break;
					case 7:
						pred = (a + b)/2;
						break;
					case 8:
						pred = a+b-c;

						mini = min(a, b);
						maxi = max(a, b);

						if (c >= maxi) {
							pred = mini;
							break;
						}
						if (c <= mini)
							pred = maxi;
						break;
				}

				err = dec.decode();
				pFrameData[frame.channels() * (frame.cols * i + n) + ch] = pred + err;
				//frame.at<Vec3b>(i, n).val[ch] = pred+err;
			}
		}
	}
}


/**	\fn main
 * 	\brief	Encodes and decodes a video file.
 * 			Encodes the video, writing it in dest_file so it can later be decoded.
 * 			All information needed for decoding is stored in the file.
 * 			Only information that the decoder needs is the encoded file path.
 */
int main() {
	String input_file = "Resources/bgrVideo.avi";
	String dest_file = "Encoded.bin";
	
	// /** Arbitrary values. */
	// int m = 4;
	// int predictor = 8;
	// int block_size = 16;
	// int block_range = 4;
	// int period = 5;
	
	// /** Video that will be decoded. */
	// VideoCapture video = VideoCapture(input_file);
	// Mat old_frame, curr_frame;

	// /** Object used to encode frames, path to encoded file and value m are defined. */
	// GolombEncoder enc(dest_file, m);

	// /** Encode header values to use during decoding. */
	// enc.encode(predictor);
	// enc.encode(video.get(CAP_PROP_FRAME_WIDTH));
	// enc.encode(video.get(CAP_PROP_FRAME_HEIGHT));
	// enc.encode(video.get(CAP_PROP_FRAME_COUNT));
	// enc.encode(block_size);
	// enc.encode(block_range);
	// enc.encode(period);

	// //cout << predictor << endl;
	// //cout << video.get(CAP_PROP_FRAME_WIDTH) << endl;
	// //cout << video.get(CAP_PROP_FRAME_HEIGHT) << endl;
	// //cout << 10 << endl;
	// //cout << block_size << endl;
	// //cout << block_range << endl;

	// int count = 0;
	// while (true) {
		
	// 	video >> curr_frame;
	// 	if (curr_frame.empty()) {break;};
	// 	if (count%period == 0){
	// 		intraEncoder(curr_frame, enc, predictor);
	// 		curr_frame.copyTo(old_frame);
	// 	}
	// 	else {
	// 		interEncoder(old_frame, curr_frame, enc, block_size, block_range);
	// 	}
	// 	cout << "Encoded frame " << count << endl;
	// 	count++;
	// }

	// enc.finishEncoding();

   	/********************
	 *  Begin Decoding  *
	 ********************/
	GolombDecoder dec(dest_file);
	Mat dec_curr_frame, dec_old_frame;

	int dec_predictor = dec.decode();
	int dec_width = dec.decode();
	int dec_height = dec.decode();
	int dec_n_frames = dec.decode();
	int dec_block_size = dec.decode();
	int dec_block_range = dec.decode();
	int dec_period = dec.decode();

	//cout << dec_predictor << endl;
	//cout << dec_width << endl;
	//cout << dec_height << endl;
	//cout << dec_n_frames << endl;
	//cout << dec_block_size << endl;
	//cout << dec_block_range << endl;

	for (int n = 0; n < dec_n_frames; n++) {
		cout << "Decoding frame " << n+1 << endl;
		dec_curr_frame = Mat::zeros(dec_height, dec_width, CV_8UC3);
		if (n%dec_period==0){
			intraDecode(dec_curr_frame, dec, dec_predictor);
			dec_curr_frame.copyTo(dec_old_frame);
		}
		else {
			interDecode(dec_old_frame, dec_curr_frame, dec, dec_block_size);
		}
		imshow("Image", dec_curr_frame);
		if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
	}

}