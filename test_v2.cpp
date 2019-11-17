#include "HLS/hls.h"
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

/*int kernelArray[4][3][3] = {{{-1,-1,-1},{2,2,2},{-1,-1,-1}},
                            {{-1,2,-1},{-1,2,-1},{-1,2,-1}},
                            {{-1,-1,2},{-1,2,-1},{2,-1,-1}},
                            {{2,-1,-1},{-1,2,-1},{-1,-1,2}}};*/

double gauss_blur3x3[3][3] = 	{{1,2,1},
															 {2,4,2},
															 {1,2,1}};
double edge[3][3] = 	{{1,1,1},
											 {1,-8,1},
											 {1,1,1}};
double gauss_blur5x5[5][5] = 	{{1, 4, 6, 4,1},
															 {4,16,24,16,4},
															 {6,24,36,24,6},
															 {4,16,24,16,4},
															 {1, 4, 6, 4,1}};
double scale = 16.0;
int kern_cols = 3;
int kern_length_offset = 1;
int kern_height_offset = 1;

component void grayscale(uchar* __restrict__ image, int rows, int cols, int channels, int step, uchar* __restrict__ grayImage) {
    
    /*grayImage = (uchar*)malloc(sizeof(uchar)*rows*cols);
    memset(grayImage, 0, sizeof(uchar)*rows*cols);*/

//#pragma unroll
	uchar tmp;
#pragma ivdep
	for(int y = 0; y < rows; y++) 
	{
		#pragma unroll        
		for (int x = 0; x < cols; x++) 
		{
			/*int blue = (int)image[channels*x + step*y];
			int green = (int)image[channels*x + step*y + 1];
			int red = (int)image[channels*x + step*y + 2];
			*/
			//grayImage[x + cols*y] = (uchar)\
																(.3 * (int)image[channels*x + step*y  + 0]/*red*/) + \
																(.59 * (int)image[channels*x + step*y + 1]/*green*/) + \
																(.11 * (int)image[channels*x + step*y + 2]/*blue*/);
			tmp = (uchar)\
						(.3 * (int)image[channels*x + step*y  + 0]/*red*/) + \
						(.59 * (int)image[channels*x + step*y + 1]/*green*/) + \
						(.11 * (int)image[channels*x + step*y + 2]/*blue*/);
			grayImage[channels*x + step*y  + 0] = tmp;
			grayImage[channels*x + step*y  + 1] = tmp;
			grayImage[channels*x + step*y  + 2] = tmp;
			
    }
	}

    return;
}

component void blur3x3(uchar* __restrict__ image, int rows, int cols, int channels, int step, uchar* __restrict__ blurImage) {
    
    /*grayImage = (uchar*)malloc(sizeof(uchar)*rows*cols);
    memset(grayImage, 0, sizeof(uchar)*rows*cols);*/

	//#pragma unroll
	#pragma ivdep
	for(int y = 1; y < rows-1; y++) 
	{
		#pragma unroll        
		for (int x = 1; x < cols-1; x++) 
		{
      /*
      int blue = (int)image[channels*x + step*y];
      int green = (int)image[channels*x + step*y + 1];
      int red = (int)image[channels*x + step*y + 2];
      */
			//printf ("bef x:%d y:%d\n", x,y);
			double r_sum = 0,b_sum = 0,g_sum = 0;
			for(int i=-1;i<2;++i)
			{
				for(int j=-1;j<2;++j)
				{
					double kern_val = gauss_blur3x3[i+1][j+1]/16.0;
					r_sum += kern_val*image[channels*(x+i) + step*(y+j)];
					g_sum += kern_val*image[channels*(x+i) + step*(y+j)+1];
					b_sum += kern_val*image[channels*(x+i) + step*(y+j)+2];
					/*if(x==50 && y==50)
					{
						printf ("k:%f i:%d\n", kern_val,image[channels*(x+i) + step*(y+j)+1]);
					}*/
				}
			}
			/*if(x==50 && y==50)
			{
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)],r_sum);
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)+1],g_sum);
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)+2],b_sum);
			}*/
			//printf ("r:%f\n", r_sum);
			blurImage[channels*x + step*y] = (int) r_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+1] = (int) g_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+2] = (int) b_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
    }
  }

    return;
}

