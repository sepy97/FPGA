#include "HLS/hls.h"
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int kernelArray[4][3][3] = {{{-1,-1,-1},{2,2,2},{-1,-1,-1}},
                            {{-1,2,-1},{-1,2,-1},{-1,2,-1}},
                            {{-1,-1,2},{-1,2,-1},{2,-1,-1}},
                            {{2,-1,-1},{-1,2,-1},{-1,-1,2}}};

extern "C" uchar* grayscale(uchar* image, int rows, int cols, int channels, int step) {
    
    uchar* grayImage = (uchar*)malloc(sizeof(uchar)*rows*cols);
    memset(grayImage, 0, sizeof(uchar)*rows*cols);
        
    for(int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int blue = (int)image[channels*x + step*y];
            int green = (int)image[channels*x + step*y + 1];
            int red = (int)image[channels*x + step*y + 2];
            
            grayImage[x + cols*y] = (uchar)(.3*red) + (.59 * green) + (.11 * blue);

        }
    }
    return grayImage;
}

int main(int argc, char** argv )
{
	if ( argc < 3 )
	{
        	printf("usage: DisplayImage.out <Image_Path>\n");
        	return -1;
    	}
	Mat image;
	printf ("%s \n", argv[1]);
    	image = imread( argv[1], 1 );

	if (image.empty())   // !image.data )
	{
        	printf("No image data \n");
        	return -1;
    	}

    	Mat processedImage;

	if (strcmp(argv[2], "-g") == 0) 
	{
        	unsigned char* grayImageData = grayscale(image.data, image.rows, image.cols, image.channels(), image.step);
        	processedImage = Mat(image.rows, image.cols, CV_8UC1, grayImageData);
	}
	
	//namedWindow("Display Image", WINDOW_AUTOSIZE );

    	//imshow("Display Image", processedImage);
    	//waitKey(0);

	imwrite (strcat(argv[2], argv[1]), processedImage);
	printf ("%s \n", strcat(argv[2], argv[1]));
	
	return 0;

}
