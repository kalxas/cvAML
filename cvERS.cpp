/* * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *
 * cvers.cpp: ERS (BIL) image library for OpenCV                          *
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

#include "cvERS.h"

pixel D2(int d, int imgcolumns)
{
    pixel x;
    int k = d % imgcolumns;
    x.column = (k) ? k : imgcolumns;
    x.line = (k) ? ((int)(d/imgcolumns) + 1) : ((int)(d/imgcolumns));
    return x;
}

int D1(int imglines, int imgcolumns, int line, int column)
{
    return (((line-1)*imgcolumns) + column -1);
}

ERSImage::ERSImage(string InputHeaderName)
{
	ifstream header;
	header.open(InputHeaderName.c_str());
	int NumOfBytes = 0;

 	if (!header){								//Error handling
	    exit(1);
	}

  	HeaderName = InputHeaderName;

 	FileName = HeaderName.substr(0, HeaderName.size()-4);	/*Removes ".ers" */
 	string word;				//temporary string for input

    GroundSizeX = 1.0;
    GroundSizeY = 1.0;
    RegistrationPoint.x = 0.0;
    RegistrationPoint.y = 0.0;

 	//Header file parser

 	while (header){
 	    header >> word;

 	    if (strncmp(word.c_str(), "Version", 6) == 0){
 	        header >> word;		/*Skips the "="*/
 	        header >> word;
 	        ERSVersion = word.substr(1,word.size()-2);		/*Removes the "___"*/
 	   }

       if (strncmp(word.c_str(), "LastUpdated", 11) == 0){
           header >> word;		/*Skips the "="*/
           for (int i=0; i<5; i++){
               header >> word;
               LastUpdated = LastUpdated + word + " ";/*Reads 5 words*/
           }
           header >> word;
           LastUpdated = LastUpdated + word; /*Does not add the space after the sixth word*/
       }

       if (strncmp(word.c_str(), "DataType", 8) == 0){
           header >> word;		/*Skips the "="*/
           header >> DataType;
       }

       if (strncmp(word.c_str(), "ByteOrder", 9) == 0){
           header >> word;		/*Skips the "="*/
           header >> ByteOrder;
       }

       if (strncmp(word.c_str(), "Datum", 5) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           CoordinateSpace.Datum = word.substr(1,word.size()-2);		/*Removes the "___"*/
       }

       if (strncmp(word.c_str(), "Projection", 10) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           CoordinateSpace.Projection = word.substr(1,word.size()-2);	/*Removes the "___"*/
       }

       if (strncmp(word.c_str(), "CoordinateType", 14) == 0){
           header >> word;		/*Skips the "="*/
           header >> CoordinateSpace.CoordinateType;
       }

       if (strncmp(word.c_str(), "Units", 5) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           CoordinateSpace.Units = word.substr(1,word.size()-2);	/*Removes the "___"*/
       }

       if (strncmp(word.c_str(), "Xdimension", 10) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           GroundSizeX = atof(word.c_str());
       }

       if (strncmp(word.c_str(), "Ydimension", 10) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           GroundSizeY = atof(word.c_str());
       }

       if (strncmp(word.c_str(), "Eastings", 8) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           RegistrationPoint.x = atof(word.c_str());
       }

       if (strncmp(word.c_str(), "Northings", 9) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           RegistrationPoint.y = atof(word.c_str());
       }

       if (strncmp(word.c_str(), "Rotation", 8) == 0){
           header >> word;		/*Skips the "="*/
           header >> CoordinateSpace.Rotation;
       }

       if (strncmp(word.c_str(), "CellType", 8) == 0){
           header >> word;		/*Skips the "="*/
           header >> CellType;
           if (strncmp(CellType.c_str(), "Unsigned16BitInteger", 20) == 0)
           {
               Bytes = 2;
           }
           else if (strncmp(CellType.c_str(), "Unsigned8BitInteger", 19) == 0)
           {
               Bytes = 1;
           }
           else if (strncmp(CellType.c_str(), "IEEE4ByteReal", 13) == 0)
           {
               Bytes = 4;
           }
           else  Bytes = 1;
       }

       if (strncmp(word.c_str(), "NullCellValue", 13) == 0){
           header >> word;		/*Skips the "="*/
           header >> NullValue;
       }

       if (strncmp(word.c_str(), "NrOfLines", 9) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           Lines = atoi(word.c_str());
           if (Lines < 1){
               exit(1);
           }
       }

       if (strncmp(word.c_str(), "NrOfCellsPerLine", 16) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           Columns = atoi(word.c_str());
           if (Columns < 1){
               exit(1);
           }
       }

       if (strncmp(word.c_str(), "NrOfBands", 9) == 0){
           header >> word;		/*Skips the "="*/
           header >> word;
           Bands = atoi(word.c_str());
           if (Bands < 1){
               exit(1);
           }
           //vector<string> BandID;
           for (int i=0; i<5; i++){
               header >> word;
           }
           BandID.push_back(word);
           for (int k=0;k < Bands-1; k++){
               for (int j=0; j<7; j++){
               		header >> word;
     		   }
          	   BandID.push_back(word);
           }
       }
    }//Header file parser END
    header.close();
    MinX = RegistrationPoint.x + (GroundSizeX / 2);
    MaxX = MinX + ((Columns - 1) * GroundSizeX);
    MaxY = RegistrationPoint.y - (GroundSizeY / 2);
    MinY = MaxY - ((Lines - 1) * GroundSizeY);
    //Opening binary data file for input
    file.open(FileName.c_str(), ios::in | ios::out | ios::binary);
    if (!file){								//Error handling
	    exit(1);
    }

    file.seekg(0, ios::beg);
    NumOfBytes = Bands * Columns * Lines * Bytes;
    data = malloc(NumOfBytes * sizeof(uchar));
    file.read(reinterpret_cast<char *> (data), NumOfBytes);
}

