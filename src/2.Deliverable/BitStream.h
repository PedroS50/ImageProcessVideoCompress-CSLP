#include <stdio.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <cmath> 
#include <bitset>

using namespace std;
/*! \class BitStream
*	\brief Class that performs operations with bits.
*	
*	A class used to read/ write bits from/ to a file. 
*	Minimum amount of data that can be accessed is 1 byte.
*/
class BitStream {
	private:
		unsigned char buff, readBuff;
		int inBitCount = -1;
		int bitCount = 7;
		fstream output_file;
		fstream input_file;

	public:
		/*! \fn setOutPutFile
		*	\brief Set the output file
		*	
		*	\param file Output file path
		*/
		void setOutputFile(string file) {
			output_file.open(file, ios::binary | ios::out | ios::app);
		}

		/*! \fn setInPutFile
		*	\brief Set the input file
		*	
		*	\param file Input file path
		*/
		void setInputFile(string file) {
			input_file.open(file, ios::binary | ios::in);
		}

		void closeOutputFile() {
			output_file.close();
		}

		/*-------------------- Write Operations --------------------*/
		/*! \fn writeBit
		*	\brief Write a bit to the output file
		*	Adds the bit to a buffer until a full byte has been created.
		*	Once the buffer had 8 bits, the byte is written to the output file and the buffer is reset.
		*
		*	\param bit Bit (0 or 1) that will be written
		*/
		void writeBit(int bit) {
			if (!output_file.is_open()){
				cout << "No output file is currently opened.\n";
				return;
			}
			buff |= (bit<<bitCount);
			bitCount--;
			
			if (bitCount == -1) {
				output_file << buff;
				//output_file.write(reinterpret_cast<char*>(&buff), sizeof(buff));
				bitCount = 7;
				flush();
			}
		}

		/*! \fn writeNBits
		*	\brief Write up to 19 bits to the output file
		*	Calls writeBit function N times, based on the length of the input stream.
		*
		*	\param bits Bits (0s and 1s) that will be written
		*/
		void writeNBits(unsigned long long int bits) {
			if (!output_file.is_open()){
				cout << "No output file is currently opened.\n";
				return;
			}
			int nBits = floor(log10(bits))+1;
			unsigned long long int aux;

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

		/*! \fn flush
		*	\brief Resets the buffer to 0s
		*
		*/
		void flush() {
			while (!bitCount)
				writeBit(0);
		}
		/*----------------------------------------------------------*/

		/*--------------------- Read Operations --------------------*/
		/*! \fn readBit
		*	\brief reads 1 bit from the input file
		*
		*	\return returns 0 or 1 based on bit value
		*/
		int readBit() {
			/*if (input_file.eof()) {
				cout << "\nEnd of file\n";
				return -1;
			}*/
			if (inBitCount == -1) {
				input_file.read(reinterpret_cast<char*>(&readBuff), sizeof(readBuff));
				inBitCount = 7;
			}

			//bitset<1> x(0 | readBuff>>inBitCount);
			//cout << x;

			int bit = (1 & (readBuff>>inBitCount));

			inBitCount--;
			return bit;
		}

		/*! \fn readNBits
		*	\brief Reads N bits from the input file
		*
		*	\param nBits number of bits to read
		*
		*	\return returns a vector with size nBits that has read bits
		*/
		vector<int> readNBits(int nBits) {
			if (!input_file.is_open()){
				cout << "No Input file is currently opened.\n";
				return vector<int>();
			}

			vector<int> v;
			int bit;

			for (int n = 0; n < nBits; n++){
				bit = readBit();

				v.push_back(bit);
			}

			return v;
		}

		/*! \fn readFile
		*	\brief Reads all bits from the input file
		*
		*/
		void readFile() {
			if (!input_file.is_open()){
				cout << "No Input file is currently opened.\n";
				return;
			}

			char byte;
			/*while (!input_file.eof()) {
				input_file.read(reinterpret_cast<char*>(&byte), sizeof(byte));
				cout << byte << "\n";
			}*/

			while (input_file.get(byte)) {
				input_file.read(reinterpret_cast<char*>(&buff), sizeof(buff));
				for (int n = 7; n >= 0; n--) {
					bitset<1> x(byte>>n | 0);
					cout << x << "\n";
				}
				cout << "\n";
			}
		}

		/*! \fn resetRead
		*	\brief Resets read mark to the start of the input file
		*
		*/
		void resetRead() {
			input_file.clear();
			input_file.seekg(0, std::ios::beg);
		}
		/*----------------------------------------------------------*/
};