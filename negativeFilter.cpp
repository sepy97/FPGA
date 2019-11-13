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
  IplImage* image;
  image = cvLoadImage( argv[1]);

  if (argv[1] == null)   // !image.data )
  {
          printf("No image parameter \n");
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
