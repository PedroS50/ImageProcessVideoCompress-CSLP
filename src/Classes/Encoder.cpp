#include "Encoder.h"
#include "FormatConverter.h"
#include <unistd.h>

IntraEncoder::IntraEncoder(GolombEncoder *enc, int predictor) {
	this->predictor = predictor;
	this->enc = enc;
}

int IntraEncoder::get_predictor() {
	return predictor;
}

int IntraEncoder::calc_predictor(int a, int b, int c) {
	switch (predictor) {
		case 1:
			return a;
		case 2:
			return b;
		case 3:
			return c;
		case 4:
			return a + b - c;
		case 5:
			return a + (b - c) / 2;
		case 6:
			return b + (a - c) / 2;
		case 7:
			return (a + b) / 2;
		case 8: {
			int pred = a+b-c;

			int mini = min(a, b);
			int maxi = max(a, b);

			if (c >= maxi) {
				pred = mini;
			}
			if (c <= mini)
				pred = maxi;

			return pred;
		}
		default:
			cerr << "Invalid predictor mode." << endl;
			return 0;
	}
}

void IntraEncoder::encode(cv::Mat &frame) {
	/** Previous pixel color value. */
	int a;
	/** Above pixel color value. */
	int b;
	/** Diagonally previous color value. */
	int c;

	int size = frame.rows*frame.cols*frame.channels();

	int mEnc;

	// Matrix where the padded frame is stored.
	Mat image;
	Mat aux_frame;
	if (frame.type()==CV_8UC3) {
		hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
		vconcat(Mat::zeros(1, frame.cols+1, CV_8UC3), image, image);
		aux_frame = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
	} else if (frame.type()==CV_8UC1) {
		hconcat(Mat::zeros(frame.rows, 1, CV_8UC1), frame, image);
		vconcat(Mat::zeros(1, frame.cols+1, CV_8UC1), image, image);
		aux_frame = Mat::zeros(frame.rows, frame.cols, CV_16SC1);
	}
	/** Frame data pointer. */
	//unsigned char *pFrameData = (unsigned char*)(image.data);
	//short *pAuxFrameData = (short*)(aux_frame.data);
	// Iterate through height, width and channel.
	for (int i = 1; i < image.rows; i++) {
		for (int n = 1; n < image.cols; n++) {
			for (int ch = 0; ch < frame.channels() ; ch++) {
				// Retrieve values from frame.
				a = image.ptr<uchar>(i, n-1)[ch];
				b = image.ptr<uchar>(i-1, n)[ch];
				c = image.ptr<uchar>(i-1, n-1)[ch];
				//a = pFrameData[image.channels() * (image.cols * i + (n-1)) + ch];
				//b = pFrameData[image.channels() * (image.cols * (i-1) + n) + ch];
				//c = pFrameData[image.channels() * (image.cols * (i-1) + (n-1)) + ch];

				// Store Error = estimate - real value.
				enc->encode(image.ptr<uchar>(i, n)[ch] - this->calc_predictor(a,b,c));
				//enc->encode(pFrameData[image.channels() * (image.cols * i + (n)) + ch] - this->calc_predictor(a,b,c));
				//pAuxFrameData[aux_frame.channels() * (aux_frame.cols * (i-1) + (n-1)) + ch] = pFrameData[image.channels() * (image.cols * i + (n)) + ch] - this->calc_predictor(a,b,c);
			}
		}
	}

	// Calculate optimal m...
	//mEnc = 4;

	//if (enc->get_m() == mEnc)
	//	enc->encode(0);
	//else
	//	enc->encode(mEnc);

	//enc->set_m(mEnc);

	//for (int i = 0; i < size; i++)
	//	enc->encode( pAuxFrameData[i] );
	//for (int i = 0; i < aux_frame.rows; i++)
	//	for (int n = 0; n < aux_frame.cols; n++)
	//		for (int ch = 0; ch < 3 ; ch++)
	//			enc->encode( pAuxFrameData[aux_frame.channels() * (aux_frame.cols * i + (n)) + ch] );
}

IntraDecoder::IntraDecoder(GolombDecoder *dec, int predictor) {
	this->predictor = predictor;
	this->dec = dec;
}

