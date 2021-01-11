#include "Golomb.h"

GolombEncoder::GolombEncoder(string file_path) {
	bitStream.setToWrite(file_path);
	this->set_m(3);
}

void GolombEncoder::set_m(int m) {
	this->mEnc = m;
	this->b = ceil(log2(m));

}

int GolombEncoder::get_m() {
	return mEnc;

}

void GolombEncoder::encode(int num) {
	int q, r;

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

	if ( mEnc%2==0 ) {
		bitStream.writeNBits(r, b);

	} else {

		if (r < pow(2, b+1)-mEnc) {
			bitStream.writeNBits(r, b);

		} else {
			bitStream.writeNBits(r+pow(2, b+1)-mEnc, b+1);

		}
	}
}

void GolombEncoder::finishEncoding() {
	bitStream.close();
}

GolombDecoder::GolombDecoder(string file_path) {
	bitStream.setToRead(file_path);
	this->set_m(3);

}

void GolombDecoder::set_m(int m) {
	this->mEnc = m;
	this->b = ceil(log2(m));

}

int GolombDecoder::get_m() {
	return mEnc;

}

int GolombDecoder::decode() {
	unsigned int r;
	signed int num;
	int q, signal;

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

	if ( mEnc%2==0 ) {
		r = bitStream.readNBits(b);
		num = q*mEnc+r;
		if (!signal)
			return num; 
		else
			return -1*num;
	}
	else {
		r = bitStream.readNBits(b);

		if (r < pow(2, b+1)-mEnc) {
			num = q*mEnc+r;
			if (!signal)
				return num; 
			else
				return -1*num;
		} else {
			int val = (bitStream.readBit()&1);
			num = q*mEnc + ( 2*r+val ) - pow(2, b+1)+mEnc;
			if (!signal)
				return num; 
			else
				return -1*num;
		}
	}
}
