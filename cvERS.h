/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * cvers.h: ERS (BIL) image library for OpenCV                            *
 * Version: 1.0.0 Final                                                   *
 * Last revised: 01/04/2009                                               *
 *                                                                        *
 *									  *
 * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * Implementation by Angelos Tzotsos (tzotsos@gmail.com)	  	  *
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

#ifndef CVERS_H
#define CVERS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include "cv.h"
#include "highgui.h"

using namespace std;

int D1(int imglines, int imgcolumns, int line, int column);

struct pixel {
    int line;
    int column;
};

struct point {
    double x;
    double y;
    double z;
};

pixel D2(int d, int imgcolumns);

struct Coordinate {
        string Datum;
        string Projection;
        string CoordinateType;
        string Units;
        string Rotation;
};

class cvHyperImage;

class ERSImage
{
    public:

    int Bands;
    int Lines;
    int Columns;
    int Bytes;

    double MinX;
    double MinY;
    double MaxX;
    double MaxY;

    point RegistrationPoint;
    double GroundSizeX;
    double GroundSizeY;

    fstream file;
    //fstream file; or ofstream file;
    void* data;

    int NullValue;
    //ERS Header strings
    string HeaderName;
    string FileName;
    string CellType;
    string ERSVersion;
    string DataType;
    string ByteOrder;
    string LastUpdated;


    //ERS Band names
    vector<string> BandID;

    //ERS Geocoding Information
    Coordinate CoordinateSpace;

    //Methods
    void SaveAs(string);
    int D1(int line, int column, int band);
    point Pixel2Point(int line, int column);
    //Constructors
    ERSImage(const cvHyperImage& img);
    ERSImage(string);
    ERSImage(const ERSImage& img, int selectedBand);
    ~ERSImage();
};

class cvHyperImage
{
    public:

    int Bands;
    int Lines;
    int Columns;
    int Bytes;

    IplImage** cube;

    point RegistrationPoint;
    double GroundSizeX;
    double GroundSizeY;
    int NullValue;

    string CellType;
    string ERSVersion;
    string DataType;
    string ByteOrder;
    string LastUpdated;

    vector<string> BandID;
    Coordinate CoordinateSpace;

    //Constructors
    cvHyperImage(int lines, int columns, int bands, int bytes);
    cvHyperImage(const ERSImage& img);
    ~cvHyperImage();

    //Methods

};


#endif /* CVERS_H */