int IntraDecoder::calc_predictor(int a, int b, int c) {
	switch (predictor) {
		case 1:
			return a;
		case 2:
			return b;
		case 3:
			return c;
		case 4:
			return a + b - c;
		case 5:
			return a + (b - c) / 2;
		case 6:
			return b + (a - c) / 2;
		case 7:
			return (a + b) / 2;
		case 8: {
			int pred = a+b-c;

			int mini = min(a, b);
			int maxi = max(a, b);

			if (c >= maxi) {
				pred = mini;
			}
			if (c <= mini)
				pred = maxi;

			return pred;
		}
		default:
			cerr << "Invalid predictor mode." << endl;
			return 0;
	}
}

void IntraDecoder::decode(Mat &frame) {
	int m;
	unsigned char a, b, c;
	//unsigned char *pFrameData = (unsigned char*)(frame.data);

	// Retrieve parameter m
	//m = dec->decode();
	//if (m != 0)
	//	dec->set_m(m);

	for (int i = 0; i < frame.rows; i++) {
		for (int n = 0; n < frame.cols; n++) {
			for (int ch = 0; ch < frame.channels(); ch++){

				if (n == 0)
					a = 0;
				else
					a = frame.ptr<uchar>(i, n-1)[ch];
					//a = pFrameData[frame.channels() * (frame.cols * i + (n-1)) + ch];
				
				if (i == 0)
					b = 0;
				else
					b = frame.ptr<uchar>(i-1, n)[ch];
					//b = pFrameData[frame.channels() * (frame.cols * (i-1) + n) + ch];
				
				if (i == 0 || n == 0)
					c = 0;
				else
					c = frame.ptr<uchar>(i-1, n-1)[ch];
					//c = pFrameData[frame.channels() * (frame.cols * (i-1) + (n-1)) + ch];

				frame.ptr<uchar>(i, n)[ch] = this->calc_predictor(a,b,c) + dec->decode();
				//pFrameData[frame.channels() * (frame.cols * i + n) + ch] = this->calc_predictor(a,b,c) + dec->decode();
			}
		}
	}
}

InterEncoder::InterEncoder(GolombEncoder *enc, int block_size, int block_range) {
	this->block_size = block_size;
	this->block_range = block_range;
	this->enc = enc;

}

int InterEncoder::get_block_size() {
	return block_size;
}

int InterEncoder::get_block_range() {
	return block_range;
}

float InterEncoder::cost(Mat block) {
	return abs(sum(sum(block))[0]);
}

