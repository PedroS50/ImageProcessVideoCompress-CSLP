#include <iostream>
#include <stdio.h>
#include <bitset>
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
	BitStream bs;
	bs.setToWrite("EncodedData.bin");
	bs.writeNBits(23, 8);
	bs.writeNBits(23, 5);
	bs.writeNBits(10, 4);
	bs.writeNBits(1, 50);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeNBits(50, 16);
	bs.close();
	bs.setToRead("EncodedData.bin");
	cout << bs.readNBits(8) << endl;
	cout << bs.readNBits(5) << endl;
	cout << bs.readNBits(4) << endl;
	cout << bs.readNBits(50) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << bs.readNBits(16) << endl;


	// --------------------
	/*
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);

	bs.close();
	bs.setToWrite("EncodedData.bin");
	
	bs.writeNBits(11, 8);
	bs.close();

	if (bs.setToRead("EncodedData.bin") == -1)
		return 0;
	unsigned char bit;
	int count = 0;
	while (true) {
		count++;
		bit = bs.readBit();
		if (bit == 2)
			break;
		
		cout << (1&bit);
		if (count % 8 == 0)
			cout << endl;
	}
	*/
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
	
	return 0;
}