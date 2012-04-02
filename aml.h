/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * aml.h: Anisotropic Morphological Leveling                              *
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

#ifndef AML_H
#define AML_H

#include "alm.h"

void ExpLev2D(const IplImage *src, IplImage *dst, const IplImage *mark, int n, double MSEth);

void CreateLev2D(const IplImage *src, IplImage *dst, IplImage **Marker, int n, double MSEth, int Fin);

void cvAml(const IplImage *src, IplImage *dst, int scale_ML, double K, double dt, double s);

#endif