ERSImage::ERSImage(const cvHyperImage& img)
{
    int i, l, c, b, step;
    uchar* buf8;
    uchar* bil8;
    ushort* buf16;
    ushort* bil16;
    float* buf32;
    float* bil32;

    string tmp;
    Bands = img.Bands;
    Lines = img.Lines;
    Columns = img.Columns;
    Bytes = img.Bytes;
    RegistrationPoint.x = img.RegistrationPoint.x;
    RegistrationPoint.y = img.RegistrationPoint.y;
    RegistrationPoint.z = img.RegistrationPoint.z;
    GroundSizeX = img.GroundSizeX;
    GroundSizeY = img.GroundSizeY;
    NullValue = img.NullValue;
    CellType = img.CellType;
    ERSVersion = img.ERSVersion;
    DataType = img.DataType;
    ByteOrder = img.ByteOrder;
    LastUpdated = img.LastUpdated;

    for(i=0;i<img.BandID.size();i++)
    {
        tmp = img.BandID[i];
        BandID.push_back(tmp);
    }
    CoordinateSpace.CoordinateType = img.CoordinateSpace.CoordinateType;
    CoordinateSpace.Datum = img.CoordinateSpace.Datum;
    CoordinateSpace.Projection = img.CoordinateSpace.Projection;
    CoordinateSpace.Rotation = img.CoordinateSpace.Rotation;
    CoordinateSpace.Units = img.CoordinateSpace.Units;

    int NumOfBytes = Bands * Columns * Lines * Bytes;
    data = malloc(NumOfBytes * sizeof(uchar));

    if(Bytes == 1)
    {
        step = (img.cube[0]->widthStep)/(sizeof(uchar));
        bil8 = (uchar *)data;
        for(b=0;b<Bands;b++)
        {
            buf8 = (uchar *)(img.cube[b]->imageData);
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    bil8[((l*Bands*Columns) + (b*Columns) + c)] = buf8[l*step+c];
                }
            }
        }
    }
    else if(Bytes == 2)
    {
        step = (img.cube[0]->widthStep)/(sizeof(ushort));
        bil16 = (ushort *)data;
        for(b=0;b<Bands;b++)
        {
            buf16 = (ushort *)(img.cube[b]->imageData);
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    bil16[((l*Bands*Columns) + (b*Columns) + c)] = buf16[l*step+c];
                }
            }
        }
    }
    else if(Bytes == 4)
    {
        step = (img.cube[0]->widthStep)/(sizeof(float));
        bil32 = (float *)data;
        for(b=0;b<Bands;b++)
        {
            buf32 = (float *)(img.cube[b]->imageData);
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    bil32[((l*Bands*Columns) + (b*Columns) + c)] = buf32[l*step+c];
                }
            }
        }
    }

}

