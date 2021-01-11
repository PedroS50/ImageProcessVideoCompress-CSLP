#include "Encoder.h"
#include <unistd.h>

IntraEncoder::IntraEncoder(GolombEncoder *enc, int predictor) {
	this->predictor = predictor;
	this->enc = enc;
}

int IntraEncoder::get_predictor() {
	return predictor;
}

void IntraEncoder::encode(cv::Mat &frame) {
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
	int mEnc;

	Mat aux_frame = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
	// Matrix where the padded frame is stored.
	Mat image;
	hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
	vconcat(Mat::zeros(1, frame.cols+1, CV_8UC3), image, image);
	/** Frame data pointer. */
	unsigned char *pFrameData = (unsigned char*)(image.data);
	short *pAuxFrameData = (short*)(aux_frame.data);

	// Iterate through height, width and channel.
	for (int i = 1; i < image.rows; i++) {
		for (int n = 1; n < image.cols; n++) {
			for (int ch = 0; ch < 3 ; ch++) {
				// Retrieve values from frame.
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
				// Store Error = estimate - real value.
				pAuxFrameData[aux_frame.channels() * (aux_frame.cols * (i-1) + (n-1)) + ch] = pFrameData[image.channels() * (image.cols * i + (n)) + ch] - pred;
				// Encode Error = estimate - real value.
				//enc->encode(pFrameData[image.channels() * (image.cols * i + (n)) + ch] - pred);
			}
		}
	}

	// Calculate optimal m...
	mEnc = 2;
	if (enc->get_m() == mEnc) {
		enc->encode(0);
	} else {
		enc->encode(mEnc);
	}
	enc->set_m(mEnc);

	for (int i = 0; i < aux_frame.rows; i++) {
		for (int n = 0; n < aux_frame.cols; n++) {
			for (int ch = 0; ch < 3 ; ch++) {
				//cout << int(aux_frame.at<Vec3b>(i,n).val[ch]) << endl;
				enc->encode(pAuxFrameData[aux_frame.channels() * (aux_frame.cols * i + (n)) + ch]);
			}
		}
	}
}

IntraDecoder::IntraDecoder(GolombDecoder *dec, int predictor) {
	this->predictor = predictor;
	this->dec = dec;
}

void IntraDecoder::decode(Mat &frame) {
	int err, m;
	unsigned char a, b, c, pred, mini, maxi;

	unsigned char *pFrameData = (unsigned char*)(frame.data);

	// Retrieve parameter m
	m = dec->decode();
	if (m != 0)
		dec->set_m(m);

	for (int i = 0; i < frame.rows; i++) {
		for (int n = 0; n < frame.cols; n++) {
			for (int ch = 0; ch < 3; ch++){

				if (n == 0)
					a = 0;
				else
					//a = frame.at<Vec3b>(i, n-1).val[ch];
					a = pFrameData[frame.channels() * (frame.cols * i + (n-1)) + ch];
				
				if (i == 0)
					b = 0;
				else
					//b = frame.at<Vec3b>(i-1, n).val[ch];
					b = pFrameData[frame.channels() * (frame.cols * (i-1) + n) + ch];
				
				if (i == 0 || n == 0)
					c = 0;
				else
					//c = frame.at<Vec3b>(i-1, n-1).val[ch];
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

				err = dec->decode();
				pFrameData[frame.channels() * (frame.cols * i + n) + ch] = pred + err;
				//frame.at<Vec3b>(i, n).val[ch] = pred+err;
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

void InterEncoder::encode(Mat old_frame, Mat curr_frame) {
	/** Current frame's block. */
	Mat curr_block;
	/** Previous frame's block. */
	Mat old_block;
	/** Block with error values. */
	Mat block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
	/** Block with error values which minizes min_diff. */
	Mat min_block_diff;

	/** Sum of matrix that corresponds to the minimum difference between current frame's block
	 *	and previous frame's block. */
	int min_diff;

	int min_block_sum;
	/** x coordinate of previous frame's block which minimizes error. */
	int min_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int min_y;
	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_size);
	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_size);

	unsigned char *pCurrFrameData = (unsigned char*)(curr_frame.data);
	unsigned char *pOldFrameData = (unsigned char*)(old_frame.data);

	// Iterate through current frame's blocks.
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			min_diff = 10000000;

			// Get current block.
			//curr_block = Mat(curr_frame, Rect(curr_x, curr_y, block_size, block_size));

			// Iterate through previous frame's blocks
			for (int old_y = ( (curr_y-block_range < 0) ? 0 : (curr_y-block_range) );
				old_y <= ( (curr_y+block_range >= max_y) ? max_y : (curr_y+block_range) ); old_y++) {
					for (int old_x = ( (curr_x-block_range < 0) ? 0 : (curr_x-block_range) );
						old_x <= ( (curr_x+block_range >= max_x) ? max_x : (curr_x+block_range) ); old_x++) {
							
							//old_block = Mat(old_frame, Rect(old_x, old_y, block_size, block_size));
							//block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
							for (int i = 0; i < block_size; i++)
								for (int j = 0; j < block_size; j++)
									for (int ch = 0; ch < 3; ch++)
										block_diff.at<Vec3s>(i,j).val[ch] = pCurrFrameData[curr_frame.channels() * (curr_frame.cols * (curr_y+i) + (curr_x+j)) + ch]-pOldFrameData[old_frame.channels() * (old_frame.cols * (old_y+i) + (old_x+j)) + ch];
										//block_diff.at<Vec3s>(i,j).val[ch] = curr_block.at<Vec3b>(i,j).val[ch] - old_block.at<Vec3b>(i,j).val[ch];
							min_block_sum = abs(sum(sum(block_diff))[0]);
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
			
			// Encode error between blocks. 
			for (int i = 0; i < block_size; i++) {
				for (int j = 0; j < block_size; j++) {
					for (int ch = 0; ch < 3; ch++) {
						enc->encode(min_block_diff.at<Vec3s>(i,j).val[ch]);
					}
				}
			}
		}
	}
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
	
	/** Error decoded from file. */
	int err;

	/** Predicator value extracted from previous frame. */
	int pred;

	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_size);

	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_size);

	/** x coordinate of previous frame's block which minimizes error. */
	int old_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int old_y;

	unsigned char *pOldFrameData = (unsigned char*)(old_frame.data);
	unsigned char *pCurrFrameData = (unsigned char*)(curr_frame.data);

	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			old_x = dec->decode();
			old_y = dec->decode();

			for (int i = curr_y; i < curr_y+block_size; i++) {
				for (int j = curr_x; j < curr_x+block_size; j++) {
					for (int ch = 0; ch < 3; ch++) {
						pCurrFrameData[curr_frame.channels() * (curr_frame.cols * i + j) + ch] = pOldFrameData[old_frame.channels() * (old_frame.cols * (old_y+i-curr_y) + (old_x+j-curr_x)) + ch] + dec->decode();
					}
				}
			}
			/*
			for (int aux_y = old_y; aux_y < old_y+block_size; aux_y++) {
				for (int aux_x = old_x; aux_x < old_x+block_size; aux_x++) {
					for (int ch = 0; ch < 3; ch++) {
						//cout << old_x+block_size << endl;
						pCurrFrameData[curr_frame.channels() * (curr_frame.cols * (curr_y+aux_y-old_y) + (curr_x+aux_x-old_x)) + ch] = dec->decode() + pOldFrameData[old_frame.channels() * (old_frame.cols * aux_y + aux_x) + ch];
						//curr_frame.at<Vec3b>(curr_y+aux_y-old_y, curr_x+aux_x-old_x).val[ch] = dec->decode() + old_frame.at<Vec3b>(aux_y, aux_x).val[ch];
						//cout << curr_y+aux_y-old_y << ", " << curr_x+aux_x-old_x << ", " << ch << endl;
					}
				}
				old_x-=block_size;
				//cout << old_x << endl;
				//while (true){}
			}
			*/
		}
	}
}

