#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "2.Deliverable/BitStream.h"

using namespace std;
/*! \class Golomb
*	\brief Encoding based on Golomb codes
*	
*	Class with methods to encode signed ints and decode an encoded file
*	based on the Golomb Codes, capable of encoding (and decoding) for any value of m
*/
class Golomb {
	private:
		string file_path;
		int m;
		BitStream bitStream;

	public:
		Golomb(string path, int value) {
			file_path = path;
			m = value;
			
			bitStream.setOutputFile(file_path);
			bitStream.writeNBits(m, 8);
		}

		/*! \fn encode
		*	\brief encode a signed int
		*	
		*	\param signedint number that will  be encoded
		*/
		void encode (signed int num) {

			int k, q, r;

			q = num/m;
			//cout << "q = " << num << " / " << m << " = " << q << "\n";
			r = num%m;
			//cout << "r = " << num << " % " << m << " = " << r << "\n";

			for (int i = 0; i<q; i++) {
				bitStream.writeBit(1);
			}

			bitStream.writeBit(0);

			if (m%2 == 0) {
				k = log2(m);
				bitStream.writeNBits(r, k);
			} else {
				int b = ceil(log2(m));

				if (r < b)
					k = b - 1;
				else
					k = b;

				int sizeNumber = 32 - __builtin_clz(r+b);
				
				if (sizeNumber>k)
					for (int n = 0; n<k; n++)
						bitStream.writeBit(1);
				else
					bitStream.writeNBits(r+b, k);
			}			

		}
		
		/*
		void encodeList(int numArray[], int size) {
			BitStream bitStream;
			bitStream.setOutputFile(file_path);
			
			int k = log2(m);

			bitStream.writeNBits(k, 8);

			signed int num;
			for (int n = 0; n < size; n++){
				num = numArray[n];

				int q, r;

				q = num/m;
				//cout << "q = " << num << " / " << m << " = " << q << "\n";
				r = num%m;
				//cout << "r = " << num << " % " << m << " = " << r << "\n";

				for (int i = 0; i<q; i++) {
					bitStream.writeBit(1);
				}

				bitStream.writeBit(0);

				bitStream.writeNBits(r, k);
			}

			bitStream.closeOutputFile();
		}
		*/

		/*! \fn decode
		*	\brief decode an encoded file
		*	Decodes the file previously defined and prints the decoded data
		*/
		void decode () {
			BitStream bitStream;
			bitStream.setInputFile(file_path);

			unsigned int r;
			int q, k, b;

			signed int num = 0;
			unsigned char bit;

			m = bitStream.readNBits(8);
			//cout << "\nSeparador\n";

			while (true){
				q = 0;
				if (bitStream.getEOF())
					break;

				while (true) {
					bit = bitStream.readBit();
					//cout << "\n" << (0|bit) << "\n";
					if ((bit & 1) == 0) 
						break;
					q++;
				}
				if (bitStream.getEOF())
					break;
				if (m%2 == 0) {
					k = log2(m);
					r = bitStream.readNBits(k);
					cout << "\nResultado: " << q*m+r << "\n"; }

				else {
					b = ceil(log2(m));
					k = pow(2, b)-m;
					r = bitStream.readNBits(b);

					if (r < k) {
						cout << "\nResultado: " << q*m+r << "\n";

					} else {
						int val = (bitStream.readBit()&1);
						//cout << "\nAqui: " << val;
						cout << "\nResultado: " << q*m + ((r<<1)&val) - k+1 << "\n";
					}
				}

			}
		}

		/*! \fn finishEncoding
		*	\brief closes the output file flushing the write buffer
		*/
		void finishEncoding() {
			bitStream.closeOutputFile();
		}
};