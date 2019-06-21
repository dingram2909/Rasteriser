/******************************************************************************
Class:RenderObject
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to represent an object in our basic rendering program

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Matrix4.h"

class Texture;

class RenderObject	{
public:
	RenderObject(void);
	~RenderObject(void);

	Mesh*	 GetMesh()	 { return mesh;}
	Texture* GetTexure() { return texture;}

	Matrix4 GetModelMatrix() {
		return modelMatrix;
	}

//protected:
	Matrix4 modelMatrix;

	Texture*	texture;
	Mesh*		mesh;
};