ERSImage::ERSImage(const ERSImage& img, int selectedBand)
{
    int l, c, b;
    uchar* buf8;
    uchar* bil8;
    ushort* buf16;
    ushort* bil16;
    float* buf32;
    float* bil32;

    string tmp;
    Bands = 1;
    Lines = img.Lines;
    Columns = img.Columns;
    Bytes = img.Bytes;
    RegistrationPoint.x = img.RegistrationPoint.x;
    RegistrationPoint.y = img.RegistrationPoint.y;
    RegistrationPoint.z = img.RegistrationPoint.z;
    GroundSizeX = img.GroundSizeX;
    GroundSizeY = img.GroundSizeY;
    NullValue = img.NullValue;
    CellType = img.CellType;
    ERSVersion = img.ERSVersion;
    DataType = img.DataType;
    ByteOrder = img.ByteOrder;
    LastUpdated = img.LastUpdated;

    tmp = img.BandID[selectedBand - 1];
    BandID.push_back(tmp);

    CoordinateSpace.CoordinateType = img.CoordinateSpace.CoordinateType;
    CoordinateSpace.Datum = img.CoordinateSpace.Datum;
    CoordinateSpace.Projection = img.CoordinateSpace.Projection;
    CoordinateSpace.Rotation = img.CoordinateSpace.Rotation;
    CoordinateSpace.Units = img.CoordinateSpace.Units;

    int NumOfBytes = Bands * Columns * Lines * Bytes;
    data = malloc(NumOfBytes * sizeof(uchar));

    if(Bytes == 1)
    {
        bil8 = (uchar *)data;
        buf8 = (uchar *)(img.data);
        b = selectedBand - 1;

        for(l=0;l<Lines;l++)
        {
            for(c=0;c<Columns;c++)
            {
                bil8[((l*Columns) + c)] = buf8[((l*img.Bands*img.Columns) + (b*img.Columns) + c)];
            }
        }
    }

    if(Bytes == 2)
    {
        bil16 = (ushort *)data;
        buf16 = (ushort *)(img.data);
        b = selectedBand - 1;

        for(l=0;l<Lines;l++)
        {
            for(c=0;c<Columns;c++)
            {
                bil16[((l*Columns) + c)] = buf16[((l*img.Bands*img.Columns) + (b*img.Columns) + c)];
            }
        }
    }

    if(Bytes == 4)
    {
        bil32 = (float *)data;
        buf32 = (float *)(img.data);
        b = selectedBand - 1;

        for(l=0;l<Lines;l++)
        {
            for(c=0;c<Columns;c++)
            {
                bil32[((l*Columns) + c)] = buf32[((l*img.Bands*img.Columns) + (b*img.Columns) + c)];
            }
        }
    }

}

ERSImage::~ERSImage()
{
    file.close();
    free(data);
}

