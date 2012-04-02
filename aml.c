/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * aml.c: Anisotropic Morphological Leveling                              *
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

#include "aml.h"

void cvAml(const IplImage *src, IplImage *dst, int scale_ML, double K, double dt, double s)
{
    //Check image
    int i;

    if(src->nChannels != 1)
    {
        printf("Image must have only one band for AML\n");
        exit(-1);
    }

    cvZero(dst);
    //Initialize marker
    IplImage* J = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* aml = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    cvCopy(src, aml, NULL);
    cvCopy(src, J, NULL);

    //Create Anisotropic Markers - Main Loop
    for(i=0;i<scale_ML;i++)
    {
        //Compute ALM
        cvAlm(J, tmp, K, 1, dt, s);
        cvCopy(tmp, J, NULL);

        //Compute AML
        ExpLev2D(aml, tmp, J, 100, 0.1);
        cvCopy(tmp, aml, NULL);
    }
    cvCopy(aml, dst, NULL);
    //Clear things up
    cvReleaseImage(&J);
    cvReleaseImage(&aml);
    cvReleaseImage(&tmp);

    return;
}

void CreateLev2D(const IplImage *src, IplImage *dst, IplImage **Marker, int n, double MSEth, int Fin)
{
    int i;
    //Initialize Leveling cube
    IplImage** Lev = (IplImage**) malloc(Fin * sizeof(IplImage*));
    for(i=0;i<Fin;i++)
    {
        Lev[i] = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
        cvZero(Lev[i]);
    }

    //Main Loop
    for(i=0;i<Fin;i++)
    {
        if(i==0)
        {
            ExpLev2D(src, Lev[i], Marker[i], n, MSEth);
        }
        else
        {
            ExpLev2D(Lev[i-1], Lev[i], Marker[i], n, MSEth);
        }
    }

    cvCopy(Lev[Fin-1], dst, NULL);

    for(i=0;i<Fin;i++)
    {
        cvReleaseImage(&Lev[i]);
    }
    free(Lev);
}

void ExpLev2D(const IplImage *src, IplImage *dst, const IplImage *mark, int n, double MSEth)
{
    int i;
    double MSdiff;
    double MSEth2 = MSEth * MSEth;
    CvScalar mean;
    IplImage* tpn = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* tpn2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* dst2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* dif2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* dil = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* ero = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* mini = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);

    IplConvKernel* element = cvCreateStructuringElementEx( 3, 3, 2, 2, CV_SHAPE_CROSS, NULL );

    cvCopy(mark, dst, NULL);

    for(i=0;i<n;i++)
    {
        cvCopy(dst, tpn, NULL);
        //Leveling transform
        cvErode(dst,ero,element,1);
        cvDilate(dst,dil,element,1);
        cvMin(src, dil, mini);
        cvMax(mini, ero, dst);

        //Statistics
        cvPow(tpn, tpn2, 2.0);
        cvPow(dst, dst2, 2.0);
        cvSub(tpn2, dst2, mini, NULL);
        mean = cvAvg(mini, NULL);
        MSdiff = fabs(mean.val[0]);
        //MSdiff = mean.val[0] * mean.val[0];
        if(MSdiff <= MSEth2) break;
    }

    //House Cleaning
    cvReleaseStructuringElement(&element);
    cvReleaseImage(&tpn);
    cvReleaseImage(&tpn2);
    cvReleaseImage(&dst2);
    cvReleaseImage(&dif2);
    cvReleaseImage(&dil);
    cvReleaseImage(&ero);
    cvReleaseImage(&mini);
    return;
}

