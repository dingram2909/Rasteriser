#pragma once

#include "SoftwareRasteriser.h"
#include "Colour.h"

#include <string>
/******************************************************************************
Class:Texture
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Simple class to hold texture data for our software rasteriser.

Later on in the tutorial series, the ability to performa mipmapping and 
bilinear filtering is added to this class.

The provided TextureFromTGA function is very basic, and will only support
uncompressed targa files - you can save images in this format using paint.net, 
which is free

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::vector;

class Texture	{
public:
	friend class SoftwareRasteriser;
	Texture(void);
	~Texture(void);

	static Texture* TextureFromTGA(const string &filename);
	
	const Colour&	NearestTexSample(const Vector3 &coords, int miplevel = 0);

	const Colour&	ColourAtPoint(int x, int y, int mipLevel = 0) {
		int texWidth  = width  >> mipLevel;
		int texHeight = height >> mipLevel;

		x = max(0,min(x,(int)texWidth-1));
		y = max(0,min(y,(int)texHeight-1));

		int index =  (y * texHeight) + x;

		return texels[index];
	}

	uint	GetWidth()	{ return width;}
	uint	GetHeight() { return height;}

protected:
	uint width;
	uint height;

	Colour* texels;
};