void ERSImage::SaveAs(string OutputHeaderName)
{
    ofstream head_out, file_out;
    string tmp = OutputHeaderName.substr(0, OutputHeaderName.size()-4);

    head_out.open(OutputHeaderName.c_str());
    file_out.open(tmp.c_str(), ios::binary);

	//save binary data
    int numOfBytes = Bands * Columns * Lines * Bytes;
    file_out.write(reinterpret_cast<char *> (data), numOfBytes);

    //Coordinate CoordinateSpace;
    //save header
    head_out << "DatasetHeader Begin" << endl;
    head_out << "\tVersion \t= \"" << ERSVersion << "\"" << endl;
    //head_out << "\tName\t\t= \"" << HeaderName << "\"" << endl;
    head_out << "\tLastUpdated\t= " << LastUpdated << endl;
    head_out << "\tDataSetType\t= ERStorage" << endl;
    head_out << "\tDataType\t= " << DataType << endl;
    head_out << "\tByteOrder\t= " << ByteOrder << endl;
    head_out << "\tCoordinateSpace Begin" << endl;
    head_out << "\t\tDatum\t\t= \"" << CoordinateSpace.Datum << "\"" << endl;
    head_out << "\t\tProjection\t= \"" << CoordinateSpace.Projection << "\"" << endl;
    head_out << "\t\tCoordinateType\t= " << CoordinateSpace.CoordinateType << endl;
    head_out << "\t\tUnits\t\t= \"" << CoordinateSpace.Units << "\"" << endl;
    head_out << "\t\tRotation\t= " << CoordinateSpace.Rotation << endl;
    head_out << "\tCoordinateSpace End" << endl;
    head_out << "\tRasterInfo Begin" << endl;
    head_out << "\t\tCellType\t= " << CellType << endl;
    head_out << "\t\tNrOfLines\t= " << Lines << endl;
    head_out << "\t\tNrOfCellsPerLine\t= " << Columns << endl;
    head_out << "\t\tNrOfBands\t= " << Bands << endl;
    for(int i=0;i<BandID.size();i++){
        head_out << "\t\tBandId Begin" << endl;
        head_out << "\t\t\tValue\t\t= " << BandID[i] << endl;
        head_out << "\t\tBandId End" << endl;
    }
    head_out << "\t\tRegionInfo Begin" << endl;
    head_out << "\t\t\tType\t\t= Polygon" << endl;
    head_out << "\t\t\tRegionName\t= \"All\"" << endl;
    //head_out << "\t\t\tSourceDataset\t= \"" << FileName << "\"" << endl;
    head_out << "\t\t\tRGBcolour Begin" << endl;
    if(Bytes == 1)
    {
        head_out << "\t\t\t\tRed\t\t= 255" << endl;
        head_out << "\t\t\t\tGreen\t\t= 255" << endl;
        head_out << "\t\t\t\tBlue\t\t= 255" << endl;
    }
    else
    {
        head_out << "\t\t\t\tRed\t\t= 65535" << endl;
        head_out << "\t\t\t\tGreen\t\t= 65535" << endl;
        head_out << "\t\t\t\tBlue\t\t= 65535" << endl;
    }
    head_out << "\t\t\tRGBcolour End" << endl;
    head_out << "\t\t\tSubRegion\t= {" << endl;
    head_out << "\t\t\t\t0\t0" << endl;
    head_out << "\t\t\t\t0\t" << Lines << endl;
    head_out << "\t\t\t\t" << Columns << "\t" << Lines << endl;
    head_out << "\t\t\t\t" << Columns << "\t0" << endl;
    head_out << "\t\t\t}" << endl;
    head_out << "\t\tRegionInfo End" << endl;
    head_out << "\tRasterInfo End" << endl;
    head_out << "DatasetHeader End" << endl;

    //Close files
    file_out.close();
    head_out.close();
}

point ERSImage::Pixel2Point(int line, int column)
{
    point temp;
    temp.x = MinX + ((column - 1) * GroundSizeX);
    temp.y = MaxY - ((line - 1) * GroundSizeY);
    return temp;
}

int ERSImage::D1(int line, int column, int band)
{
    return (((line-1)*Bands*Columns) + ((band-1)*Columns) + column -1);
}