component void blur5x5(uchar* __restrict__ image, int rows, int cols, int channels, int step, uchar* __restrict__ blurImage) {
	#pragma ivdep
	for(int y = 2; y < rows-2; y++) 
	{
		#pragma unroll        
		for (int x = 2; x < cols-2; x++) 
		{
			double r_sum = 0,b_sum = 0,g_sum = 0;
			for(int i=-2;i<3;++i)
			{
				for(int j=-2;j<3;++j)
				{
					double kern_val = gauss_blur5x5[i+2][j+2]/256.0;
					r_sum += kern_val*image[channels*(x+i) + step*(y+j)];
					g_sum += kern_val*image[channels*(x+i) + step*(y+j)+1];
					b_sum += kern_val*image[channels*(x+i) + step*(y+j)+2];
					
				}
			}
			/*if(x==50 && y==50)
			{
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)+1],g_sum);
			}*/
			//printf ("r:%f\n", r_sum);
			blurImage[channels*x + step*y] = (int) r_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+1] = (int) g_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+2] = (int) b_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
    }
  }

    return;
}

component void edge3x3(uchar* __restrict__ image, int rows, int cols, int channels, int step, uchar* __restrict__ blurImage) {
    
	#pragma ivdep
	for(int y = 1; y < rows-1; y++) 
	{
		#pragma unroll        
		for (int x = 1; x < cols-1; x++) 
		{
			double r_sum = 0,b_sum = 0,g_sum = 0;
			for(int i=-1;i<2;++i)
			{
				for(int j=-1;j<2;++j)
				{
					double kern_val = edge[i+1][j+1];
					r_sum += kern_val*image[channels*(x+i) + step*(y+j)];
					g_sum += kern_val*image[channels*(x+i) + step*(y+j)+1];
					b_sum += kern_val*image[channels*(x+i) + step*(y+j)+2];
					if(x==50 && y==50)
					{
						printf ("k:%f i:%d\n", kern_val,image[channels*(x+i) + step*(y+j)+1]);
					}
				}
			}
			if(x==50 && y==50)
			{
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)],r_sum);
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)+1],g_sum);
				printf ("old:%d new:%f\n", image[channels*(x) + step*(y)+2],b_sum);
			}
			//printf ("r:%f\n", r_sum);
			blurImage[channels*x + step*y] = (int) r_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+1] = (int) g_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
			blurImage[channels*x + step*y+2] = (int) b_sum;
			//printf ("un:%d blr:%d\n", image[channels*x + step*y],blurImage[channels*x + step*y]);
    }
  }

    return;
}

