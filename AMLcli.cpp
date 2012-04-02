/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * amlcli.cpp: Anisotropic Morphological Leveling CLI                     *
 * Version: 1.0.0 Final                                                   *
 * Last revised: 01/04/2009                                               *
 *                                                                        *
 * Karantzalos K. and Argialas D., 2006. Improving Edge Detection 	  *
 * and Watershed Segmentation with Anisotropic Diffusion and 		  *
 * Morphological Levelings, International Journal of Remote Sensing, 	  *
 * Volume 27, Issue 24, pages 5427–5434.				  *
 *                                                     			  *
 * K - a constant for the monotonicly increasing function                 *
 * Scale - iterations                                                     *
 * dt - time step                                                         *
 * s - standard deviation of the Gaussian                                 *
 *									  *
 * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * AML C implementation by Angelos Tzotsos (tzotsos@gmail.com)	  	  *
 * Remote Sensing Lab NTUA - GCpp                         April 2009      *
 *									  *
 *   Copyright (C) April 2009 Angelos Tzotsos <tzotsos@gmail.com>	  *
 *									  *
 * This program is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published by   *
 * the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.					  *
 *									  *
 * This program is distributed in the hope that it will be useful,	  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 	  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	  *
 * GNU General Public License for more details.				  *
 *									  *
 * You should have received a copy of the GNU General Public License	  *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *									  *
 * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <string>
#include "cv.h"
#include "highgui.h"
extern "C" {
#include "aml.h"
}

using namespace std;

int main(int argc, char *argv[])
{

    if(argc != 4 && argc != 5)   //TODO: Add to Error Handling
    {
        cout << "Error in arguments:" << endl << endl
        << "Usage : AMLcmd <input.tif> <output.tif> <scale> [s]"
        << endl;
        exit(1);
    }

    //read arguments
    int scales = atoi(argv[3]);
    double s = 1.0;
    if(argc == 5) s = atof(argv[4]);

    string c1 = "Initial Image";
    string c2 = "AML Image";

    IplImage* img = cvLoadImage(argv[1], -1);
    IplImage* out = cvCreateImage(cvGetSize(img), img->depth, 1);

    IplImage* dbl1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
    IplImage* dbl2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
    cvConvert(img, dbl1);


    cvAml(dbl1, dbl2, scales, 1.0, 0.1, s);

    cvConvert(dbl2, out);
    cvNamedWindow(c1.c_str(), CV_WINDOW_AUTOSIZE);
    cvNamedWindow(c2.c_str(), CV_WINDOW_AUTOSIZE);

    cvShowImage(c1.c_str(), img);
    cvShowImage(c2.c_str(), out);


    cvWaitKey(0);
    cvSaveImage(argv[2], out);
    cvReleaseImage(&img);
    cvReleaseImage(&out);
    cvReleaseImage(&dbl1);
    cvReleaseImage(&dbl2);
    cvDestroyWindow(c1.c_str());
    cvDestroyWindow(c2.c_str());

    return 0;
}


