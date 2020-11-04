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
	
	bs.writeNBits(1000);

	bs.closeOutputFile();

	// --------------------

	vector <int> v = bs.readNBits(8);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];
	
	cout << "\n";

	// --------------------

	bs.setOutputFile("BinaryFile.bin");

	bs.writeNBits(11110010);

	bs.closeOutputFile();

	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << bs.readBit();
	cout << "\n";
	cout << "\n";

	// --------------------

	bs.resetRead();

	v = bs.readNBits(8);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];
	
	cout << "\n";
	
	return 0;
}