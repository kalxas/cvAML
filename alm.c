/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * alm.c: image selective smoothing	                                  *
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

#include "alm.h"

void cvAlm(const IplImage *src, IplImage *dst, double K, int iters, double dt, double s)
{
    int i;
    double epsilon = 0.0001;
    double kT = -1.0 / K;
    CvPoint anchor=cvPoint(-1,-1);

    CvScalar eps = cvScalar(epsilon, 0, 0, 0);

    double kux[] = { 0.0, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0 };
    double kuy[] = { 0.0, 0.0, 0.0, -0.5, 0.0, 0.5, 0.0, 0.0, 0.0 };
    double kuxx[] = { 0.0, 1.0, 0.0, 0.0, -2.0, 0.0, 0.0, 1.0, 0.0 };
    double kuyy[] = { 0.0, 0.0, 0.0, 1.0, -2.0, 1.0, 0.0, 0.0, 0.0 };
    double kuxy[] = { 0.25, 0.0, -0.25, -0.25, 0.0, 0.0, 0.0, 0.0, 0.25 };

    CvMat Mux, Muy, Muxx, Muyy, Muxy;

    cvInitMatHeader(&Mux, 3, 3, CV_64FC1, kux, CV_AUTOSTEP);
    cvInitMatHeader(&Muy, 3, 3, CV_64FC1, kuy, CV_AUTOSTEP);
    cvInitMatHeader(&Muxx, 3, 3, CV_64FC1, kuxx, CV_AUTOSTEP);
    cvInitMatHeader(&Muyy, 3, 3, CV_64FC1, kuyy, CV_AUTOSTEP);
    cvInitMatHeader(&Muxy, 3, 3, CV_64FC1, kuxy, CV_AUTOSTEP);


    IplImage* Ux2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uy2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uxs2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uys2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Ux = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uy = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uxx = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uyy = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* Uxy = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    IplImage* tmp1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
    cvCopy(src, dst, NULL);

    for(i=0;i<iters;i++)
    {
        //dst == J
        cvSmooth(dst, tmp1, CV_GAUSSIAN, 5, 5, s, 0.0);//Gaussian Scale
        cvFilter2D(tmp1, Ux2, &Mux, anchor);//Uxs actually
        cvFilter2D(tmp1, Uy2, &Muy, anchor);//Uys actually
        cvPow(Ux2, Uxs2, 2.0);
        cvPow(Uy2, Uys2, 2.0);
        cvFilter2D(dst, Ux, &Mux, anchor);
        cvFilter2D(dst, Uy, &Muy, anchor);
        cvFilter2D(dst, Uxx, &Muxx, anchor);
        cvFilter2D(dst, Uyy, &Muyy, anchor);
        cvFilter2D(dst, Uxy, &Muxy, anchor);
        cvPow(Ux, Ux2, 2.0);
        cvPow(Uy, Uy2, 2.0);
        //Compute equation 5
        //flux = exp(-(Uxs2 + Uys2)/K);

        cvAdd(Uxs2, Uys2, tmp1, NULL);
        cvScale(tmp1, Uxs2, kT, 0);
        cvExp(Uxs2, tmp1);//tmp1=flux

        //Compute equation 6
        //eq6= (Uy2.*Uxx - 2*Ux.*Uy.*Uxy + Ux2.*Uyy) ./ (Ux2+Uy2 + epsilon*epsilon);
        cvMul(Uy2,Uxx,Uxs2,1);
        cvMul(Ux2,Uyy,Uys2,1);
        cvMul(Ux, Uy, Uxx, 2.0);
        cvMul(Uxx,Uxy,Uyy,1);
        cvSub(Uxs2, Uyy, Uxx, NULL);
        cvAdd(Uxx, Uys2, Uxs2, NULL);
        cvAdd(Ux2, Uy2, Uxx, NULL);
        cvAddS(Uxx, eps, Uys2, NULL);
        cvDiv(Uxs2, Uys2, Uxy, 1);

        //Compute final equation
        //J = J + dt*flux.*eq6;
        cvMul(tmp1, Uxy, Uxs2, dt);
        cvAdd(dst, Uxs2, tmp1, NULL);
        cvCopy(tmp1, dst, NULL);

    }

    cvReleaseImage(&Ux2);
    cvReleaseImage(&Uy2);
    cvReleaseImage(&Uxs2);
    cvReleaseImage(&Uys2);
    cvReleaseImage(&Ux);
    cvReleaseImage(&Uy);
    cvReleaseImage(&Uxx);
    cvReleaseImage(&Uyy);
    cvReleaseImage(&Uxy);
    cvReleaseImage(&tmp1);

}



