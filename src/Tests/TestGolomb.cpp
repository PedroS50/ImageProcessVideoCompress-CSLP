/** \file	TestGolomb.cpp
 *	\brief	Test program for the Golomb classess by encoding and decoding numbers on a file.
 *	
 *	File used to test and demonstrate the GolombEncoder and GolombDecoder classes.
 */
#include <iostream>
#include <stdio.h>
#include "Golomb.h"
#include "BitStream.h"

using namespace std;

/**	\fn main
 *	\brief Main function used to demonstrate Golomb methods.
 *	The GolombEncoder object is created with the output file being set to
 *	"EncodedGData.bin", the parameter m is set to 3 and finally, 
 *	numbers ranging from 15 to -15 are encoded to the file. 
 *	After encoding is complete, the GolombDecoder object is created
 *	and all numbers are decoded.
 */
int main() {
	GolombEncoder enc("EncodedGData.bin");

	enc.set_m(3);

	for (int i = 15; i > -15; i--) {
		enc.encode(i);
	}

	enc.finishEncoding();

	GolombDecoder dec("EncodedGData.bin");

	dec.set_m(3);

	for (int i = 0; i < 30; i++) {
		cout << dec.decode() << endl;
	}
}