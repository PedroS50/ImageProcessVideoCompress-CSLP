#include "HybridEncoder.h"

HybridEncoder::HybridEncoder(VideoCapture video, string video_name) {
	string output_file = video_name + "_encoded.bin";
	GolombEncoder enc(output_file);

	// Calculate optimal parameters...
	int predictor = 1;
	int block_size = 8;
	int block_range = 5;

	enc.encode(predictor);
	enc.encode(block_size);
	enc.encode(block_range);
	enc.encode(video.get(CAP_PROP_FRAME_WIDTH));
	enc.encode(video.get(CAP_PROP_FRAME_HEIGHT));
	enc.encode(video.get(CAP_PROP_FRAME_COUNT));

	IntraEncoder intra_enc(&enc, predictor);
	InterEncoder inter_enc(&enc, block_size, block_range);

	this->width = video.get(CAP_PROP_FRAME_WIDTH);
	this ->height = video.get(CAP_PROP_FRAME_HEIGHT);
	this->n_frames = video.get(CAP_PROP_FRAME_COUNT);
	this->enc = &enc;
	this->intra_enc = intra_enc;
	this->inter_enc = inter_enc;
	this->video = video;
}

void HybridEncoder::encode() {
	int count = 0;
	Mat curr_frame;
	Mat old_frame;

	while (true) {
		
		video >> curr_frame;
		if (curr_frame.empty()) {break;};
		if (count%count == 0){
			intra_enc.encode(curr_frame);
			curr_frame.copyTo(old_frame);
		}
		else {
			inter_enc.encode(old_frame, curr_frame);
		}
		cout << "Encoded frame " << count << endl;
		count++;
	}

	enc->finishEncoding();
}

HybridDecoder::HybridDecoder(string input_file) {
	GolombDecoder dec(input_file);
	this->dec = &dec;

	IntraDecoder intra_dec(&dec, dec.decode());
	InterDecoder inter_dec(&dec, dec.decode(), dec.decode());

	this->width = dec.decode();
	this->height = dec.decode();
	this->n_frames = dec.decode();
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
			intra_dec.decode(curr_frame);
			curr_frame.copyTo(old_frame);
		}
		else {
			inter_dec.decode(old_frame, curr_frame);
		}
		imshow("Image", curr_frame);
		if (waitKey(10) == 27) {destroyAllWindows();}; // Wait for a keystroke in the window
	}
}