#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void convertPixel(double y, double u, double v, unsigned char &r, unsigned char &g, unsigned char &b) {
	b = y + 2.032*u;
	g = y - 0.395*u - 0.581*v;
	r = y + 1.140*v;
	//r = y + (1.4065 * (v - 128));
    //g = y - (0.3455 * (u - 128)) - (0.7169 * (v - 128));
    //b = y + (1.7790 * (u - 128));

}

void realFunc(Mat frame) {
	int step = 0;
	//cout << "Aqui: " << frame.elemSize() << endl;
	//return;
	float *data = (float *)frame.data; 
	int size = frame.cols * frame.rows;
	Mat result = Mat::zeros(frame.size(), CV_16SC3);
	//uchar *dataRGB = (uchar *)result.data; 
	/*
	for(int i = 0, j=0; i < frame.cols * frame.rows * 3; i+=6, j+=4)
	{
		result.data[i] = data[j] + data[j+3]*((1 - 0.299)/0.615);
		result.data[i+1] = data[j] - data[j+1]*((0.114*(1-0.114))/(0.436*0.587)) - data[j+3]*((0.299*(1 - 0.299))/(0.615*0.587));
		result.data[i+2] = data[j] + data[j+1]*((1 - 0.114)/0.436);
		result.data[i+3] = data[j+2] + data[j+3]*((1 - 0.299)/0.615);
		result.data[i+4] = data[j+2] - data[j+1]*((0.114*(1-0.114))/(0.436*0.587)) - data[j+3]*((0.299*(1 - 0.299))/(0.615*0.587));
		result.data[i+5] = data[j+2] + data[j+1]*((1 - 0.114)/0.436);
	}
	*/
	
	for (int i = 0; i<frame.rows; i++)
	{
		for (int j=0; j<frame.cols; j++)
		{
			float Y = round(data[i*step + j]);
			float U = round(data[ (int)(size + (i/2)*(step/2)  + j/2) ]);
			float V = round(data[ (int)(size*1.25 + (i/2)*(step/2) + j/2)]);

			int R = Y-16 + 1.402 * (V - 128);
			int G = Y-16 - 0.344 * (U - 128) - 0.714 * (V - 128);
			int B = Y-16 + 1.772 * (U - 128);

			if (R < 0){ R = 0; } if (G < 0){ G = 0; } if (B < 0){ B = 0; }
			if (R > 255 ){ R = 255; } if (G > 255) { G = 255; } if (B > 255) { B = 255; }

			result.at<Vec3i>(i,j).val[0] = B;
			result.at<Vec3i>(i,j).val[1] = G;
			result.at<Vec3i>(i,j).val[2] = R;

		}
	}
	
	imshow("Wahteva", result);
	if (waitKey(10) == 27) {destroyAllWindows();};

}

Mat yuvTorgb(Mat frame) {
	Mat channels[3];

	split(frame, channels);

	int height = frame.rows;
	int width = frame.cols;
	//double y, u, v;
	int y, u, v;
	double r, g, b;


	Mat result = Mat::zeros(frame.size(), CV_8UC3);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			//cout << "(" << row << ", " << col << ")" << endl;
			cout << channels[0] << endl;
			return result;
			y = channels[0].at<int>(row, col);
			v = channels[1].at<int>(row, col);
			u = channels[2].at<int>(row, col);
			//convertPixel(y, u, v, r, g, b);

			b = y + 2.032*u;
			g = y - 0.395*u - 0.581*v;
			r = y + 1.140*v;

			if (r < 0) r = 0;
			else if (r > 255) r = 255;
			if (g < 0) g = 0;
			else if (g > 255) g = 255;
			if (b < 0) b = 0;
			
			result.at<Vec3b>(row, col).val[0] = r;
			result.at<Vec3b>(row, col).val[1] = g;
			result.at<Vec3b>(row, col).val[2] = b;
				
			//result.at<Vec3b>(row, col).val[2] = (r < 0) ? 0 : (r > 255) ? 255 : r;
			//result.at<Vec3b>(row, col).val[1] = (g < 0) ? 0 : (g > 255) ? 255 : g;
			//result.at<Vec3b>(row, col).val[0] = (b < 0) ? 0 : (b > 255) ? 255 : b;
			//cout << "(" << int(result.at<Vec3b>(row, col).val[2]) << ", " << int(result.at<Vec3b>(row, col).val[1]) << ", " << int(result.at<Vec3b>(row, col).val[0]) << ")" << endl; 
		}
	}

	return result;
}


void DisplayYUV()  
{  
    int w = 352;  
    int h = 288;  
	int n_frames = 300;
  
    FILE* pFileIn = fopen("Resources/akiyo_cif.yuv", "rb+");  
    int bufLen = w*h*1.5;  
    unsigned char* pYuvBuf = new unsigned char[bufLen];  
    int iCount = 0;  
  
    for(int i=0; i<n_frames; i++)
    {  
        fread(pYuvBuf, bufLen*sizeof(unsigned char), 1, pFileIn);  
		cout << "Ya" << endl;
        Mat yuvImg;  
        yuvImg.create(h*3/2, w, CV_8UC1);   
        memcpy(yuvImg.data, pYuvBuf, bufLen*sizeof(unsigned char));  
        Mat rgbImg;  
        cvtColor(yuvImg, rgbImg,  COLOR_YUV2BGR_IYUV);  

        imshow("img", rgbImg);  
        waitKey(30);  
  
        printf("%d \n", iCount++);  
    }  
  
    delete[] pYuvBuf;  
  
  
    fclose(pFileIn);  
} 

int main() {
	DisplayYUV();
	/*
	//int width = 352, height = 288, n_frames = 300;
	string filename = "Resources/ducks_take_off_422_720p50.y4m";
	VideoCapture cap = VideoCapture(filename);
	Mat frame;

	while (true) {
		cout << "New frame" << endl;
		cap >> frame;
		if (frame.empty())
			break;
		realFunc(frame);
	} 
	*/
	//fstream fp(filename, fstream::in | fstream::binary);
	//unsigned char y, u, v;

	//while (true) {
		//cout << fp.read(width*height) << endl;
	//}
	return 0;

}