#include "Encoder.h"
#include "FormatConverter.h"
#include <unistd.h>

IntraEncoder::IntraEncoder(GolombEncoder *enc, int predictor, int shift) {
	this->predictor = predictor;
	this->enc = enc;
	this->shift = shift;
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

int IntraEncoder::encode(cv::Mat &frame) {
	/** Previous pixel color value. */
	int a;
	/** Above pixel color value. */
	int b;
	/** Diagonally previous color value. */
	int c;
	/** Error value (predictor - real value). */
	int err;
	/** Predictor value. */
	int pred;
	/** Number of frame channels. */
	int n_ch = frame.channels();
	/** Total frame size. */
	int size = frame.rows*frame.cols*n_ch;
	/** Parameter m used for encoding the given frame. */
	int mEnc;
	/** Total frame cost. */
	int frame_cost = 0;
	/** Matrix where the padded frame is stored. */
	Mat image;
	/** Matrix used to store error values. */
	Mat aux_frame;

	if (n_ch==3) {
		hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
		vconcat(Mat::zeros(1, frame.cols+1, CV_8UC3), image, image);
		aux_frame = Mat::zeros(frame.rows, frame.cols, CV_16SC3);
	} else if (n_ch==1) {
		hconcat(Mat::zeros(frame.rows, 1, CV_8UC1), frame, image);
		vconcat(Mat::zeros(1, frame.cols+1, CV_8UC1), image, image);
		aux_frame = Mat::zeros(frame.rows, frame.cols, CV_16SC1);
	}

	for (int i = 1; i < image.rows; i++) {
		for (int n = 1; n < image.cols; n++) {
			for (int ch = 0; ch < n_ch ; ch++) {
				// Retrieve values from frame.
				a = image.ptr<uchar>(i, n-1)[ch];
				b = image.ptr<uchar>(i-1, n)[ch];
				c = image.ptr<uchar>(i-1, n-1)[ch];

				pred = this->calc_predictor(a,b,c);
				err = image.ptr<uchar>(i, n)[ch] - pred;

				if (err < 0)
					err = -1*(abs(err)>>shift);
				else
					err >>= shift;

				// Store Error = estimate - real value.
				aux_frame.ptr<short>(i-1, n-1)[ch] = err;
				
				if (err < 0)
					err = -1*(abs(err)<<shift);
				else
					err <<= shift;

				image.ptr<uchar>(i, n)[ch] = (unsigned char) pred + err;

			}
		}
	}

	// Calculate optimal m...
	mEnc = enc->optimal_m(aux_frame);

	// If m stays the same encode 0, otherwise, encode m
	if (enc->get_m() == mEnc) {
		enc->encode(0);
	} else {
		enc->encode(mEnc);
		enc->set_m(mEnc);
	}

	for (int i = 0; i < aux_frame.rows; i++)
		for (int n = 0; n < aux_frame.cols; n++)
			for (int ch = 0; ch < n_ch ; ch++) {
				frame_cost+=abs(aux_frame.ptr<short>(i, n)[ch]);
				enc->encode( aux_frame.ptr<short>(i, n)[ch] );
			}

	 return frame_cost/size;
}

IntraDecoder::IntraDecoder(GolombDecoder *dec, int predictor, int shift) {
	this->predictor = predictor;
	this->dec = dec;
	this->shift = shift;
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

int IntraDecoder::decode(Mat &frame) {
	/** Previous pixel color value. */
	int a;
	/** Above pixel color value. */
	int b;
	/** Diagonally previous color value. */
	int c;
	/** Error value (predictor - real value). */
	int err;
	/** Total frame cost. */
	int frame_cost = 0;

	// Retrieve parameter m
	dec->set_m(dec->decode());

	for (int i = 0; i < frame.rows; i++) {
		for (int n = 0; n < frame.cols; n++) {
			for (int ch = 0; ch < frame.channels(); ch++){

				if (n == 0)
					a = 0;
				else
					a = frame.ptr<uchar>(i, n-1)[ch];
				
				if (i == 0)
					b = 0;
				else
					b = frame.ptr<uchar>(i-1, n)[ch];
				
				if (i == 0 || n == 0)
				 	c = 0;
				else
					c = frame.ptr<uchar>(i-1, n-1)[ch];

				err = dec->decode();
				frame_cost += abs(err);

				if (err < 0)
					err = -1*(abs(err)<<shift);
				else
					err <<= shift;

				frame.ptr<uchar>(i, n)[ch] = (unsigned char) this->calc_predictor(a,b,c) + err;
				
			}
		}
	}

	return frame_cost/(frame.rows*frame.cols*frame.channels());
}

InterEncoder::InterEncoder(GolombEncoder *enc, int block_size, int block_range, int shift) {
	this->block_size = block_size;
	this->block_range = block_range;
	this->enc = enc;
	this->shift = shift;

}

int InterEncoder::get_block_size() {
	return block_size;
}

void InterEncoder::set_block_size(int size) {
	block_size = size;
}

int InterEncoder::get_block_range() {
	return block_range;
}

float InterEncoder::cost(Mat block) {
	return sum(sum(abs(block)))[0];
}

int InterEncoder::encode(Mat old_frame, Mat curr_frame) {
	/** Block with error values. */
	Mat block_diff;
	/** Block with error values which minizes min_diff. */
	Mat min_block_diff;
	/** Frame that will store error values so they can later be processed and encoded. */
	Mat aux_frame;
	/** Number of frame channels. */
	int n_ch = curr_frame.channels();

	if ( n_ch==3 ) {
		block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
		aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC3);
	} else if ( n_ch==1 ) {
		block_diff = Mat::zeros(block_size, block_size, CV_16SC1);
		aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC1);
	}

	/** Sum of matrix that corresponds to the minimum difference between current frame's block
	 *	and previous frame's block. */
	int min_diff;
	/** Absolute value of difference between previous frame's block a current fram'es block. */
	int min_block_sum;
	/** Total frame cost. */
	int frame_cost = 0;
	/** Parameter m used for encoding given frame. */
	int mEnc;
	/** Error value (predictor - real value). */
	int err;

	/** x coordinate of previous frame's block which minimizes error. */
	int min_x;
	/** y coordinate of previous frame's block which minimizes error. */
	int min_y;
	/** Max value of x which the sliding block can reach. */
	int max_x = (curr_frame.cols - block_size);
	/** Max value of y which the sliding block can reach. */
	int max_y = (curr_frame.rows - block_size);
	/** Block location vector. */
	int locations[(curr_frame.rows*curr_frame.cols)/(block_size*block_size)*2];
	/** Current vector position. */
	int count = 0;

	// Iterate through current frame's blocks.
	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			min_diff = 10000000;

			Mat curr = Mat(curr_frame, Rect(curr_x, curr_y, block_size, block_size));
			// Iterate through previous frame's blocks
			for (int old_y = ( (curr_y-block_range < 0) ? 0 : (curr_y-block_range) );
				old_y <= ( (curr_y+block_range >= max_y) ? max_y : (curr_y+block_range) ); old_y++) {
					for (int old_x = ( (curr_x-block_range < 0) ? 0 : (curr_x-block_range) );
						old_x <= ( (curr_x+block_range >= max_x) ? max_x : (curr_x+block_range) ); old_x++) {
							Mat old = Mat(old_frame, Rect(old_x, old_y, block_size, block_size));
							
							if (n_ch==1)
								subtract(curr, old, block_diff, noArray(), CV_16SC1);
							else if (n_ch==3)
								subtract(curr, old, block_diff, noArray(), CV_16SC3);

							min_block_sum = this->cost(block_diff);
							if (min_block_sum < min_diff) {
								min_x = old_x;
								min_y = old_y;
								block_diff.copyTo(min_block_diff);
								min_diff = min_block_sum;
								// If the difference between blocks is 0, no need to keep searching.
								if (min_diff<1000)
									break;
							}
					}
					// If the difference between blocks is 0, no need to keep searching.
					if (min_diff<1000)
						break;
				}
			// Store blocks coordinates.
			locations[count++] = min_x;
			locations[count++] = min_y;

			// Store error between blocks
			Rect rect(curr_x, curr_y, block_size, block_size);
			min_block_diff.copyTo(aux_frame(rect)); 
			
		}
	}

	// Calculate optimal m...
	mEnc = enc->optimal_m(aux_frame);

	// If m stays the same encode 0, otherwise, encode m
	if (enc->get_m() == mEnc) {
		enc->encode(0);
	} else {
		enc->encode(mEnc);
		enc->set_m(mEnc);
	}

	count = 0;

	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			enc->encode(locations[count++]);
			enc->encode(locations[count++]);
			for (int i = curr_y; i < curr_y+block_size; i++)
				for (int j = curr_x; j < curr_x+block_size; j++)
					for (int ch = 0; ch < n_ch; ch++) {
						err = aux_frame.ptr<short>(i, j)[ch];

						if (err < 0)
							err = -1*(abs(err)>>shift);
						else
							err >>= shift;

						enc->encode(err);
						frame_cost+=abs(err);

						if (err < 0)
							err = -1*(abs(err)<<shift);
						else
							err <<= shift;

						curr_frame.ptr<uchar>(i, j)[ch] = err + old_frame.ptr<uchar>(i, j)[ch];

					}
		}
	return frame_cost/(curr_frame.rows*curr_frame.cols*n_ch);
}

