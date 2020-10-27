/*! \file	VideoProcessing.cpp
 *	\brief	Video Player with image processing and video compression
 *			capabilities.
 *	
 *	File VideoProcessing.cpp is a program used to display a video with several image processing 
 *	options and video compression between RGB and YUV.
 *	Press ESC to exit.
 *
 */
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

RNG rng( 0xFFFFFFFF );

/*!	\fn Mat applyWatermark(Mat img, Mat mark)
 *	\brief Apply a watermark to the given image
 *
 * 	\param img source image
 * 	\param mark watermark that will be applied
 * 
 * 	\return watermarked image
 */	
Mat applyWatermark(Mat img, Mat mark) {
	Size textsize = getTextSize("OpenCV forever!", FONT_HERSHEY_COMPLEX, 3, 5, 0);
  	Point org((img.cols - textsize.width)/2, (img.rows - textsize.height)/2);
	putText(img, "Example Text", org, FONT_HERSHEY_DUPLEX, 3,
           Scalar(0, 0, 255), 5, 4 );
	return img;
}

int main() {
	string filename = "resources/ducks_take_off1080.y4m";
	string m = "watmark.jpg";
	VideoCapture video = VideoCapture(filename);
	int op;

	if (!video.isOpened()) {
		cout << "Error opening specified file.\n";
		return -1;
	}

	cout << " ----- Options Menu -----\n";
	cout << " 1 -> Apply Watermark\n";
	cout << " 2 -> Convert to HSV\n";
	cout << " 3 -> Convert to YUV\n";
	cout << " 4 -> Show Color Histogram\n";
	cout << " 5 -> Convert to grayscale and equalize histogram\n";
	cout << " 6 -> Apply gaussean filter\n";
	cout << " 7 -> \n";
	cout << " 8 -> Edge Detection Algorithms\n";
	cin >> op;
	switch (op){
		case 1:
			while (true) {
				Mat frame;
				video >> frame;
				if (frame.empty()) {break;}
				imshow("whatever", applyWatermark(frame, imread(m, IMREAD_COLOR)));
				if (waitKey(10) == 27) {break;};
			}
			break;
		case 2:
			while (true) {
				Mat frame, hsvImg, emptyMat;
				vector<Mat> hsvChannels(3);

				video >> frame;
				if (frame.empty()) {break;}

				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				cvtColor(frame, hsvImg, COLOR_BGR2HSV);
				split(hsvImg, hsvChannels);

				namedWindow("H Channel", WINDOW_AUTOSIZE);
				imshow("H Channel", hsvChannels[0]);
				namedWindow("S Channel", WINDOW_AUTOSIZE);
				imshow("S Channel", hsvChannels[1]);
				namedWindow("V Channel", WINDOW_AUTOSIZE);
				imshow("V Channel", hsvChannels[2]);
				namedWindow("HSV Video", WINDOW_AUTOSIZE);
				imshow("HSV Video", hsvImg);
				if (waitKey(10) == 27) {break;};
			}
			break;
		case 3:
			while (true) {
				Mat frame, yuvImg, emptyMat;
				vector<Mat> yuvChannels(3);

				video >> frame;
				if (frame.empty()) {break;}

				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				cvtColor(frame, yuvImg, COLOR_BGR2YUV);
				split(yuvImg, yuvChannels);

				namedWindow("Y Channel", WINDOW_AUTOSIZE);
				imshow("Y Channel", yuvChannels[0]);
				namedWindow("U Channel", WINDOW_AUTOSIZE);
				imshow("U Channel", yuvChannels[1]);
				namedWindow("V Channel", WINDOW_AUTOSIZE);
				imshow("V Channel", yuvChannels[2]);
				namedWindow("YUV Video", WINDOW_AUTOSIZE);
				imshow("YUV Video", yuvImg);
				if (waitKey(10) == 27) {break;};
			}
			break;

		case 4:
			while (true) {
				Mat frame, yuvImg;
				Mat bgrChannels[3];
				int histSize = 256; //RGB has values between [0, 255]
				float range[] = { 0, 256 };
    			const float* histRange = { range };

				video >> frame;
				if (frame.empty()) {break;}
				split(frame, bgrChannels);

				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				Mat b_hist, g_hist, r_hist;
				calcHist( &bgrChannels[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, true, false);
				calcHist( &bgrChannels[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, true, false);
				calcHist( &bgrChannels[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, true, false);
				
				// Image to display histogram
				int hist_w = 512, hist_h = 400;
				int bin_w = cvRound( (double) hist_w/histSize );
				Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

				normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
				normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
				normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

				for( int i = 1; i < histSize; i++ ) {
					line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
						Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
						Scalar( 255, 0, 0), 2, 8, 0  );
					line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
						Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
						Scalar( 0, 255, 0), 2, 8, 0  );
					line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
						Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
						Scalar( 0, 0, 255), 2, 8, 0  );
				}

				imshow("Source image", frame);
				imshow("Histogram", histImage);
				if (waitKey(10) == 27) {break;};
			}

			break;

		case 5:
			while (true) {
				Mat frame, gsImg;
				vector<Mat> yuvChannels(3);

				video >> frame;
				if (frame.empty()) {break;}

				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				cvtColor(frame, gsImg, COLOR_BGR2GRAY);
				equalizeHist(gsImg, gsImg);
				imshow("Grayscale image", gsImg);
				if (waitKey(10) == 27) {break;};
			}

			break;

		case 6: // TODO: Add more filters
			while (true) {
				Mat frame, filteredImg;
				vector<Mat> yuvChannels(3);

				video >> frame;
				if (frame.empty()) {break;}
				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				GaussianBlur(frame, filteredImg, Size(31, 31), 0, 0);
				imshow("Blurred image", filteredImg);
				if (waitKey(10) == 27) {break;};
			}
			break;

		case 7: //TODO: Segmentation Algorithms

			break;

		case 8: 
			while (true) {
				Mat frame, gsImg;
				vector<Mat> yuvChannels(3);

				video >> frame;
				if (frame.empty()) {break;}
				resize(frame, frame, Size(frame.cols/3, frame.rows/3));
				// Convert image to grayscale (Sobel operator only accepts single channel images)
				cvtColor(frame, gsImg, COLOR_BGR2GRAY);
				Sobel(gsImg, gsImg, CV_16S, 1, 0);
				imshow("Sobel image", gsImg);
				if (waitKey(10) == 27) {break;};
			}

			break;

	}

	return 0;
}