void InterEncoder::encode(Mat old_frame, Mat curr_frame) {
	/** Block with error values. */
	Mat block_diff;
	/** Block with error values which minizes min_diff. */
	Mat min_block_diff;
	/** Frame that will store error values so they can later be processed and encoded. */
	Mat aux_frame;

	if (curr_frame.type()==CV_8UC3) {
		block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
		aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC3);
	} else if (curr_frame.type()==CV_8UC1) {
		block_diff = Mat::zeros(block_size, block_size, CV_16SC1);
		aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC1);
	}

	/** Current frame's data pointer. */
	//unsigned char *pCurrFrameData = (unsigned char*)(curr_frame.data);
	/** Previous frame's data pointer. */
	//unsigned char *pOldFrameData = (unsigned char*)(old_frame.data);
	/** Pointer to Mat which stores error values. */
	//short *pAuxFrameData = (short*)(aux_frame.data);

	/** Sum of matrix that corresponds to the minimum difference between current frame's block
	 *	and previous frame's block. */
	int min_diff;
	/** Absolute value of difference between previous frame's block a current fram'es block. */
	int min_block_sum;
	/** x coordinate of previous frame's block which minimizes error. */
	int min_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int min_y;
	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_size);
	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_size);

	int size = curr_frame.rows*curr_frame.cols*curr_frame.channels();
	int locations[(curr_frame.rows*curr_frame.cols)/(block_size*block_size)*2];
	int count = 0;
	// Iterate through current frame's blocks.
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			min_diff = 10000000;

			// Iterate through previous frame's blocks
			for (int old_y = ( (curr_y-block_range < 0) ? 0 : (curr_y-block_range) );
				old_y <= ( (curr_y+block_range >= max_y) ? max_y : (curr_y+block_range) ); old_y++) {
					for (int old_x = ( (curr_x-block_range < 0) ? 0 : (curr_x-block_range) );
						old_x <= ( (curr_x+block_range >= max_x) ? max_x : (curr_x+block_range) ); old_x++) {

							for (int i = 0; i < block_size; i++)
								for (int j = 0; j < block_size; j++)
									for (int ch = 0; ch < curr_frame.channels(); ch++)
										block_diff.ptr<short>(i, j)[ch] = curr_frame.ptr<uchar>(curr_y+i, curr_x+j)[ch] - old_frame.ptr<uchar>(old_y+i, old_x+j)[ch];
										//block_diff.at<Vec3s>(i,j).val[ch] = pCurrFrameData[curr_frame.channels() * (curr_frame.cols * (curr_y+i) + (curr_x+j)) + ch]-pOldFrameData[old_frame.channels() * (old_frame.cols * (old_y+i) + (old_x+j)) + ch];
							//min_block_sum = this->cost(block_diff);

							min_block_sum = this->cost(block_diff);
							if (min_block_sum < min_diff) {
								min_x = old_x;
								min_y = old_y;
								block_diff.copyTo(min_block_diff);
								min_diff = min_block_sum;
								// If the difference between blocks is 0, no need to keep searching.
								if (min_diff==0)
									break;
							}
					}
					// If the difference between blocks is 0, no need to keep searching.
					if (min_diff==0)
						break;
				}
			// Encode blocks coordinates.
			enc->encode(min_x);
			enc->encode(min_y);
			//locations[count++] = min_x;
			//locations[count++] = min_y;

			
			// Encode error between blocks. 
			for (int i = 0; i < block_size; i++)
				for (int j = 0; j < block_size; j++)
					for (int ch = 0; ch < curr_frame.channels(); ch++)
						enc->encode(min_block_diff.ptr<short>(i, j)[ch]);
						//enc->encode(min_block_diff.at<Vec3s>(i,j).val[ch]);
					//aux_frame.at<Vec3s>(curr_y+i,curr_x+j) = min_block_diff.at<Vec3s>(i,j);
						//pAuxFrameData[aux_frame.channels() * (aux_frame.cols * (curr_y+i)+(curr_x+j))+ch] = min_block_diff.at<Vec3s>(i,j).val[ch];
		}
	}
	/*
	count = 0;
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			enc->encode(locations[count++]);
			enc->encode(locations[count++]);
			for (int i = 0; i < block_size; i++)
				for (int j = 0; j < block_size; j++)
					for (int ch = 0; ch < 3; ch++) {
						enc->encode(aux_frame.at<Vec3s>(curr_y+i, curr_x+j).val[ch]);
					}
		}
	*/

	//for (int i = 0; i < curr_frame.rows; i++)
	//	for (int j = 0; j < curr_frame.cols; j++)
	//		for (int ch = 0; ch < 3; ch++)
	//			enc->encode(aux_frame.at<Vec3s>(i, j).val[ch]);

	// Calculate optimal m, verify if encoding should be intra or inter
	//for (int i = 0; i < size; i++)
	//	enc->encode(pAuxFrameData[i]);
}

InterDecoder::InterDecoder(GolombDecoder *dec, int block_size, int block_range) {
	this->dec = dec;
	this->block_size = block_size;
	this->block_range = block_range;

}

void InterDecoder::decode(Mat old_frame, Mat &curr_frame) {
	/** Previous frame's block. */
	Mat old_block;
	/** Previous frame's block. */
	Mat curr_block;

	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_size);
	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_size);
	/** x coordinate of previous frame's block which minimizes error. */
	int old_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int old_y;

	//unsigned char *pOldFrameData = (unsigned char*)(old_frame.data);
	//unsigned char *pCurrFrameData = (unsigned char*)(curr_frame.data);

	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			old_x = dec->decode();
			old_y = dec->decode();

			for (int i = curr_y; i < curr_y+block_size; i++)
				for (int j = curr_x; j < curr_x+block_size; j++)
					for (int ch = 0; ch < curr_frame.channels(); ch++)
						curr_frame.ptr<uchar>(i, j)[ch] = old_frame.ptr<uchar>(old_y+i-curr_y, old_x+j-curr_x)[ch] + dec->decode();
						//pCurrFrameData[curr_frame.channels() * (curr_frame.cols * i + j) + ch] = pOldFrameData[old_frame.channels() * (old_frame.cols * (old_y+i-curr_y) + (old_x+j-curr_x)) + ch] + dec->decode();
		}
	}
}

