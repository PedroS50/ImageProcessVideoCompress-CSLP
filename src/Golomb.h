#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "opencv2/opencv.hpp"
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
		int mEnc, wEnc, hEnc, nFrames;
		BitStream bitStream;

	public:
		Golomb(string path, int value, int w, int h, int nF) {
			file_path = path;
			mEnc = value;
			wEnc = w;
			hEnc = h;
			nFrames = nF;
			
			bitStream.setOutputFile(file_path);
			bitStream.writeNBits(value, 8);
			bitStream.writeNBits(h, 12);
			bitStream.writeNBits(w, 12);
			bitStream.writeNBits(nF, 16);
		}

		/*! \fn encode
		*	\brief encode a signed int
		*	
		*	\param signedint number that will  be encoded
		*/
		void encode (signed int num) {

			int k, q, r;

			if (num < 0)
				bitStream.writeBit(1);
			else
				bitStream.writeBit(0);

			num = abs(num);

			q = num/mEnc;
			//cout << "q = " << num << " / " << m << " = " << q << "\n";
			r = num%mEnc;
			//cout << "r = " << num << " % " << m << " = " << r << "\n";

			for (int i = 0; i<q; i++) {
				bitStream.writeBit(1);
				
			}

			bitStream.writeBit(0);

			if (mEnc%2 == 0) {
				k = log2(mEnc);
				bitStream.writeNBits(r, k);

			} else {
				int b = ceil(log2(mEnc));

				if (r < pow(2, b)-mEnc) {
					bitStream.writeNBits(r, b-1);

				} else {
					bitStream.writeNBits(r+pow(2, b)-mEnc, b);

				}
			}

		}

		/*! \fn decode
		*	\brief decode an encoded file
		*	Decodes the file previously defined and prints the decoded data
		*/
		void decode () {
			BitStream bitStream;
			bitStream.setInputFile(file_path);

			unsigned int r;
			int q, k, b, signal, m;

			signed int num = 0;
			unsigned char bit;
			bitStream.readNBits(48);

			//cout << "\nSeparador\n";

			while (true){
				q = 0;
				if (bitStream.getEOF())
					break;

				signal = bitStream.readBit() & 1;

				while (true) {
					bit = bitStream.readBit();

					if ((bit & 1) == 0) 
						break;
					q++;
				}

				if (bitStream.getEOF())
					break;

				if (m%2 == 0) {
					k = log2(m);
					r = bitStream.readNBits(k);
					num = q*m+r;
					if (!signal)
						cout << "\nResultado: " << num << "\n"; 
					else
						cout << "\nResultado: " << -1*num << "\n";
				}
				else {
					b = ceil(log2(m));
					r = bitStream.readNBits(b-1);

					if (r < pow(2, b)-m) {
						num = q*m+r;
						if (!signal)
							cout << "\nResultado: " << num << "\n"; 
						else
							cout << "\nResultado: " << -1 * num << "\n";
					} else {
						int val = (bitStream.readBit()&1);
						num = q*m + ( 2*r+val ) - pow(2, b)+m;
						if (!signal)
							cout << "\nResultado: " << num << "\n"; 
						else
							cout << "\nResultado: " << -1 * num << "\n";
					}
				}

			}
		}

		/*! \fn decodeVideo
		*	\brief 	Decodes an encoded video file storing the information on a vector.
		*			Each vector element will be a channel from a frame from the video.
		*	
		*	\param video Video vector address where the frames will be stored
		*/
		void decodeVideo(vector<cv::Mat> &video) {
			BitStream bitStream;
			bitStream.setInputFile(file_path);

			unsigned int r;
			int q, k, b, signal, m, width, height, frameCount;

			signed int num = 0;
			unsigned char bit;

			m = bitStream.readNBits(8);
			height = bitStream.readNBits(12);
			width = bitStream.readNBits(12);
			frameCount = bitStream.readNBits(16);

			//cout << "\nSeparador\n";

			for (int frameN = 0; frameN < frameCount; frameN++) {
				for (int ch = 0; ch < 3; ch++) {
					cv::Mat frame = cv::Mat::zeros(height, width, CV_32S);
					for (int i = 0; i < height; i++) {
						for (int n = 0; n < width; n++) {
							q = 0;

							signal = bitStream.readBit() & 1;
							while (true) {
								bit = bitStream.readBit();

								if ((bit & 1) == 0) 
									break;
								q++;
							}

							if (bitStream.getEOF())
								break;

							if (m%2 == 0) {
								k = log2(m);
								r = bitStream.readNBits(k);
								if (signal)
									num = -1*(q*m+r);
								else
									num = q*m+r;
							}
							else {
								b = ceil(log2(m));
								r = bitStream.readNBits(b-1);

								if (r < pow(2, b)-m) {
									if (signal)
										num = -1*(q*m+r);
									else
										num = q*m+r;

								} else {
									int val = (bitStream.readBit()&1);
									if (signal)
										num = -1*(q*m + ( 2*r+val ) - pow(2, b)+m);
									else
										num = q*m + ( 2*r+val ) - pow(2, b)+m;

								}
							}
							//cout << "Decoded: " << num << endl;
							//cout << i << ", " << n << endl;
							//cout << "Read number " << num << endl;
							frame.at<int>(i, n) = num;
							//cout << frame << endl;
						}
					}
					//cout << frame << endl;
					video.push_back(frame);
					//cout << frame << endl;
				}
			}
			//cout << video.at(0) << endl;
		}

		/*! \fn finishEncoding
		*	\brief closes the output file flushing the write buffer
		*/
		void finishEncoding() {
			bitStream.closeOutputFile();
		}
};