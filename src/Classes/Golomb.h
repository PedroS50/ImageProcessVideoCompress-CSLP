#ifndef GOLOMB_H
#define GOLOMB_H

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "BitStream.h"

/** \class GolombEncoder
 *	\brief Class used to encode numbers to a file using Golomb codes.
 *	
 *	Given a file path and value m (which is written in the first byte of the file),
 *	encodes and writes given numbers to the file.
 */
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

/** \class GolombDecoder
 *	\brief Class used to decode numbers from a file based on Golomb codes.
 *	
 *	Given a file path, reads the first byte which returns the value m used during encoding.
 *	Every call to method decode returns a decode int.
 */
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
};

#endif