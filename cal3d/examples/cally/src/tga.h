//----------------------------------------------------------------------------//
// viewer.h                                                                   //
// Copyright (C) 2003 Desmecht Laurent                                        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef _TGA_H
#define _TGA_H

#include <stdio.h>
#include <stdlib.h>

/*!
	Read a TGA bitmap
	Le TGA file must been :
	- Greyscale 8 bit per pixel uncompressed
	- Color 24 bit per pixel uncompressed
	- Color 32 bit per pixel uncompressed
	- Color 24 bit per pixel compressed (RLE)
	- Color 32 bit per pixel compressed (RLE)

	\param str : File name
	\param dest : Pointer on the destination 
	Pointeur vers lequel l'image est située après la fonction
	\param SizeX : width of the bitmap
	\param SizeY : height of the bitmap
	\param Bpp : Bit per pixel of the bitmap (8,24 or 32)
*/

class  CTgaHeader
{
public:
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short colourmaporigin;
    short colourmaplength;
    char  colourmapdepth;
    short x_origin;
    short y_origin;
    short SizeX;
    short SizeY;
    char  Bpp;
    char  imagedescriptor;
};

class CTga
{
private:
	CTgaHeader TgaHeader;
	int m_SizeX,m_SizeY,m_Bpp;
	unsigned char *m_dest;
public:

	CTga();	

	int GetSizeX()
	{
		return m_SizeX;
	}
	
	int GetSizeY()
	{
		return m_SizeY;
	}
	
	int Bpp()
	{
		return m_Bpp;		
	}
	
	unsigned char * GetPointer()
	{
		return m_dest;
	}	
	
	int ReadFile(const char *str);
	int Release();
};

#endif