InterDecoder::InterDecoder(GolombDecoder *dec, int block_size, int block_range, int shift) {
	this->dec = dec;
	this->block_size = block_size;
	this->block_range = block_range;
	this->shift = shift;

}

int InterDecoder::decode(Mat old_frame, Mat &curr_frame) {
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
	/** Error value (predictor - real value). */
	int err;
	/** Total frame cost. */
	int frame_cost = 0;

	// Retrieve parameter m
	dec->set_m(dec->decode());

	for (int curr_y = 0; curr_y <= max_y; curr_y += block_size) {
		for (int curr_x = 0; curr_x <= max_x; curr_x += block_size) {
			old_x = dec->decode();
			old_y = dec->decode();

			for (int i = curr_y; i < curr_y+block_size; i++)
				for (int j = curr_x; j < curr_x+block_size; j++)
					for (int ch = 0; ch < curr_frame.channels(); ch++) {
						err = dec->decode();
						frame_cost+=abs(err);
						if (err < 0)
							err = -1*(abs(err)<<shift);
						else
							err <<= shift;

								
						curr_frame.ptr<uchar>(i, j)[ch] = old_frame.ptr<uchar>(old_y+i-curr_y, old_x+j-curr_x)[ch] + err;
					}
		}
	}

	return frame_cost/(curr_frame.rows*curr_frame.cols*curr_frame.channels());
}

