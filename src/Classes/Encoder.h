#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

using namespace cv;
using namespace std;

/** \class IntraEncoder
 *	\brief Class used of encoding frames from a video using intra-frame prediction encoding.
 *	Error values are encoded in a file using Golomb codes.
 */
class IntraEncoder {
private:
	/** Predictor used when encoding frames. */
	int predictor;
	/** Pointer to GolombEncoder object used for encoding error values. */
	GolombEncoder *enc;

public:
	/** \fn IntraEncoder
	 * 	\brief Constructor for IntraEncoder objects.
	 * 
	 * 	\param enc Pointer to GolombEncoder object used for encoding error values into a file.
	 * 	\param predictor Predictor used when calculating error values.
	 */
	IntraEncoder(GolombEncoder *enc, int predictor);

	/** \fn IntraEncoder
	 * 	\brief Default constructor for IntraEncoder objects.
	 */
	IntraEncoder() = default;

	/** \fn get_predictor
	 * 	\brief Get method for predictor value.
	 * 
	 * 	\return Integer value which represents the chosen predictor.
	 */
	int get_predictor();

	/** \fn calc_predictor
	 * 	\brief Calculates prediction value based on selected predictor and a, b and c color values.
	 * 	
	 * 	\param a a pixel color value (x-1, y)
	 * 	\param b b pixel color value (x, y-1)
	 * 	\param c c pixel color value (x-1, y-1)
	 * 	\return Prediction integer value.
	 */
	int calc_predictor(int a, int b, int c);

	/** \fn 	intraEncoder
	 *	\brief 	Encode a frame using intraframe predictive coding, based on Golomb codes.
	 *	
	 *	\param frame 	Frame that will be encoded.
	 */
	void encode(Mat &frame);
};

/** \class IntraDecoder
 *	\brief Class used to decode frames from a file encoded using intra-frame prediction encoding.
 */
class IntraDecoder {
private:
	/** Predictor used during video encoding. */
	int predictor;
	/** Pointer to GolombDecoder object used for decoding error values. */
	GolombDecoder *dec;

public:
	/** \fn IntraDecoder
	 * 	\brief Constructor for IntraDecoder objects.
	 * 
	 * 	\param dec Pointer to GolombDecoder object used for decoding error values from a file.
	 * 	\param predictor Predictor used when calculating error values.
	 */
	IntraDecoder(GolombDecoder *dec, int predictor);

	/** \fn IntraDecoder
	 * 	\brief Default constructor for IntraDecoder objects.
	 */
	IntraDecoder() = default;

	/** \fn calc_predictor
	 * 	\brief Calculates prediction value based on selected predictor and a, b and c color values.
	 * 	
	 * 	\param a a pixel color value (x-1, y)
	 * 	\param b b pixel color value (x, y-1)
	 * 	\param c c pixel color value (x-1, y-1)
	 * 	\return Prediction integer value.
	 */
	int calc_predictor(int a, int b, int c);

	/**	\fn	intraDecode
	 * 	\brief	Decodes a frame previously encoded using intraframe encoding.
	 * 			Stores the decoded data on a Mat passed to the function.
	 * 
	 * 	\param frame	Mat where the decoded data is stored.
	 */
	void decode(Mat &frame);
};

/** \class InterEncoder
 *	\brief Class used for encoding video frames using inter-frame motion compensation encoding.
 *	Error values are encoded in a file using Golomb codes.
 */
class InterEncoder {
private:
	/** Value which defines the search block's size. The frame being encoded is divided into
	 *  blocks with size block_size x block_size.
	 */
	int block_size;
	/** Range in which the matching blocks will be searched for in the reference frame. */
	int block_range;
	/** Pointer to GolombEncoder object that will be used for encoding the error values into a file. */
	GolombEncoder *enc;

public:
	/** \fn InterEncoder
	 * 	\brief Constructor for InterEncoder objects.
	 * 
	 * 	\param enc Pointer to GolombEncoder instance that will be used for encoding error values.
	 * 	\param block_size Size of search blocks.
	 * 	\param block_range Block search range.
	 */
	InterEncoder(GolombEncoder *enc, int block_size, int block_range);

	/** \fn InterEncoder 
	 *	\brief Default constructor for InterEncoder objects.
	*/
	InterEncoder() = default;

	/** \fn get_block_size
	 * 	\brief Get method for block_size property.
	 * 
	 * 	\return Integer value of the size of search blocks.
	 */
	int get_block_size();