cvHyperImage::cvHyperImage(int lines, int columns, int bands, int bytes)
{
    int i;
    string tmp;
    Lines = lines;
    Columns = columns;
    Bands = bands;
    Bytes = bytes;
    ERSVersion = "6.3";
    LastUpdated = "Fri Apr 02 11:52:07 GMT 2004";
    DataType = "Raster";
    ByteOrder = "LSBFirst";
    CoordinateSpace.Datum = "RAW";
    CoordinateSpace.Projection = "RAW";
    CoordinateSpace.CoordinateType = "RAW";
    CoordinateSpace.Units = "natural";
    CoordinateSpace.Rotation = "0:0:0.0";
    if(Bytes == 1) CellType = "Unsigned8BitInteger";
    else if(Bytes == 2) CellType = "Unsigned16BitInteger";
    else if(Bytes == 4) CellType = "IEEE4ByteReal";
    for(i=0;i<Bands;i++)
    {
        stringstream ss;
        ss << (i+1);
        tmp = "Band " + ss.str();
        BandID.push_back(tmp);
    }

    cube = (IplImage**) malloc(Bands * sizeof(IplImage*));
    for(i=0;i<Bands;i++)
    {
        if(Bytes == 1) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_8U, 1);
        else if (Bytes == 2) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_16U, 1);
        else if (Bytes == 4) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_32F, 1);
    }

}

cvHyperImage::cvHyperImage(const ERSImage& img)
{
    int i, l, c, b, step;
    uchar* buf8;
    uchar* bil8;
    ushort* buf16;
    ushort* bil16;
    float* buf32;
    float* bil32;

    string tmp;
    Bands = img.Bands;
    Lines = img.Lines;
    Columns = img.Columns;
    Bytes = img.Bytes;
    RegistrationPoint.x = img.RegistrationPoint.x;
    RegistrationPoint.y = img.RegistrationPoint.y;
    RegistrationPoint.z = img.RegistrationPoint.z;
    GroundSizeX = img.GroundSizeX;
    GroundSizeY = img.GroundSizeY;
    NullValue = img.NullValue;
    CellType = img.CellType;
    ERSVersion = img.ERSVersion;
    DataType = img.DataType;
    ByteOrder = img.ByteOrder;
    LastUpdated = img.LastUpdated;

    for(i=0;i<img.BandID.size();i++)
    {
        tmp = img.BandID[i];
        BandID.push_back(tmp);
    }
    CoordinateSpace.CoordinateType = img.CoordinateSpace.CoordinateType;
    CoordinateSpace.Datum = img.CoordinateSpace.Datum;
    CoordinateSpace.Projection = img.CoordinateSpace.Projection;
    CoordinateSpace.Rotation = img.CoordinateSpace.Rotation;
    CoordinateSpace.Units = img.CoordinateSpace.Units;

    cube = (IplImage**) malloc(img.Bands * sizeof(IplImage*));
    for(i=0;i<img.Bands;i++)
    {
        if(img.Bytes == 1) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_8U, 1);
        else if (img.Bytes == 2) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_16U, 1);
        else if (img.Bytes == 4) cube[i] = cvCreateImage(cvSize(Columns, Lines), IPL_DEPTH_32F, 1);
    }

    if(Bytes == 1)
    {
        step = (cube[0]->widthStep)/(sizeof(uchar));
        bil8 = (uchar *)img.data;
        for(b=0;b<Bands;b++)
        {
            buf8 = (uchar *)(cube[b]->imageData);
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    buf8[l*step+c] = bil8[((l*Bands*Columns) + (b*Columns) + c)];
                }
            }
        }
    }
    else if(Bytes == 2)
    {
        step = (cube[0]->widthStep)/(sizeof(ushort));
        bil16 = (ushort *)img.data;
        for(b=0;b<Bands;b++)
        {
            buf16 = (ushort *)cube[b]->imageData;
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    buf16[l*step+c] = bil16[((l*Bands*Columns) + (b*Columns) + c)];
                }
            }
        }
    }
    else if(Bytes == 4)
    {
        step = (cube[0]->widthStep)/(sizeof(float));
        bil32 = (float *)img.data;
        for(b=0;b<Bands;b++)
        {
            buf32 = (float *)cube[b]->imageData;
            for(l=0;l<Lines;l++)
            {
                for(c=0;c<Columns;c++)
                {
                    buf32[l*step+c] = bil32[((l*Bands*Columns) + (b*Columns) + c)];
                }
            }
        }
    }

}

cvHyperImage::~cvHyperImage()
{
    int i;
    for (i=0;i<Bands;i++)
    {
        cvReleaseImage(&cube[i]);
    }
    free(cube);
}




