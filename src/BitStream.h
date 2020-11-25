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
		int inBitCount = 8;
		int bitCount = 0;
		bool eof = false;
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
			eof = false;
			input_file.clear();
			input_file.seekg(0, std::ios::beg);
			input_file.open(file, ios::binary | ios::in);
		}

		void closeOutputFile() {
			if (bitCount != 0) {
				flushBuffer();
				bitCount = 0;
			}
			output_file.close();
		}

		bool getEOF() {
			return eof;
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
				cout << "No output file is currently open.\n";
				return;
			}
			buff = buff>>1;
			buff |= (bit<<7);
			bitCount++;

			if (bitCount == 8) {
				output_file << buff;
				output_file.flush();
				bitCount = 0;
			}
		}

		/*! \fn writeNBits
		*	\brief Write up to 19 bits to the output file
		*	Calls writeBit function N times, based on the length of the input stream.
		*
		*	\param bits Bits (0s and 1s) that will be written
		*/
		void writeNBits(int number, int nBits) {
			if (!output_file.is_open()){
				cout << "No output file is currently open.\n";
				return;
			}

			int nZeros = __builtin_clz(number);
			//cout << "\nnZeros:" << nZeros << "\n";
			int sizeNumber = 32 - __builtin_clz(number);
			//cout << "\nsizeNumber:" << sizeNumber << "\n";
			int n;

			if (sizeNumber > nBits) {
				cout << "Número insuficiente de bits.\n";
				return;
			}

			int nZerosPad = nBits-sizeNumber;
			
			for (n = 0; n<nZerosPad; n++) {
				writeBit(0);
			}

			for (n = sizeNumber-1; n>=0; n--) {
				writeBit(1 & (number>>n));

			}
		}

		/*! \fn flushBuffer
		*	\brief Resets the buffer to 0s
		*
		*/
		void flushBuffer() {
			
			while(bitCount) {
				writeBit(0);				
			}
		}
		/*----------------------------------------------------------*/

		/*--------------------- Read Operations --------------------*/
		/*! \fn readBit
		*	\brief reads 1 bit from the input file
		*
		*	\return returns 0 or 1 based on bit value
		*/
		unsigned char readBit() {
			if (!input_file.is_open()){
				cout << "No input file is currently open.\n";
				return 0;
			}

			if (inBitCount == 8) {
				input_file.read(reinterpret_cast<char*>(&readBuff), sizeof(readBuff));
				if (input_file.eof()){
					eof = true;
					cout << "\nEnd of File.\n";
					return 0;
				}
				inBitCount = 0;
			}
			unsigned char bit = 1 & readBuff;
			readBuff = readBuff >> 1;
			inBitCount++;
			return bit;
		}

		/*! \fn readNBits
		*	\brief Reads N bits from the input file
		*
		*	\param nBits number of bits to read
		*
		*	\return returns a vector with size nBits that has read bits
		*/
		unsigned int readNBits(int nBits) {
			if (!input_file.is_open()){
				cout << "No input file is currently open.\n";
				return 0;
			}

			unsigned char bit;

			if (!eof){
				unsigned int result = 0;

				for (int i = 0; i<nBits ; i++){
					bit = readBit();
					if (!eof) {
						result |= bit;
						result = result<<1;
					} else {
						result = result>>1;
						return result;
					}
				}
				result = result>>1;
				return result;
			} else {
				//cout << "\nEnd of File.";
				return -1;
			}
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
			eof = false;
			input_file.clear();
			input_file.seekg(0, std::ios::beg);
		}
		/*----------------------------------------------------------*/
};