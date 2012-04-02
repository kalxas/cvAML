/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * alm.h: image selective smoothing	                                  *
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

#ifndef ALM_H
#define ALM_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cv.h"
#include "highgui.h"


//src IPL_DEPTH_32F, dst IPL_DEPTH_32F
void cvAlm(const IplImage *src, IplImage *dst, double K, int iters, double dt, double s);


#endif
