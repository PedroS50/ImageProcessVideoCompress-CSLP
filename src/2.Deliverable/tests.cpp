#include <iostream>
#include <stdio.h>
#include <bitset>
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
	BitStream bs;

	bs.setOutputFile("BinaryFile.bin");

	bs.setInputFile("BinaryFile.bin");

	// --------------------

	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(1);
	
	bs.writeNBits(1010);
	bs.writeNBits(101);
	bs.closeOutputFile();
	/*
	unsigned int res = bs.readNBits(16);
	cout << "0-8 bits: " << res << "\n";
	cout << (bs.readBit() & 1) << "\n";
	cout << (bs.readBit() & 1) << "\n";*/
	/*
	unsigned int res = 0;
	res = bs.readNBits(17);
	cout << "\n0-16 bits: " << res << "\n";

	unsigned char c = bs.readBit();
	cout << (c&1);
	*/


	// Read Entire file -------------------------------//
	/*unsigned int res = 0;
	int count = 0;
	unsigned char bit;

	
	while (true) {
		bit = bs.readBit();

		if (bs.getEOF()) {
			res = res >> 1;
			break;
		}

		res |= bit;
		res = res<<1;
		count++;
	}
	cout << "0-" << count << " bits: " << res<< "\n";*/
	//-------------------------------------------------//

	//res = bs.readNBits(8);
	//cout << "\n8-16 bits: " << res << "\n";
	/*
	unsigned char c = 0;
	for (int i = 7; i>=0 ; i--){
		c |= (bs.readBit() << i);
	}
	cout << "0-8 bits: " << +c<< "\n";

	unsigned char v = 0;
	for (int i = 7; i>=0 ; i--){
		v |= (bs.readBit() << i);
	}
	cout << "\n8-16: " << +v<< "\n";
	*/

	
	/*for (int i = 0; i<16 ; i++){
		cout << (bs.readBit() & 1);
		if (i == 7) 
			cout << "\n";
	}*/

	//bs.readNBits(10);

	bs.closeOutputFile();
	cout << "\n";
	
	return 0;
}