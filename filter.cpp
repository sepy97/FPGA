#include "process.hpp"

#include <ctime>

int main(int argc, char** argv )
{
    if ( argc < 3 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );

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

    if (strcmp(argv[2], "-l") == 0) {

//        unsigned char* lineImageData = detectLine(image.data, image.rows, image.cols, image.channels(), image.step);
  //      processedImage = Mat(image.rows, image.cols, CV_8UC1, lineImageData);
        
    }   else if (strcmp(argv[2], "-g") == 0) {
        unsigned char* grayImageData = grayscale(image.data, image.rows, image.cols, image.channels(), image.step);
        processedImage = Mat(image.rows, image.cols, CV_8UC1, grayImageData);
        
    }   else if (strcmp(argv[2], "-b") == 0 && argc == 4 && atoi(argv[3]) >= 0) {
    //    unsigned char* blurImageData = blur(image.data, image.rows, image.cols, image.channels(), image.step, atoi(argv[3]));
    //    processedImage = Mat(image.rows, image.cols, CV_8UC3, blurImageData);

    }   else {
        printf("Missing or wrong tag\n");
        return 0;
    }

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


