/*! \file	CompleteEncoder.cpp
 *	\brief	Video encoder based on Golomb codes and predictive coding. 
 *			Takes in a file to encode and the predictor type
 *
 */

#include "opencv2/opencv.hpp"
#include "Golomb.h"
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

/*! \fn encodeFrame
*	\brief Encode a frame using Golomb codes and predictive coding
*	
*	\param frame Frame that will be encoded
*	\param g Golomb encoder object
*	\param predictor Predictor type from 1-8, 8 is Jpeg-LS
*/
void encodeFrame(Mat frame, GolombEncoder &g, int predictor) {
	int colorVal, pred, a, b, c, mini, maxi;

	// Padding the frame with outter zeros
	Mat image;
	hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
	vconcat(Mat::zeros(1, frame.cols+1, CV_8UC3), image, image);

	for (int ch = 0; ch < 3 ; ch++) {
		for (int i = 1; i < image.rows; i++) {
			for (int n = 1; n < image.cols; n++) {
				a = image.at<Vec3b>(i, n-1).val[ch];
				b = image.at<Vec3b>(i-1, n).val[ch];
				c = image.at<Vec3b>(i-1, n-1).val[ch];

				switch (predictor) {
					case 1:
						// Predictor = previous pixel value
						pred = a;
						break;
					case 2:
						// Predictor = top pixel value
						pred = b;
						break;
					case 3:
						// Predictor = previous pixel value (diagonal)
						pred = c;
						break;
					case 4:
						// Predictor = previous pixel value -> ( a + b - c )
						pred = a + b - c;
						break;
					case 5:
						// Predictor = previous pixel value -> ( a + (b - c)/2 )
						pred = a + (b - c) / 2;
						break;
					case 6:
						// Predictor = previous pixel value -> (b  + (a - c)/2 )
						pred = b + (a - c) / 2;
						break;
					case 7:
						// Predictor = previous pixel value -> ( a + b ) / 2
						pred = (a + b)/2;
						break;
					case 8:
						pred = a+b-c;
						
						mini = min(a, b);
						maxi = max(a, b);

						if (c >= maxi) {
							pred = mini;
							break;
						}
						if (c <= mini)
							pred = maxi;
						break;
				}

				// image.at<Vec3b>(i,n) -> Retrieve pixel at location (i, n)
				// .val[ch] -> Retrieve color value from pixel [b,g,r]
				colorVal = image.at<Vec3b>(i, n).val[ch];
				// Encode Error = estimate - real value
				g.encode(colorVal - pred);
			}
		}
	}

}

/*! \fn decode
*	\brief Decode a video based on a predictor type
*	
*	\param path Path to file that will be decoded
*	\param g Golomb encoder object
*	\param predictor Predictor type from 1-8, 8 is Jpeg-LS
*/
void decodeVideo(string path) {
	int err;
	unsigned char a, b, c, pred, mini, maxi;
	Mat frame;

	GolombDecoder dec(path);

	int predictor = dec.decode();
	int width = dec.decode();
	int height = dec.decode();
	int nFrames = dec.decode();

	for (int nFrame = 0; nFrame < nFrames; nFrame++){
		vector<Mat> encodedData;
		frame = Mat::zeros(height, width, CV_8UC3);
		for (int ch = 0; ch < 3; ch++){
			for (int i = 0; i < height; i++) {
				for (int n = 0; n < width; n++) {
					if (n == 0)
						a = 0;
					else
						a = frame.at<Vec3b>(i, n-1).val[ch];
					if (i == 0)
						b = 0;
					else
						b = frame.at<Vec3b>(i-1, n).val[ch];
					if (i == 0 || n == 0)
						c = 0;
					else
						c = frame.at<Vec3b>(i-1, n-1).val[ch];
					
					switch (predictor) {
						case 1:
							// Predictor = previous pixel value
							pred = a;
							break;
						case 2:
							// Predictor = top pixel value
							pred = b;
							break;
						case 3:
							// Predictor = previous pixel value (diagonal)
							pred = c;
							break;
						case 4:
							// Predictor = previous pixel value -> ( a + b - c )
							pred = a + b - c;
							break;
						case 5:
							// Predictor = previous pixel value -> ( a + (b - c)/2 )
							pred = a + (b - c) / 2;
							break;
						case 6:
							// Predictor = previous pixel value -> (b  + (a - c)/2 )
							pred = b + (a - c) / 2;
							break;
						case 7:
							// Predictor = previous pixel value -> ( a + b ) / 2
							pred = (a + b)/2;
							break;
						case 8:
							pred = a+b-c;

							mini = min(a, b);
							maxi = max(a, b);

							if (c >= maxi) {
								pred = mini;
								break;
							}
							if (c <= mini)
								pred = maxi;
							break;
					}
					err = dec.decode();
					frame.at<Vec3b>(i, n).val[ch] = pred+err;
				}
			}
		}

		cout << "Showing frame N" << nFrame << endl;
		namedWindow("Image", WINDOW_AUTOSIZE);
		imshow("Image", frame);
		if (waitKey(10) == 27) {destroyAllWindows();break;}; // Wait for a keystroke in the window
		
	}
	//imshow("Image", frame);
	//int k = waitKey(0); // Wait for a keystroke in the window
}

