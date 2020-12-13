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
 * 	\return Watermarked image
 */	
Mat applyWatermark(Mat srcImg, string op, string input) {
	if (op == "text") {
		Size textsize = getTextSize(input, FONT_HERSHEY_COMPLEX, 3, 5, 0);
		Point org(0, (srcImg.rows - textsize.height)); //Bottom left corner of text location

		putText(srcImg, input, org, FONT_HERSHEY_DUPLEX, 3,
			Scalar(0, 0, 255), 3, 4 );
		return srcImg;

	} else {
		Mat img = imread(input);

		if (img.empty()) {
			cout << "Image file not found.\n";
			return Mat();
		}

		resize(img, img, Size(srcImg.cols, srcImg.rows));
		return (0.8*srcImg + 0.2*img);
	}
}

/*!	\fn Mat splitHSV(Mat bgrImg)
 *	\brief converts a bgr image to hsv, splits the 3 channels and shows the results
 *
 * 	\param bgrImg source image in bgr
 * 
 * 	\return void
 */	
void splitHSV(Mat bgrImg) {
	Mat hsvImg;
	Mat channels[3];

	cvtColor(bgrImg, hsvImg, COLOR_BGR2HSV);
	split(hsvImg, channels);

	namedWindow("H Channel", WINDOW_AUTOSIZE);
	imshow("H Channel", channels[0]);

	namedWindow("S Channel", WINDOW_AUTOSIZE);
	imshow("S Channel", channels[1]);
	
	namedWindow("V Channel", WINDOW_AUTOSIZE);
	imshow("V Channel", channels[2]);

	namedWindow("HSV Video", WINDOW_AUTOSIZE);
	imshow("HSV Video", hsvImg);

}

/*!	\fn Mat splitYUV(Mat bgrImg)
 *	\brief converts a bgr image to yuv, splits the 3 channels and shows the results
 *
 * 	\param bgrImg source image in bgr
 * 
 * 	\return void
 */	
void splitYUV(Mat bgrImg) {
	Mat yuvImg;
	Mat channels[3];

	cvtColor(bgrImg, yuvImg, COLOR_BGR2YUV);
	split(yuvImg, channels);

	namedWindow("Y Channel", WINDOW_AUTOSIZE);
	imshow("Y Channel", channels[0]);

	namedWindow("U Channel", WINDOW_AUTOSIZE);
	imshow("U Channel", channels[1]);

	namedWindow("V Channel", WINDOW_AUTOSIZE);
	imshow("V Channel", channels[2]);

	namedWindow("YUV Video", WINDOW_AUTOSIZE);
	imshow("YUV Video", yuvImg);

}

/*!	\fn Mat colorHistogram(Mat bgrImg)
 *	\brief calculates the color histogram from a given picture
 *
 * 	\param bgrImg source image in bgr
 * 
 * 	\return color histogram
 */	
