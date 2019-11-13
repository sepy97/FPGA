#include "HLS/hls.h"
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int kernelArray[4][3][3] = {{{-1,-1,-1},{2,2,2},{-1,-1,-1}},
                            {{-1,2,-1},{-1,2,-1},{-1,2,-1}},
                            {{-1,-1,2},{-1,2,-1},{2,-1,-1}},
                            {{2,-1,-1},{-1,2,-1},{-1,-1,2}}};

component void grayscale(uchar* __restrict__ image, int rows, int cols, int channels, int step, uchar* __restrict__ grayImage) {
    
    /*grayImage = (uchar*)malloc(sizeof(uchar)*rows*cols);
    memset(grayImage, 0, sizeof(uchar)*rows*cols);*/

//#pragma unroll
#pragma ivdep
    for(int y = 0; y < rows; y++) {
#pragma unroll        
	for (int x = 0; x < cols; x++) {
            /*int blue = (int)image[channels*x + step*y];
            int green = (int)image[channels*x + step*y + 1];
            int red = (int)image[channels*x + step*y + 2];
            */
            grayImage[x + cols*y] = (uchar)(.3 * (int)image[channels*x + step*y]/*red*/) + (.59 * (int)image[channels*x + step*y + 1]/*green*/) + (.11 * (int)image[channels*x + step*y + 2]/*blue*/);

        }
    }
    return;
}

int main(int argc, char** argv )
{
	if ( argc < 3 )
	{
        	printf("usage: DisplayImage.out <Image_Path>\n");
        	return -1;
    	}
	Mat  image;
	printf ("%s \n", argv[1]);
    	image = imread( argv[1], 1 );
	unsigned char* __restrict__ imageData = image.data;

	if (image.empty())   // !image.data )
	{
//        	printf("No image data \n");
        	return -1;
    	}

    	Mat processedImage;

	if (strcmp(argv[2], "-g") == 0) 
	{
        	unsigned char* __restrict__ grayImageData = (uchar*)malloc(sizeof(uchar)*image.rows*image.cols);
		memset(grayImageData, 0, sizeof(uchar)*image.rows*image.cols);
		grayscale(imageData, image.rows, image.cols, image.channels(), image.step, grayImageData);
        	processedImage = Mat(image.rows, image.cols, CV_8UC1, grayImageData);
	}
	
	//namedWindow("Display Image", WINDOW_AUTOSIZE );

    	//imshow("Display Image", processedImage);
    	//waitKey(0);

	imwrite (strcat(argv[2], argv[1]), processedImage);
//	printf ("%s \n", strcat(argv[2], argv[1]));
	
	return 0;

}
