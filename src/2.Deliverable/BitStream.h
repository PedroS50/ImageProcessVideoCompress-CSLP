#include <stdio.h>
#include <iostream>
#include <fstream>

#include <cmath> 
#include <bitset>

using namespace std;

class BitStream {
	private:
		unsigned char buff;
		int bitCount = 7;
		fstream output_file;
		fstream input_file;
	public:
		void setOutputFile(string file) {
			output_file.open(file, ios::binary | ios::out | ios::app);
		}

		void setInputFile(string file) {
			input_file.open(file, ios::binary | ios::in);
		}

		/*-------------------- Write Operations --------------------*/
		bool writeBit(int bit) {
			if (!output_file.is_open()){
				cout << "Invalid file.\n";
				return false;
			}
			buff |= (bit<<bitCount);
			bitCount--;
			
			if (bitCount == -1) {
				output_file << &buff;
				bitCount = 7;
				flush();
				return true;
			}
			return false;
		}

		void writeNBits(unsigned long long int bits) {
			int nBits = floor(log10(bits))+1;
			unsigned long long int aux;

			cout << "Aqui: " << nBits << "\n";
			for (int n = 0; n < nBits; n++) {
				aux = bits / pow(10, nBits-n-1);
				writeBit(aux%10);
			}
			cout << "\n";
		}
		// ?????
		void writeStr(string text) {
			output_file << text.c_str();
		}

		int checkFileSize() {
			int nBits = 0;
			char byte;
			while (input_file.get(byte)) {
				nBits+=8;
			}
			return nBits;
		}

		void flush() {
			while (!bitCount)
				writeBit(0);
		}
		/*----------------------------------------------------------*/
		/*--------------------- Read Operations --------------------*/
		void readNBits(int nBits) {
			int nBlocks, n, remBits;
			int currBlock = 0;

			if (nBits%8 != 0)
				nBlocks = nBits/8+1;
			else
				nBlocks = nBits/8;
			char byte;

			while (input_file.get(byte)) {
				currBlock++;
				cout << "\n";
				if (currBlock != nBlocks) {
					for (n = 7;n>=0;n--) {
						bitset<1> x(byte>>n | 0);
						cout << x << "\n";
					}
				} else {
					remBits = nBits - (currBlock-1)*8;
					for (n = 7;n>7-remBits;n--) {
						bitset<1> x(byte>>n | 0);
						cout << x << "\n";
					}
					break;
				}
			}

			if (nBits > currBlock*8) {
				cout << "\nFile size is smaller (" << currBlock*8 << ") than desired bit count.\n";
				cout << "Terminating read...\n";
			}
		}

		bool readFile() {
			char byte;
			while (input_file.get(byte)) {
				for (int n = 7; n >= 0; n--) {
					bitset<1> x(byte>>n | 0);
					cout << x << "\n";
				}
				cout << "\n";
			}
		}
		/*----------------------------------------------------------*/
};