void intraEncode() {
	
}

int main(int argc, char** argv) {
	VideoCapture video = VideoCapture("Resources/ducks_take_off_1080p50.y4m");
	Mat frame;

	int predictor = 8;
	GolombEncoder enc("Encoded.bin", 3);

	enc.encode(predictor);
	enc.encode(video.get(CAP_PROP_FRAME_WIDTH));
	enc.encode(video.get(CAP_PROP_FRAME_HEIGHT));
	enc.encode(video.get(CAP_PROP_FRAME_COUNT));
	int count = 0;
	while (true) {
		count++;
		video >> frame;
		if (frame.empty()) {break;};
		encodeFrame(frame, enc, predictor);
		cout << "Encoded frame " << count << endl;
	}
	enc.finishEncoding();

	//decodeVideo("Encoded.bin");

	/*
	if (argc != 4) {
        cout << "Usage: " << argv[0] << " \'Input_File_Path\' \'Ouput_File_Path\' \'Predictor_Type_[1,8]\' " << endl;
        return 1;
    }
	*/
	//Mat img = imread(argv[1], IMREAD_COLOR);

	//if (img.empty()) {
	//	cout << "Error opening file." << endl;
	//	return 1;
	//}
	/*
	if (atoi(argv[2]) < 1 || atoi(argv[2]) > 8) {
		cout << "Invalid predictor option." << endl;
		return 1;
	}
	
	Golomb g("EncodedFile.bin", 4, img.cols, img.rows, 1);

	encodeFrame(img, g, atoi(argv[2]));

	decodeVideo(g, atoi(argv[2]));
	*/
	/*************************************************************
	 * Implementation for video coding
	VideoCapture video = VideoCapture(argv[1]);
	Mat frame;
	GolombEncoder enc("EncodedFile.bin", 2);
	enc.encode(argv[3]);

	int count = 0;
	
	while (true) {
		count++;
		video >> frame;
		if (frame.empty()) {break;}
		cout << "Encoding frame " << count << endl;
		if (count==10) {break;}
	}
	gol.finishEncoding();

	decodeVideo(argv[2], gol);
	return 0;
	*************************************************************/

	/*************************************************************
	 * Tests
	Mat testMat = Mat::zeros(3, 4, CV_8UC3);
	int data[36] = {1,2,2,3,2,1,1,1,2, 4,3,2,4,5,1,2,4,5,1, 4,1,2,3,4,6,1,2,2,1,1, 3,2,3,1,3,4};
	int count = 0;
	count = 0;
	for (int ch = 0; ch < 3; ch++) {
		for (int i = 0; i < 3; i++) {
			for (int n = 0; n < 4; n++) {
				testMat.at<Vec3b>(i, n).val[ch] = data[count];
				count++;
			}
		}
	}
	cout << testMat << endl;
	Golomb g("encoded.bin", 3, testMat.cols, testMat.rows, 1);
	encodeFrame(testMat, g);

	decodeVideo(g);
	*************************************************************/
}