	/** \fn get_block_range
	 * 	\brief Get method for block_range property.
	 * 
	 * 	\return Integer value of the block search range.
	 */
	int get_block_range();

	/** \fn cost
	 * 	\brief Calculates the total cost of a prediction block.
	 * 	
	 * 	\param block Mat block containing error values.
	 * 	\return A float which represents the cost of the prediction block.
	 */
	float cost(Mat block);

	/** \fn encode
	 * 	\brief Encodes a frame using block-based motion compensation (inter-frame encoding).
	 * 	Divides the curr_frame into blocks of size block_size x block_size, for every block searches
	 * 	within a specific range (block_range) for a block whose errors values (current frame block - previous frame block)
	 *  minimize the cost method.
	 * 	The block and the location vector are stored and later encoded using Golomb codes.
	 * 	
	 * 	\param old_frame Frame which will serve as reference when calculating error values.
	 * 	\param curr_frame Frame which is being encoded.
	 */
	void encode(Mat old_frame, Mat curr_frame);

};

/** \class InterDecoder
 *	\brief Class used to decode video frames from a file encoded using inter-frame motion compensation encoding.
 */
class InterDecoder {
private:
	/** Value which defined the search block size, the encoded frame is divided into blocks
	 *  with size block_size x block_size.
	 */
	int block_size;
	/** Range in which the matching blocks where searched for in the reference frame. */
	int block_range;
	/** GolombDecoder object used for decoding error values from a file. */
	GolombDecoder *dec;

public:
	/** \fn InterDecoder
	 * 	\brief Constructor for InterDecoder objects.
	 * 
	 * 	\param enc Pointer to GolombDecoder instance that will be used for decoding error values from a file.
	 * 	\param block_size Size of search blocks used during encoding.
	 * 	\param block_range Block search range used during encoding.
	 */
	InterDecoder(GolombDecoder *dec, int block_size, int block_range);

	/** \fn InterDecoder
	 * 	\brief Default constructor for InterDecoder objects. 
	 */
	InterDecoder() = default;

	/** \fn decode
	 * 	\brief Decodes a frame from the file encoded using block-based motion compensation (inter-frame encoding).
	 * 	Divides the frame into blocks of size block_size x block_size, decodes each block summing 
	 * 	the prediction value (old_frame's block) and the error value (encoded block's value).
	 * 	Decoded data is stored in curr_frame Mat.
	 * 
	 * 	\param old_frame Reference frame used while encoding the current frame.
	 * 	\param curr_frame Mat that will store the decoded frame.
	 */
	void decode(Mat old_frame, Mat &curr_frame);

};

/** \class HybridEncoder 
 *	\brief Class used for encoding a video using hybrid encoding techniques (both inter and intra frame encoding).
 *	Utilizes the classes InterframeEncoder and IntraframeEncoder to process frames and store them in
 *	an encoded file.
*/
class HybridEncoder {
private:
	/** Video to be encoded. */
	VideoCapture video;
	/** Width of the video. */
	int video_width;
	/** Height of the video. */
	int video_height;
	/** Number of video frames. */
	int video_n_frames;
	/** Predictor used in intra-frame encoding. */
	int predictor;
	/** Block size used in inter-frame encoding. */
	int block_size;
	/** Block range used in inter-frame encoding. */
	int block_range;

public:
	/** \fn HybridEncoder
	 * 	\brief Constructor for HybridEncoder objects.
	 * 	
	 * 	\param video Video which will be encoded.
	 */
	HybridEncoder(VideoCapture video);

	/** \fn encode 
	 * 	\brief Method used to encoded the chosen video using hybrid encoding (inter and intra frame).
	 * 
	 * 	\param output_file File in which the encoded data will be stored.
	*/
	void encode(string output_file);
	Mat encodeyuv(Mat &img); 
};

/** \class HybridDecoder
 * 	\brief Class used for decoding a file encoded using hybrid encoding techniques.
 * 	Processes the encoded file using IntraframeDecoder and InterframeDecoder classes, 
 * 	showing the decoded video.
 */
class HybridDecoder {
private:
	/** File which contains the video encoded using hybrid encoding. */
	string input_file;

public:
	/** \fn HybridDecoder
	 * 	\brief Constructor for HybridDecoder objects.
	 * 	
	 * 	\param video File which will be decoded.
	 */ 
	HybridDecoder(string input_file);

	/** \fn encode 
	 * 	\brief Method used to decode the chosen file using hybrid decoding (inter and intra frame).
	 * 	All necessary properties are read from the encoded file.
	*/
	void decode();
	Mat decodeyuv(Mat &img);
};

#endif