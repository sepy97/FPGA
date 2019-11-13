// modified from https://github.com/iamrachitgulati/image-filter

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  //Load an Image

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
          printf("No image data \n");
          return -1;
  }

  Mat processedImage;

  cv::namedWindow("Image", CV_WINDOW_AUTOSIZE);
  cvShowImage("Image", image);

  //Negative Effect
  cvNot(image, processedImage);
  cv::namedWindow("NegativeEffect", CV_WINDOW_AUTOSIZE);
  cvShowImage("NegativeEffect", processedImage);

  imwrite (strcat(argv[2], argv[1]), processedImage);

  //Wait Key press
  cvWaitKey(0);

  //destroy
  cvDestroyWindow("Image");
  cvDestroyWindow("NegativeEffect");

  return 0;
}