HybridEncoder::HybridEncoder(VideoCapture video, string format) {
	this->video = video;

	if (!format.compare("rgb"))
		this->format = 0;
	if (!format.compare("yuv444"))
		this->format = 1;
	if (!format.compare("yuv422"))
		this->format = 2;
	if (!format.compare("yuv420"))
		this->format = 3;

	this->video_width = video.get(CAP_PROP_FRAME_WIDTH);
	this->video_height = video.get(CAP_PROP_FRAME_HEIGHT);
	this->video_n_frames = video.get(CAP_PROP_FRAME_COUNT);

	// Calculate optimal parameters...
	this->predictor = 8;
	this->block_size = 16;
	this->block_range = 4;
}

void HybridEncoder::encode(string output_file) {
	FormatConverter conv;
	GolombEncoder enc(output_file);

	IntraEncoder intra_enc(&enc, predictor);
	InterEncoder inter_enc(&enc, block_size, block_range);

	Mat curr_frame;
	Mat old_frame;
	
	enc.encode(format);
	enc.encode(predictor);
	enc.encode(block_size);
	enc.encode(block_range);
	enc.encode(5);
	enc.encode(video_n_frames);

	int count = 0;
	switch (format) {
		case 0: {
			while (true) {
				video >> curr_frame;
				if (curr_frame.empty()) {break;};

				if (count==0) {
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if (count%5 == 0) {
					curr_frame.copyTo(old_frame);
					intra_enc.encode(curr_frame);
				}
				else {
					inter_enc.encode(old_frame, curr_frame);
				}
				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 1: {
			while (true) {
				video >> curr_frame;
				if (curr_frame.empty()) {break;};
				curr_frame = conv.rgb_to_yuv444(curr_frame);

				if (count==0) {
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if (count%5 == 0) {
					curr_frame.copyTo(old_frame);
					intra_enc.encode(curr_frame);
				}
				else {
					inter_enc.encode(old_frame, curr_frame);
				}
				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 2: {
			while (true) {
				video >> curr_frame;
				if (curr_frame.empty()) {break;};
				curr_frame = conv.rgb_to_yuv422(curr_frame);

				if (count==0) {
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if (count%5 == 0) {
					curr_frame.copyTo(old_frame);
					intra_enc.encode(curr_frame);
				}
				else {
					inter_enc.encode(old_frame, curr_frame);
				}
				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
		case 3: {
			while (true) {
				video >> curr_frame;
				if (curr_frame.empty()) {break;};
				curr_frame = conv.rgb_to_yuv420(curr_frame);

				if (count==0) {
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if (count%5 == 0) {
					curr_frame.copyTo(old_frame);
					intra_enc.encode(curr_frame);
				}
				else {
					inter_enc.encode(old_frame, curr_frame);
				}
				cout << "Encoded frame " << count++ << endl;
			}
			break;
		}
	}
	
	enc.finishEncoding();
}

HybridDecoder::HybridDecoder(string input_file) {
	this->input_file = input_file;
}

void HybridDecoder::decode() {
	FormatConverter conv;
	GolombDecoder dec(input_file);
	
	int format = dec.decode();
	int predictor = dec.decode();
	int block_size = dec.decode();
	int block_range = dec.decode();
	int period = dec.decode();
	int n_frames = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	cout << endl;
	cout << "Parameter m: " << dec.get_m() << endl;
	cout << "Predictor: " << predictor << endl;
	cout << "Block Size: " << block_size << endl;
	cout << "Block Range: " << block_range << endl;
	cout << "Period: " << period << endl;
	cout << endl;

	IntraDecoder intra_dec(&dec, predictor);
	InterDecoder inter_dec(&dec, block_size, block_range);

	Mat curr_frame;
	Mat old_frame;

	int count = 0;
	switch (format) {
		case 0: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC3);

				if (count%period==0){
					intra_dec.decode(curr_frame);
					curr_frame.copyTo(old_frame);
				}
				else {
					inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", curr_frame);
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 1: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC3);

				if (count%period==0){
					intra_dec.decode(curr_frame);
					curr_frame.copyTo(old_frame);
				}
				else {
					inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv444_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 2: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC1);

				if (count%period==0){
					intra_dec.decode(curr_frame);
					curr_frame.copyTo(old_frame);
				}
				else {
					inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv422_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 3: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC1);

				if (count%period==0){
					intra_dec.decode(curr_frame);
					curr_frame.copyTo(old_frame);
				}
				else {
					inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv420_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
	}
}