#include <iostream>
#include <stdio.h>
#include "Golomb.h"
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
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
}