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

  cv::namedWindow("Image", CV_WINDOW_AUTOSIZE);
  cvShowImage("Image", img);

  //Negative Effect
  cvNot(img, img);
  cv::namedWindow("NegativeEffect", CV_WINDOW_AUTOSIZE);
  cvShowImage("NegativeEffect", img);

  //Wait Key press
  cvWaitKey(0);

  //destroy
  cvDestroyWindow("Image");
  cvDestroyWindow("NegativeEffect");

  return 0;
}