component void kernel(uchar* __restrict__ inputImage, int im_rows, int im_cols, int channels, int step, double* filter, int ft_rows, int ft_cols, uchar* __restrict__ outputImage) {
	int kern_length_offset = ft_rows>>1;
	int kern_height_offset = ft_cols>>1;
	#pragma ivdep
	for(int y = kern_height_offset; y < im_rows-kern_height_offset; y++) 
	{
		#pragma unroll        
		for (int x = kern_length_offset; x < im_cols-kern_length_offset; x++) 
		{
			double r_sum = 0,b_sum = 0,g_sum = 0;
			for(int i=-kern_height_offset;i<=kern_height_offset;++i)
			{
				for(int j=-kern_length_offset;j<=kern_length_offset;++j)
				{
					double kern_val = filter[ft_cols*(i+kern_height_offset)+(j+kern_length_offset)];
					r_sum += kern_val*inputImage[channels*(x+i) + step*(y+j)];
					g_sum += kern_val*inputImage[channels*(x+i) + step*(y+j)+1];
					b_sum += kern_val*inputImage[channels*(x+i) + step*(y+j)+2];
				}
			}
			outputImage[channels*x + step*y] = (int) r_sum;
			outputImage[channels*x + step*y+1] = (int) g_sum;
			outputImage[channels*x + step*y+2] = (int) b_sum;
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
	//unsigned char* __restrict__ imageData = image.data;
	//printf("here %d\n", image.type());

	if (image.empty())   // !image.data )
	{
//        	printf("No image data \n");
			return -1;
	}

	//Mat processedImage;
	unsigned char* __restrict__ tempImageData = (uchar*)malloc(3*sizeof(uchar)*image.rows*image.cols);
	for(int argi=2;argi<argc;++argi)
	{
		if (strcmp(argv[argi], "-g") == 0) 
		{
			memset(tempImageData, 0, sizeof(uchar)*image.rows*image.cols);
			grayscale(image.data, image.rows, image.cols, image.channels(), image.step, tempImageData);
    	
    	memcpy(image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
    	//image = Mat(image.rows, image.cols, CV_8UC1, tempImageData);
		}
		else if (strcmp(argv[argi], "-b3") == 0) 
		{
			memset(tempImageData, 25, 3*sizeof(uchar)*image.rows*image.cols);
			blur3x3(image.data, image.rows, image.cols, image.channels(), image.step, tempImageData);
    	
    	memcpy(image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
    	//image = Mat(image.rows, image.cols, CV_8UC3);//, tempImageData);
		}
		else if (strcmp(argv[argi], "-b5") == 0) 
		{
			memset(tempImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
			blur5x5(image.data, image.rows, image.cols, image.channels(), image.step, tempImageData);
    	
    	memcpy(image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
    	//image = Mat(image.rows, image.cols, CV_8UC3, tempImageData);
		}
		else if (strcmp(argv[argi], "-e") == 0) 
		{
			memset(tempImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
			edge3x3(image.data, image.rows, image.cols, image.channels(), image.step, tempImageData);
			
			memcpy(image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
    	//image = Mat(image.rows, image.cols, CV_8UC3, tempImageData);
		}
		else if (strcmp(argv[argi], "-m") == 0) 
		{
			int kernel_rows = stoi(argv[++argi]);
			int kernel_cols = stoi(argv[++argi]);
			double* K = new double[kernel_rows*kernel_cols];
			for(int i=0;i<kernel_rows*kernel_cols;++i)
			{
				K[i] = stod(argv[++argi]);
			}
	  	//unsigned char* __restrict__ tempImageData = (uchar*)malloc(3*sizeof(uchar)*image.rows*image.cols);
	  	//double K[9] = {0,0,0,0,1,0,0,0,0};
	  	
			memset(tempImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
			kernel(image.data, image.rows, image.cols, image.channels(), image.step, K, kernel_rows, kernel_cols, tempImageData);
			
			image = Mat(image.rows, image.cols, CV_8UC3, tempImageData);
		}
		else
		{
			imwrite (strcat(argv[argi], ""), image);
			image = imread( argv[1], 1 );
		}
	}
	/*if (strcmp(argv[2], "-g") == 0) 
	{
        	unsigned char* __restrict__ grayImageData = (uchar*)malloc(sizeof(uchar)*image.rows*image.cols);
		memset(grayImageData, 0, sizeof(uchar)*image.rows*image.cols);
		grayscale(imageData, image.rows, image.cols, image.channels(), image.step, grayImageData);
        	processedImage = Mat(image.rows, image.cols, CV_8UC1, grayImageData);

		imwrite (strcat(argv[2], argv[1]), processedImage);
	}
	else if (strcmp(argv[2], "-b3") == 0) 
	{
        	unsigned char* __restrict__ blurImageData = (uchar*)malloc(3*sizeof(uchar)*image.rows*image.cols);
		memset(blurImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
		blur3x3(imageData, image.rows, image.cols, image.channels(), image.step, blurImageData);
        	processedImage = Mat(image.rows, image.cols, CV_8UC3, blurImageData);

		imwrite (strcat(argv[2], argv[1]), processedImage);
	}
	else if (strcmp(argv[2], "-b5") == 0) 
	{
        	unsigned char* __restrict__ blurImageData = (uchar*)malloc(3*sizeof(uchar)*image.rows*image.cols);
		memset(blurImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
		blur5x5(imageData, image.rows, image.cols, image.channels(), image.step, blurImageData);
        	processedImage = Mat(image.rows, image.cols, CV_8UC3, blurImageData);

		imwrite (strcat(argv[2], argv[1]), processedImage);
	}
	else if (strcmp(argv[2], "-e") == 0) 
	{
        	unsigned char* __restrict__ edgeImageData = (uchar*)malloc(3*sizeof(uchar)*image.rows*image.cols);
		memset(edgeImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
		edge3x3(imageData, image.rows, image.cols, image.channels(), image.step, edgeImageData);
        	processedImage = Mat(image.rows, image.cols, CV_8UC3, edgeImageData);

		imwrite (strcat(argv[2], argv[1]), processedImage);
	}*/
	
	//namedWindow("Display Image", WINDOW_AUTOSIZE );

    	//imshow("Display Image", processedImage);
    	//waitKey(0);

//	printf ("%s \n", strcat(argv[2], argv[1]));
	
	return 0;

}