HybridEncoder::HybridEncoder(VideoCapture video, string format, int shift) {
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
	this->block_range = 5;
	this->shift = shift;
}

void HybridEncoder::encode(string output_file) {
	FormatConverter conv;
	GolombEncoder enc(output_file);

	IntraEncoder intra_enc(&enc, predictor, shift);
	InterEncoder inter_enc(&enc, block_size, block_range, shift);

	Mat curr_frame;
	Mat old_frame;
	
	enc.encode(format);
	enc.encode(predictor);
	enc.encode(block_range);
	enc.encode(shift);
	enc.encode(20);
	enc.encode(video_n_frames);

	int old_frame_cost = 0;
	int curr_frame_cost = 1;
	int count = 0;
	switch (format) {
		case 0: {
			while (true) {
				video >> curr_frame;
				if (curr_frame.empty()) {break;};
				
				if (count==0) {
					int a = curr_frame.cols;
					int b = curr_frame.rows;
					if (a!=b){
						int gcd = -1;
						while( b!=0 ){
							a %= b;
							if( a==0 )
								gcd = b;
							break;
							b %= a;
						}
						if (gcd == -1)
							gcd = a;
						if (gcd==a || gcd==b)
							gcd = 16;
						this->block_size = gcd;
						inter_enc.set_block_size(gcd);
						enc.encode(gcd);
					} else {
						this->block_size = 16;
						inter_enc.set_block_size(16);
						enc.encode(16);
					}

					
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}
				if ( curr_frame_cost > old_frame_cost || count%20==0 ) {
					curr_frame.copyTo(old_frame);
					curr_frame_cost = intra_enc.encode(curr_frame);
					old_frame_cost = curr_frame_cost;
				} else {
					curr_frame_cost = inter_enc.encode(old_frame, curr_frame);
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
					int a = curr_frame.cols;
					int b = curr_frame.rows;
					if (a!=b){
						int gcd = -1;
						while( b!=0 ){
							a %= b;
							if( a==0 )
								gcd = b;
							break;
							b %= a;
						}
						if (gcd == -1)
							gcd = a;
						if (gcd==a || gcd==b)
							gcd = 16;
						this->block_size = gcd;
						inter_enc.set_block_size(gcd);
						enc.encode(gcd);
					} else {
						this->block_size = 16;
						inter_enc.set_block_size(16);
						enc.encode(16);
					}

					
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if ( curr_frame_cost > old_frame_cost || count%20==0 ) {
					curr_frame.copyTo(old_frame);
					curr_frame_cost = intra_enc.encode(curr_frame);
					old_frame_cost = curr_frame_cost;
				} else {
					curr_frame_cost = inter_enc.encode(old_frame, curr_frame);
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
					int a = curr_frame.cols;
					int b = curr_frame.rows;
					if (a!=b){
						int gcd = -1;
						while( b!=0 ){
							a %= b;
							if( a==0 )
								gcd = b;
							break;
							b %= a;
						}
						if (gcd == -1)
							gcd = a;
						if (gcd==a || gcd==b)
							gcd = 16;
						this->block_size = gcd;
						inter_enc.set_block_size(gcd);
						enc.encode(gcd);
					} else {
						this->block_size = 16;
						inter_enc.set_block_size(16);
						enc.encode(16);
					}

					
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if ( curr_frame_cost > old_frame_cost || count%20==0 ) {
					curr_frame.copyTo(old_frame);
					curr_frame_cost = intra_enc.encode(curr_frame);
					old_frame_cost = curr_frame_cost;
				} else {
					curr_frame_cost = inter_enc.encode(old_frame, curr_frame);
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
					int a = curr_frame.cols;
					int b = curr_frame.rows;
					if (a!=b){
						int gcd = -1;
						while( b!=0 ){
							a %= b;
							if( a==0 )
								gcd = b;
							break;
							b %= a;
						}
						if (gcd == -1)
							gcd = a;
						if (gcd==a || gcd==b)
							gcd = 16;
						this->block_size = gcd;
						inter_enc.set_block_size(gcd);
						enc.encode(gcd);
					} else {
						this->block_size = 16;
						inter_enc.set_block_size(16);
						enc.encode(16);
					}

					
					enc.encode(curr_frame.cols);
					enc.encode(curr_frame.rows);
				}

				if ( curr_frame_cost > old_frame_cost || count%20==0 ) {
					curr_frame.copyTo(old_frame);
					curr_frame_cost = intra_enc.encode(curr_frame);
					old_frame_cost = curr_frame_cost;
				} else {
					curr_frame_cost = inter_enc.encode(old_frame, curr_frame);
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
	int block_range = dec.decode();
	int shift = dec.decode();
	int period = dec.decode();
	int n_frames = dec.decode();
	int block_size = dec.decode();
	int width = dec.decode();
	int height = dec.decode();

	cout << endl;
	cout << "Parameter m: " << dec.get_m() << endl;
	cout << "Predictor: " << predictor << endl;
	cout << "Block Size: " << block_size << endl;
	cout << "Block Range: " << block_range << endl;
	cout << "Shift: " << shift << endl;
	cout << "Period: " << period << endl;
	cout << endl;

	IntraDecoder intra_dec(&dec, predictor, shift);
	InterDecoder inter_dec(&dec, block_size, block_range, shift);

	Mat curr_frame;
	Mat old_frame;

	int old_frame_cost = 0;
	int curr_frame_cost = 1;
	int count = 0;
	switch (format) {
		case 0: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC3);

				if ( curr_frame_cost > old_frame_cost || count%period==0 ) {
					curr_frame_cost = intra_dec.decode(curr_frame);
					old_frame_cost = curr_frame_cost;
					curr_frame.copyTo(old_frame);
				}
				else {
					curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", curr_frame);
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 1: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC3);

				if ( curr_frame_cost > old_frame_cost || count%period==0 ) {
					curr_frame_cost = intra_dec.decode(curr_frame);
					old_frame_cost = curr_frame_cost;
					curr_frame.copyTo(old_frame);
				}
				else {
					curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv444_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 2: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC1);

				if ( curr_frame_cost > old_frame_cost || count%period==0 ) {
					curr_frame_cost = intra_dec.decode(curr_frame);
					old_frame_cost = curr_frame_cost;
					curr_frame.copyTo(old_frame);
				}
				else {
					curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv422_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
		case 3: {
			while (count < n_frames) {
				curr_frame = Mat::zeros(height, width, CV_8UC1);

				if ( curr_frame_cost > old_frame_cost || count%period==0 ) {
					curr_frame_cost = intra_dec.decode(curr_frame);
					old_frame_cost = curr_frame_cost;
					curr_frame.copyTo(old_frame);
				}
				else {
					curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
				}

				imshow("Image", conv.yuv420_to_rgb(curr_frame));
				if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
				cout << "Frame " << count++ << " decoded." << endl;
			}
		}
	}
}