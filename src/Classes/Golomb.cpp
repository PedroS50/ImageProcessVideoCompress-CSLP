#include "Golomb.h"

GolombEncoder::GolombEncoder(string file_path, int m) {
	bitStream.setToWrite(file_path);
	mEnc = m;

	bitStream.writeNBits(m, 8);
}

void GolombEncoder::encode(int num) {
	int k, q, r;

	if (num < 0)
		bitStream.writeBit(1);
	else
		bitStream.writeBit(0);

	num = abs(num);

	q = num/mEnc;
	//cout << "q = " << num << " / " << m << " = " << q << "\n";
	r = num%mEnc;
	//cout << "r = " << num << " % " << m << " = " << r << "\n";

	for (int i = 0; i<q; i++) {
		bitStream.writeBit(1);
	}

	bitStream.writeBit(0);

	if ( !(mEnc&1) ) {
		k = log2(mEnc);
		bitStream.writeNBits(r, k);

	} else {
		int b = ceil(log2(mEnc));

		if (r < pow(2, b)-mEnc) {
			bitStream.writeNBits(r, b-1);

		} else {
			bitStream.writeNBits(r+pow(2, b)-mEnc, b);

		}
	}
}

void GolombEncoder::finishEncoding() {
	bitStream.close();
}

GolombDecoder::GolombDecoder(string file_path) {
	bitStream.setToRead(file_path);

	mEnc = bitStream.readNBits(8);

}

int GolombDecoder::decode() {
	unsigned int r;
	int q, k, b, signal;

	signed int num;
	unsigned char bit;

	q = 0;

	if (bitStream.getEOF())
		return NULL;

	signal = bitStream.readBit() & 1;

	while (true) {
		bit = bitStream.readBit();

		if ((bit & 1) == 0) 
			break;
		q++;
	}

	if ( !(mEnc&1) ) {
		k = log2(mEnc);
		r = bitStream.readNBits(k);
		num = q*mEnc+r;
		if (!signal)
			return num; 
		else
			return -1*num;
	}
	else {
		b = ceil(log2(mEnc));
		r = bitStream.readNBits(b-1);

		if (r < pow(2, b)-mEnc) {
			num = q*mEnc+r;
			if (!signal)
				return num; 
			else
				return -1*num;
		} else {
			int val = (bitStream.readBit()&1);
			num = q*mEnc + ( 2*r+val ) - pow(2, b)+mEnc;
			if (!signal)
				return num; 
			else
				return -1*num;
		}
	}
}

void GolombDecoder::decodeFrame(vector<cv::Mat> &frame, int m, int height, int width) {
	unsigned int r;
	int q, k, b, signal;

	signed int num = 0;
	unsigned char bit;

	for (int ch = 0; ch < 3; ch++) {
		cv::Mat frameCh = cv::Mat::zeros(height, width, CV_32S);
		for (int i = 0; i < height; i++) {
			for (int n = 0; n < width; n++) {
				q = 0;

				signal = bitStream.readBit() & 1;
				while (true) {
					bit = bitStream.readBit();

					if ((bit & 1) == 0) 
						break;
					q++;
				}

				if (bitStream.getEOF())
					break;

				if ( !(m&1) ) {
					k = log2(m);
					r = bitStream.readNBits(k);
					if (signal)
						num = -1*(q*m+r);
					else
						num = q*m+r;
				}
				else {
					b = ceil(log2(m));
					r = bitStream.readNBits(b-1);

					if (r < pow(2, b)-m) {
						if (signal)
							num = -1*(q*m+r);
						else
							num = q*m+r;

					} else {
						int val = (bitStream.readBit()&1);
						if (signal)
							num = -1*(q*m + ( 2*r+val ) - pow(2, b)+m);
						else
							num = q*m + ( 2*r+val ) - pow(2, b)+m;

					}
				}
				frameCh.at<int>(i, n) = num;
			}
		}
		frame.push_back(frameCh);
	}
}

int GolombDecoder::readFileHeaders(string input_path, int &m, int &predictor, int &width, int &height, int &frameCount) {
	m = bitStream.readNBits(8);
	predictor = bitStream.readNBits(8);
	height = bitStream.readNBits(12);
	width = bitStream.readNBits(12);
	frameCount = bitStream.readNBits(16);
}
