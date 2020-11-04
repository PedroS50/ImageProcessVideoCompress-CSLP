#include <iostream>
#include <stdio.h>
#include <bitset>
#include "BitStream.h"

using namespace std;

int main( int argc, char** argv) {
	BitStream bs;
	bs.setOutputFile("BinaryFile.bin");
	bs.setInputFile("BinaryFile.bin");

	/*bs.writeStr("Hello World!");
	vector<int> v = bs.readNBits(20);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];

	cout << "\n";*/
	//bs.writeNBits(10101010);
	/*int counter = 0;
	while (true) {
		counter++;
		cout << bs.readBit();
		if (counter == 10)
			break;
	}*/

	/*bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);	
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);

	bs.writeNBits(11001001);

	bs.closeOutputFile();*/

	for (int i = 0; i<5; i++){
		cout << bs.readBit();
	}

	vector<int> v = bs.readNBits(3);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];
	cout << "\n";
	
	v = bs.readNBits(8);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];
	cout << "\n";

	//bs.writeNBits(1110101111);
	//bs.writeStr("Hello World!");
	//bs.setInputFile("Whatever.bin");
	/*
	vector<int> v = bs.readNBits(3);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];

	cout << "\n";
	cout << bs.readBit();
	cout << bs.readBit();
	cout << "\n";

	v = bs.readNBits(3);
	for (int i = 0; i<v.size(); i++)
		cout << v[i];
	cout << "\n";
	*/
	
	return 0;
}