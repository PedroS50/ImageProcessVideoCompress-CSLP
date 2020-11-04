#include <iostream>
#include <stdio.h>
#include <bitset>
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
	/*
	// Test print 8 bits: 10101100
	unsigned char buff;
	int inBit;
	int bitCount = 0;

	while (true) {
		cin >> inBit;
		//if (inBit)
		buff |= (inBit<<bitCount);
		bitCount++;
		
		if (bitCount == 8) {
			bitset<8> x(buff);
			cout << x;
			cout << '\n';
			buff = 00000000;
			bitCount = 0;
			break;
		}
	}*/
	BitStream bs;
	bs.setOutputFile("Whatever.dat");
	bs.setInputFile("Whatever.dat");
	//bs.readNBits(20);
	cout << bs.checkFileSize() << "\n";
	/*bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);

	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);*/
	
	//bs.writeNBits(1110101111);
	//bs.writeStr("Hello World!");
	//bs.setInputFile("Whatever.dat");
	//bs.readFile();
	return 0;
}