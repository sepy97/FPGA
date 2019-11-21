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

#define ROWS 20
#define COLS 20
#define CHANNELS 3
#define STEP 60

component void grayscale (ihc::stream_in<uchar> &image, const int rows, const int cols, const int channels, const int step, ihc::stream_out<uchar> &grayImage) 
{
	uchar tmp;
	for (int y = 0; y < ROWS; y++) 
	{
		for (int x = 0; x < COLS; x++) 
		{
			int red = (int) image.read();
            		int green = (int) image.read();
            		int blue = (int) image.read();
			
			tmp = (uchar)(.3 * red) + (.59 * green) + (.11 * blue);	
			
			#pragma unroll
			for (int k = 0; k < CHANNELS; k++)
			{
            			grayImage.write (tmp);
			}
        	}
	}

	return;
}

component void blur3x3 (uchar* __restrict__ image, int rows, int cols, int channels, int step, ihc::stream_out<uchar> &blurImage) 
{
 	for (int y = 0; y < ROWS; y++) 
	{
		for (int x = 0; x < COLS; x++) 
		{
			double r_sum = 0, b_sum = 0, g_sum = 0;
			if (x == 0 || y == 0 || x == COLS-1 || y == ROWS-1)
			{
				blurImage.write (r_sum);
				blurImage.write (g_sum);
				blurImage.write (b_sum);
				continue;
			}
			#pragma unroll
			for (int i = -1; i < 2; ++i)
			{
				#pragma unroll
				for (int j = -1; j < 2; ++j)
				{
					double kern_val = gauss_blur3x3[i+1][j+1] / 16.0;
					
					r_sum += kern_val*image[channels*(x+i) + step*(y+j) + 0];/*red*/
					g_sum += kern_val*image[channels*(x+i) + step*(y+j) + 1];/*green*/
					b_sum += kern_val*image[channels*(x+i) + step*(y+j) + 2];/*blue*/
				}
			}
			blurImage.write (r_sum);
			blurImage.write (g_sum);
			blurImage.write (b_sum);
  		}
  	}

    return;
}

component void edge3x3 (uchar* __restrict__ image, int rows, int cols, int channels, int step, ihc::stream_out<uchar> &edgedImage) 
{
    	for (int y = 0; y < ROWS; y++) 
	{
		for (int x = 0; x < COLS; x++) 
		{
			double r_sum = 0, b_sum = 0, g_sum = 0;
			if (x == 0 || y == 0 || x == COLS-1 || y == ROWS-1)
                        {
                                edgedImage.write (r_sum);
                                edgedImage.write (g_sum);
                                edgedImage.write (b_sum);
                                continue;
                        }

			#pragma unroll
			for (int i = -1; i < 2; ++i)
			{
				#pragma unroll
				for (int j = -1; j < 2; ++j)
				{
					double kern_val = edge[i+1][j+1];
					
					r_sum += kern_val*image[channels*(x+i) + step*(y+j) + 0];/*red*/
					if (r_sum < 0) r_sum = 0;
					g_sum += kern_val*image[channels*(x+i) + step*(y+j) + 1];/*green*/
                                        if (g_sum < 0) g_sum = 0;

					b_sum += kern_val*image[channels*(x+i) + step*(y+j) + 2];/*blue*/
                                        if (b_sum < 0) b_sum = 0;

				}
			}
			edgedImage.write (r_sum);
                        edgedImage.write (g_sum);
                        edgedImage.write (b_sum);
  		}
  	}
    return;
}

/*   JUST AN EXAMPLE OF kernel TO BUILD NEW ONE ON ITS BASIS
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
}*/

int main (int argc, char** argv )
{
	ihc::stream_in<uchar> in_image;
        ihc::stream_out<uchar> out_image;

	if ( argc < 3 )
	{
        	printf("usage: DisplayImage.out <Image_Path>\n");
        	return -1;
    	}
	Mat  image;
	printf ("%s \n", argv[1]);
  	image = imread (argv[1], 1);

	if (image.empty()) 
	{
        	printf("No image data \n");
		return -1;
	}
	
	printf ("rows: %d; cols: %d; step: %d; channels: %d\n", image.rows, image.cols, (int)image.step, image.channels());
	
	for (int i = 0; i < image.rows*image.cols*image.channels(); i++)
        {
                in_image.write (image.data[i]);
        }

	unsigned char* __restrict__ tempImageData = (uchar*) malloc (3*sizeof(uchar)*image.rows*image.cols);
	for (int argi = 2; argi < argc; ++argi)
	{
		if (strcmp (argv[argi], "-g") == 0) 
		{
			memset (tempImageData, 0, sizeof(uchar)*image.rows*image.cols);
			grayscale (in_image, image.rows, image.cols, image.channels(), image.step, out_image);
			for(int y = 0; y < image.rows; y++) 
			{
	                	for (int x = 0; x < image.cols; x++) 
				{
					for (int k = 0; k < image.channels(); k++)
					{
						tempImageData [x*image.channels()+k+image.cols*y*image.channels()] = out_image.read();
					}
                	    }
                 	}

    	
		    	memcpy (image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
		}
		else if (strcmp (argv[argi], "-b3") == 0) 
		{
			memset (tempImageData, 25, 3*sizeof(uchar)*image.rows*image.cols);
			blur3x3 (image.data, image.rows, image.cols, image.channels(), image.step, out_image);
			for (int y = 0; y < image.rows; y++) 
			{
                        	for (int x = 0; x < image.cols; x++) 
				{
	    				for (int k = 0; k < image.channels(); k++)
                                	{
                                        	tempImageData [x*image.channels()+k+image.cols*y*image.channels()] = out_image.read();
                                	}
			    }
			}
    			memcpy (image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
		}
		else if (strcmp (argv[argi], "-e") == 0) 
		{
			memset (tempImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
			edge3x3 (image.data, image.rows, image.cols, image.channels(), image.step, out_image);
			for (int y = 0; y < image.rows; y++)
                        {
                                for (int x = 0; x < image.cols; x++)
                                {
                                        for (int k = 0; k < image.channels(); k++)
                                        {
                                                tempImageData [x*image.channels()+k+image.cols*y*image.channels()] = out_image.read();
                                        }
                            }
                        }

			memcpy (image.data, tempImageData, 3*sizeof(uchar)*image.rows*image.cols);
		}
		/*else if (strcmp(argv[argi], "-m") == 0)   JUST FOR TESTING kernel COMPONENT
		{
			int kernel_rows = stoi(argv[++argi]);
			int kernel_cols = stoi(argv[++argi]);
			double* K = new double[kernel_rows*kernel_cols];
			for(int i=0;i<kernel_rows*kernel_cols;++i)
			{
				K[i] = stod(argv[++argi]);
			}
	  	
			memset(tempImageData, 0, 3*sizeof(uchar)*image.rows*image.cols);
			kernel(image.data, image.rows, image.cols, image.channels(), image.step, K, kernel_rows, kernel_cols, tempImageData);
			
			image = Mat(image.rows, image.cols, CV_8UC3, tempImageData);
		}*/
		else
		{
			imwrite (strcat (argv[argi], ""), image);
			image = imread (argv[1], 1 );
		}
	}
	
	return 0;

}
