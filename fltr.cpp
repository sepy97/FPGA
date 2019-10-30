#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <systemc>

using namespace cv;
using namespace std;
using namespace sc_dt;
using namespace sc_core;

int kernelArray[4][3][3] = {{{-1,-1,-1},{2,2,2},{-1,-1,-1}},
                            {{-1,2,-1},{-1,2,-1},{-1,2,-1}},
                            {{-1,-1,2},{-1,2,-1},{2,-1,-1}},
                            {{2,-1,-1},{-1,2,-1},{-1,-1,2}}};

SC_MODULE (GRAY)
{
	sc_in< uchar* > image;
	sc_in< int > rows, cols, channels, step;
	sc_out< uchar* > ret;

	void grayscale ()
	{
	    uchar* g_image;
	    int g_rows, g_cols, g_channels, g_step;
	
	    g_image    = image.read();
	    g_rows     = rows.read();
 	    g_cols     = cols.read();
	    g_channels = channels.read();
	    g_step     = step.read();

	    uchar* grayImage = (uchar*)malloc(sizeof(uchar)*g_rows*g_cols);
	    memset(grayImage, 0, sizeof(uchar)*g_rows*g_cols);
        
	    for(int y = 0; y < g_rows; y++) 
	    {
	        for (int x = 0; x < g_cols; x++) 
		{
	            int blue  = (int)g_image[g_channels*x + g_step*y];
	            int green = (int)g_image[g_channels*x + g_step*y + 1];
	            int red   = (int)g_image[g_channels*x + g_step*y + 2];
            
	            grayImage[x + g_cols*y] = (uchar)(.3*red) + (.59 * green) + (.11 * blue);

	        }
	    }
	    ret.write (grayImage);

	}

	SC_CTOR (GRAY)
	{
		SC_METHOD (grayscale);
		sensitive << image << rows << cols << channels << step << ret;
	}
};

