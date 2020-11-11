#include <iostream>
#include <stdio.h>
#include <bitset>
#include "Golomb.h"

using namespace std;

int main( int argc, char** argv) {
	BitStream bs;

	bs.setOutputFile("BinaryFile.bin");
	bs.setInputFile("BinaryFile.bin");

	/*bs.writeNBits(5,8);
	bs.writeNBits(6,8);
	bs.writeNBits(10,5);
	bs.writeNBits(20,22);
	bs.writeNBits(1,2);

	bs.closeOutputFile();

	cout << bs.readNBits(8) << "\n";
	cout << bs.readNBits(8) << "\n";
	cout << bs.readNBits(5) << "\n";
	cout << bs.readNBits(22) << "\n";
	cout << bs.readNBits(2) << "\n";*/

	Golomb g("encoded.bin", 4);
	for ( int n = 0; n<=15; n++){
		bs.writeNBits(n, 8);
		g.encode(n);
	}

	g.finishEncoding();
	
	g.decode();


	Golomb g2("encoded2.bin", 3);

	g2.encode(240);

	g2.finishEncoding();
	
	g2.decode();

	/*for ( int n = 0; n<=15; n++){
		bs.writeNBits(n, 8);
		g2.encode(n);
	}

	g2.finishEncoding();
	
	g2.decode();*/


}