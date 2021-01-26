/** \file	TestBitStream.cpp
 *	\brief	Test program for the BitStream class by writting and reading bits/ numbers on a file.
 *	
 *	File used to test and demonstrate the BitStream class. All class methods are used, 
 *	including writting bits, writting number with a set amount of bits, reading bits and 
 *	reading numbers.
 *
 */
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "BitStream.h"

using namespace std;

/**	\fn main
 *	\brief Main function used to demonstrate BitStream methods.
 *	The BitStream object is first created, set to write mode and several 
 *	numbers and bits are written in succession with the output
 *	stored in the file "EncodedBSData.bin", in the end the close() method must be called.
 *	The BitStream object is then set to read mode and all the numbers
 *	and bits are read.
 */
int main( int argc, char** argv) {
	BitStream bs;

	bs.setToWrite("EncodedBSData.bin");

	bs.writeNBits(23, 8);
	bs.writeNBits(23, 5);
	bs.writeNBits(10, 4);
	bs.writeNBits(1, 50);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeNBits(50, 16);

	bs.close();

	bs.setToRead("EncodedBSData.bin");

	cout << bs.readNBits(8) << endl;
	cout << bs.readNBits(5) << endl;
	cout << bs.readNBits(4) << endl;
	cout << bs.readNBits(50) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << (bs.readBit()&1) << endl;
	cout << bs.readNBits(16) << endl;
	
	return 0;
}