Mat colorHistogram(Mat img) {
	Mat bgrChannels[3];
	Mat b_hist, g_hist, r_hist;
	int histSize = 256; //RGB has values between [0, 255]
	float range[] = { 0, 256 };
	const float* histRange = { range };

	split(img, bgrChannels);
	calcHist(&bgrChannels[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, true, false);
	calcHist(&bgrChannels[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, true, false);
	calcHist(&bgrChannels[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, true, false);
	
	// Image to display histogram
	int hist_w = 512, hist_h = 400; // Histogram width and height
	int bin_w = cvRound((double) hist_w/histSize);
	Mat histogram(hist_h, hist_w, CV_8UC3, Scalar(0,0,0));

	normalize(b_hist, b_hist, 0, histogram.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histogram.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histogram.rows, NORM_MINMAX, -1, Mat());

	for( int i = 1; i < histSize; i++ ) {
		line(histogram, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
			Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
			Scalar( 255, 0, 0), 2);
		line(histogram, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
			Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
			Scalar( 0, 255, 0), 2 );
		line(histogram, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
			Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
			Scalar( 0, 0, 255), 2);
	}

	return histogram;
}

/*!	\fn Mat cannyEdge(Mat image, int lowerThreshold)
 *	\brief applies the Canny Edge Detection algorithm to a given picture based on a lowerthreshold
 *
 * 	\param image source image
 *  \param lowerThreshold lower threshold used in Canny() function
 * 
 * 	\return edge detected image
 */	
Mat cannyEdge(Mat image, int lowerThresh) {
	Mat edges, result, imgGray;

	cvtColor( image, imgGray, COLOR_BGR2GRAY );
	blur( imgGray, edges, Size(3,3) );

    Canny( edges, edges, lowerThresh, lowerThresh*3, 3 );

    result = Scalar::all(0);
    image.copyTo( result, edges);
    return result;

}

int main() {
	string filename = "Resources/ducks_take_off_1080p50.y4m";
	string m = "watmark.jpg";
	VideoCapture video = VideoCapture(filename);
	int op;

	if (!video.isOpened()) {
		cout << "Error opening specified file.\n";
		return -1;
	}
	//while (true) {
		
		cout << "\n ----- Options Menu -----\n";
		cout << " 1 -> Apply Watermark\n";
		cout << " 2 -> Convert to HSV\n";
		cout << " 3 -> Convert to YUV\n";
		cout << " 4 -> Show Color Histogram\n";
		cout << " 5 -> Convert to grayscale and equalize histogram\n";
		cout << " 6 -> Apply gaussean filter\n";
		cout << " 7 -> -----\n";
		cout << " 8 -> Edge Detection Algorithms (Sobel)\n";
		cout << " 9 -> Apply Canny Edge Detection\n";
		cout << " 10 -> Open video in YUV format\n";
		cout << " 0 -> Exit Program\n";
		//cin.clear();
		cin >> op;
		//cin.get();

		switch (op){
			case 1: {
				int wtrmrkOp;
				Mat frame;

				cout << "\n 1 -> Text Watermark\n";
				cout << " 2 -> Image watermark\n";
				cin >> wtrmrkOp;

				if (wtrmrkOp == 1) {
					cout << " Write desired text:\n";
					string text;
					cin >> text;

					while (true) {
						video >> frame;
						if (frame.empty()) {destroyAllWindows();break;}
						//resize(frame, frame, Size(frame.cols/2, frame.rows/2));
						namedWindow("Watermarked Image", WINDOW_AUTOSIZE);
						imshow("Watermarked Image", applyWatermark(frame, "text", text));
						if (waitKey(10) == 27) {destroyAllWindows();break;};
					}
				} else {
					cout << " Image path:\n";
					string filepath;
					cin >> filepath;

					while (true) {
						video >> frame;
						if (frame.empty()) {destroyAllWindows();break;};
						//resize(frame, frame, Size(frame.cols/2, frame.rows/2));
						namedWindow("Watermarked Image", WINDOW_AUTOSIZE);
						imshow("Watermarked Image", applyWatermark(frame, "image", filepath));
						if (waitKey(10) == 27) {destroyAllWindows();break;};
					}
				}
				break;
			}
			case 2:
				while (true) {
					Mat frame;

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};

					resize(frame, frame, Size(frame.cols/2, frame.rows/2));
					splitHSV(frame);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}
				break;
			case 3:
				while (true) {
					Mat frame, yuvImg, emptyMat;
					vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};

					resize(frame, frame, Size(frame.cols/2, frame.rows/2));
					splitYUV(frame);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}
				break;

			case 4:
				while (true) {
					Mat frame;
					Mat histogram;

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};
					resize(frame, frame, Size(frame.cols/2, frame.rows/2));

					histogram = colorHistogram(frame);
					namedWindow("Source image", WINDOW_AUTOSIZE);
					imshow("Source image", frame);
					namedWindow("Histogram", WINDOW_AUTOSIZE);
					imshow("Histogram", histogram);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}

				break;

			case 5:
				while (true) {
					Mat frame, gsImg;
					vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};

					resize(frame, frame, Size(frame.cols/2, frame.rows/2));
					cvtColor(frame, gsImg, COLOR_BGR2GRAY);
					equalizeHist(gsImg, gsImg);
					namedWindow("Grayscale Image", WINDOW_AUTOSIZE);
					imshow("Grayscale Image", gsImg);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}

				break;

			case 6: // TODO: Add more filters
			{
				int kernel = 1;
				int frameCount = 0;
				while (true) {
					Mat frame, filteredImg;
					vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};
					// resize(frame, frame, Size(frame.cols/2, frame.rows/2));
					GaussianBlur(frame, filteredImg, Size(kernel, kernel), 0, 0);
					namedWindow("Blurred Image", WINDOW_AUTOSIZE);
					imshow("Blurred Image", filteredImg);
					frameCount+=1;
					if (frameCount % 20 == 0)
						kernel+=2;
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}
				break;
			}

			case 7: //TODO: Segmentation Algorithms

				break;

			case 8: 
				while (true) {
					Mat frame, gsImg, gradx, grady;
					vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};
					// resize(frame, frame, Size(frame.cols/2, frame.rows/2));
					// Convert image to grayscale (Sobel operator only accepts single channel images)
					GaussianBlur(frame, gsImg, Size(3, 3), 0, 0, BORDER_DEFAULT);
					cvtColor(frame, gsImg, COLOR_BGR2GRAY);
					
					Sobel(gsImg, gradx, CV_16S, 1, 0, 3);
					Sobel(gsImg, grady, CV_16S, 0, 1, 3);

					convertScaleAbs(gradx, gradx);
					convertScaleAbs(grady, grady);
					addWeighted(gradx, 0.5, grady, 0.5, 0, frame);
					namedWindow("Sobel Image", WINDOW_AUTOSIZE);
					imshow("Sobel Image", frame);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}

				break;
			case 9:
			{
				int threshold = 150;
				int frameCount = 0;
				while (true) {
					Mat frame, edgedImage;
					vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};

					edgedImage = cannyEdge(frame, threshold);
					namedWindow("Canny Edge Detection", WINDOW_AUTOSIZE);
					imshow("Canny Edge Detection", edgedImage);
					frameCount+=1;
					if (frameCount % 5 == 0)
						threshold-=1;
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}
				break;
			}
			case 10: {
				video = VideoCapture("resources/ducks_take_off_422_720p50.y4m");
				int size, i, j, n, height, width;
				Mat py, pu, pv;
				Mat r, g, b, finished, finishedImg;
				vector<Mat> yuvChannels, result;
				while (true) {
					Mat frame;
					//vector<Mat> yuvChannels(3);

					video >> frame;
					if (frame.empty()) {destroyAllWindows();break;};
					// Begin processing frame
					height = frame.rows;
					width = frame.cols;
					/*
					// YUV444 to RGB
					Mat r, g, b, finishedImg;
					vector<Mat> channels, result;
					split(frame, channels);

					r = channels[0] + 1.140*channels[2];
					g = channels[0] - 0.395*channels[1] - 0.581*channels[2];
					b = channels[0] + 2.032*channels[1];

					result.push_back(r);
					result.push_back(g);
					result.push_back(b);
					merge(result, finishedImg);

					imshow("Resulting Image", finishedImg);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
					// -------------
					*/
					yuvChannels.clear();
					py = Mat::zeros(height, width, CV_8UC1);
					pu = Mat::zeros(height, width, CV_8UC1);
					pv = Mat::zeros(height, width, CV_8UC1);
					finished = Mat::zeros(height, width, CV_8UC3);
					finishedImg = Mat::zeros(height, width, CV_8UC3);

					i, j, n;
					size = height*width;
					split(frame, yuvChannels);
					for (i = 0;i<height-1;i++){
						for (j = 0; j<width-1; j++){
							py.at<float>(i,j) = frame.at<float>(i,j);
						}
					}

					for (i = 0;i<height-1;i++){
						for (j = 0, n = 1; j<width-1;n+=3, j+=4){
							pu.at<float>(i,n) = frame.at<float>(i,j);
							pu.at<float>(i,n+1) = frame.at<float>(i,j);
						}
					}

					for (i = 0;i<height-1;i++){
						for (j = 3, n = 2; j<width-1;n+=3, j+=4){
							pv.at<float>(i,n) = frame.at<float>(i,j);
							pv.at<float>(i,n+1) = frame.at<float>(i,j);
						}
					}
					// vector<Mat> channels;
					//channels.push_back(pv);
					//channels.push_back(pu);
					//channels.push_back(py);

					// merge(channels, finished);

					r = py + 1.140*pv;
					g = py - 0.395*pu - 0.581*pv;
					b = py + 2.032*pu;

					result.push_back(r);
					result.push_back(g);
					result.push_back(b);
					
					merge(result, finishedImg);
					
					// ------
					namedWindow("YUV2BGR Image", WINDOW_AUTOSIZE);
					imshow("YUV2BGR Image", finishedImg);
					if (waitKey(10) == 27) {destroyAllWindows();break;};
				}
				break;
			}
			case 0:
				
				break;
			default:
				cout << "Invalid option!\n";
				break;
		}
	//}

	return 0;
}