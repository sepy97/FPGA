#include <time.h>
#include <stdio.h>
#include "cv.h"
using namespace cv;
using namespace std;

uchar* grayscale (uchar* image, int rows, int cols, int channels, int step);

int kernelArray[4][3][3] = {{{-1,-1,-1},{2,2,2},{-1,-1,-1}},
                            {{-1,2,-1},{-1,2,-1},{-1,2,-1}},
                            {{-1,-1,2},{-1,2,-1},{2,-1,-1}},
                            {{2,-1,-1},{-1,2,-1},{-1,-1,2}}};

int main(int argc, char** argv )
{
/*    if ( argc < 3 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
  */
    Mat image;
    image = imread( "images/duck.jpg", 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat processedImage;

    struct timespec start, finish;
    double elapsed;
    printf("-Starting Image Processor-\n");
    // Start Timer
    clock_gettime(CLOCK_MONOTONIC, &start);

        
    unsigned char* grayImageData = grayscale(image.data, image.rows, image.cols, image.channels(), image.step);
    processedImage = Mat(image.rows, image.cols, CV_8UC1, grayImageData);
        

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time: %f\n", elapsed);
        
    namedWindow("Display Image", WINDOW_AUTOSIZE );

    imshow("Display Image", processedImage);
    waitKey(0);

    return 0;
}

uchar* grayscale(uchar* image, int rows, int cols, int channels, int step) {

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

