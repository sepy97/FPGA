//g++ -std=c++11 -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o fltr main.cpp -lsystemc -lm `pkg-config --cflags --libs opencv`

#include <opencv2/opencv.hpp>
#include <ctime>
#include <systemc>
#include "fltr.cpp"

using namespace cv;
using namespace sc_dt;
using namespace sc_core;

int sc_main(int argc, char** argv )
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
	    GRAY func ("GRAY");
	    func.image (image.data);
	    func.rows (image.rows);
	    func.cols (image.cols);
	    func.channels (image.channels());
	    func.step (image.step);
	    func.ret (grayImageData);

        //unsigned char* grayImageData = grayscale(image.data, image.rows, image.cols, image.channels(), image.step);
	sc_start(1, SC_NS);
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

