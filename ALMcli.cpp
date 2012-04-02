/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * almcli.cpp: image selective smoothing command line interface           *
 * Version: 1.0.0 Final                                                   *
 * Last revised: 01/04/2009                                               *
 *                                                                        *
 * Alvarez L., Lions P. L., and Morel J. M., 1992.                        *
 * Image selective smoothing and edge detection by nonlinear diffusion.   *
 * II,SIAM-JNA. 9 -'03                                                    *
 * K - a constant for the monotonicly increasing function                 *
 * iters - iterations                                                     *
 * dt - time step                                                         *
 * s - standard deviation of the Gaussian                                 *
 *									  *
 * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * ALM algorithm implementation by Angelos Tzotsos (tzotsos@gmail.com)	  *
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
#include <stdlib.h>
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
        << "Usage : ALMcmd <input.tif> <output.tif> <scale> [s]"
        << endl;
        exit(1);
    }
    //double min_px;
    //double max_px;

    string c1 = "Initial Image";
    string c11 = "Alm Image";

    IplImage* img = cvLoadImage(argv[1], -1);
    int ite = atoi(argv[3]);
    double s = 1.0;
    if(argc == 5) s = atof(argv[4]);

    //cvMinMaxLoc(img, &min_px, &max_px, NULL, NULL, NULL);

    IplImage* scl = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

    cvConvert(img, scl);
    //cvNormalize(img, scl, 1.0, 0.0, CV_MINMAX, NULL);

    cvNamedWindow(c1.c_str(), CV_WINDOW_AUTOSIZE);
    cvNamedWindow(c11.c_str(), CV_WINDOW_AUTOSIZE);

    IplImage* Ux = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

    cvAlm(scl, Ux, 1, ite, 0.1, s);

    IplImage* out = cvCreateImage(cvGetSize(img), img->depth, 1);
    cvConvert(Ux, out);

    cvShowImage(c1.c_str(), img);
    cvShowImage(c11.c_str(), out);

    cvWaitKey(0);
    cvSaveImage(argv[2], out);
    cvReleaseImage(&img);
    cvReleaseImage(&out);
    cvReleaseImage(&scl);
    cvReleaseImage(&Ux);
    cvDestroyWindow(c1.c_str());
    cvDestroyWindow(c11.c_str());

    return 0;
}



