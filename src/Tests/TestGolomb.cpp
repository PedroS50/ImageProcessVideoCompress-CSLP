#include <iostream>
#include <stdio.h>
#include "Golomb.h"
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
	GolombEncoder enc("golenc.bin");

	for (int i = 15; i > -15; i--) {
		enc.encode(i);
	}

	enc.finishEncoding();

	GolombDecoder dec("golenc.bin");

	for (int i = 0; i < 30; i++) {
		cout << dec.decode() << endl;
	}
	/*
	int m = 9;
	int predictor = 8;
	int block_size = 16;
	int block_range = 4;
	int period = 5;

	GolombEncoder enc("golenc.bin");

	enc.encode(predictor);
	enc.encode(352);
	enc.encode(288);
	enc.encode(300);
	enc.encode(block_size);
	enc.encode(block_range);
	enc.encode(period);
	enc.finishEncoding();

	GolombDecoder dec("golenc.bin");

	int dec_predictor = dec.decode();
	int dec_width = dec.decode();
	int dec_height = dec.decode();
	int dec_n_frames = dec.decode();
	int dec_block_size = dec.decode();
	int dec_block_range = dec.decode();
	int dec_period = dec.decode();

	cout << dec_predictor << endl;
	cout << dec_width << endl;
	cout << dec_height << endl;
	cout << dec_n_frames << endl;
	cout << dec_block_size << endl;
	cout << dec_block_range << endl;
	cout << "Now:" << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	*/

	/*
	GolombEncoder enc("Encoded.bin", 2);

	for (int i = -15; i <= 15; i++) {
		enc.encode(i);
	}

	enc.finishEncoding();

	GolombDecoder dec("Encoded.bin");
	
	for (int i = -15; i <= 15; i++) {
		cout << dec.decode() << endl;
	}
	*/
	/*
	GolombEncoder enc("Encoded.bin", 2);

	enc.encode(1);
	enc.encode(1920);
	enc.encode(1080);
	enc.encode(500);

	enc.encode(50);
	enc.encode(-50);
	enc.encode(0);
	enc.encode(100);
	enc.encode(23);

	enc.finishEncoding();

	GolombDecoder dec("Encoded.bin");

	cout << "Predictor = " << dec.decode() << endl;
	cout << "Width = " << dec.decode() << endl;
	cout << "Height = " << dec.decode() << endl;
	cout << "Frame Count = " << dec.decode() << endl;

	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;
	cout << dec.decode() << endl;

	return 0;
	*/
}