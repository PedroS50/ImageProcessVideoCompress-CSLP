#ifndef GOLOMB_H
#define GOLOMB_H

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "BitStream.h"

class GolombEncoder {
private:
	/** BitStream instance that will be used to write bits to a file. */
	BitStream bitStream;
	
	/** Value m of Golomb codes. */
	int mEnc;

public:
	/**	\brief Constructor.
	 * 
	 * \param file_path String containing the path to the file where the encoded data will be stored.
	 * \param m Parameter m of Golomb codes
	 */
	GolombEncoder(string file_path, int m);

	/** \brief Method used to encoded a signed int.
	 *	\param num number that will be encoded
	 */
	void encode(int num);

	/**	\brief Calls BitStream's close method, closing the output file and flushing the write buffer.
	 */
	void finishEncoding();

};

class GolombDecoder {
private:
	/** BitStream instance that will be used to read bits from file. */
	BitStream bitStream;

	/** Value m of Golomb codes. */
	int mEnc;

public:
	/**	\brief Constructor.
	 * 	\param file_path String containing the path to the file containing encoded data.
	 */
	GolombDecoder(string path);

	/** \brief Method used to decode one number.
	 */
	int decode();

	void decodeFrame(vector<cv::Mat> &frame, int m, int height, int width);

	/** \fn readFileHeaders
	 *	\brief Read a files headers in order to get values m, width, height and frame count from an encoded video.
	 *
	 *	\param input_path File from which the headers will be read
	 *	\param m Variable where value m will be stored
	 *	\param width Variable where video width will be stored
	 *	\param height Variable where video height will be stored
	 *	\param frameCount Variable where the number of frames will be stored
	 */
	int readFileHeaders(string input_path, int &m, int &predictor, int &width, int &height, int &frameCount);


};

#endif