HybridEncoder::HybridEncoder(VideoCapture video, string video_name) {
	string output_file = "encoded.bin";
	GolombEncoder enc(output_file);

	// Calculate optimal parameters...
	this->predictor = 1;
	this->block_size = 8;
	this->block_range = 5;

	this->width = video.get(CAP_PROP_FRAME_WIDTH);
	this ->height = video.get(CAP_PROP_FRAME_HEIGHT);
	this->n_frames = video.get(CAP_PROP_FRAME_COUNT);
	this->video = video;
}

void HybridEncoder::encode() {
	string output_file = "encoded.bin";
	GolombEncoder enc(output_file);

	// Mandatory, doesn't affect first frame
	enc.set_m(4);

	//IntraEncoder intra_enc(this->predictor);
	//InterEncoder inter_enc(this->block_size, this->block_range);

	enc.encode(this->predictor);
	enc.encode(this->block_size);
	enc.encode(this->block_range);
	enc.encode(width);
	enc.encode(height);
	enc.encode(n_frames);

	int count = 0;
	Mat curr_frame;
	Mat old_frame;

	while (true) {
		//cout << "CheckAA" << endl;
		video >> curr_frame;
		//cout << "CheckBB" << endl;
		if (curr_frame.empty()) {break;};
		if (count%count == 0){
			cout << "CheckE" << endl;
			//intra_enc.encode(enc, curr_frame);
			cout << "Check1A" << endl;
			curr_frame.copyTo(old_frame);
			//cout << "Check1B" << endl;
		}
		else {
			//inter_enc.encode(enc, old_frame, curr_frame);
		}
		cout << "Encoded frame " << count << endl;
		count++;
	}

	enc.finishEncoding();
}

HybridDecoder::HybridDecoder(string input_file) {
	GolombDecoder dec(input_file);
	this->dec = &dec;

	int predictor = dec.decode();
	int block_size = dec.decode();
	int block_range = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	int n_frames = dec.decode();

	//IntraDecoder intra_dec(&dec, predictor);
	InterDecoder inter_dec(&dec, block_size, block_range);

	this->width = width;
	this->height =height;
	this->n_frames = n_frames;
	this->intra_dec = intra_dec;
	this->inter_dec = inter_dec;

}

void HybridDecoder::decode() {
	int count = 0;
	Mat curr_frame;
	Mat old_frame;

	for (int n = 0; n < n_frames; n++) {
		cout << "Decoding frame " << n+1 << endl;
		curr_frame = Mat::zeros(height, width, CV_8UC3);
		if (n%n==0){
			//intra_dec.decode(curr_frame);
			curr_frame.copyTo(old_frame);
		}
		else {
			inter_dec.decode(old_frame, curr_frame);
		}
		imshow("Image", curr_frame);
		if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
	}
}