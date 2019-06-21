/******************************************************************************
Class:SoftwareRasteriser
Implements:Window
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to encapsulate the various rasterisation techniques looked
at in the course material.

This is the class you'll be modifying the most!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderObject.h"
#include "Common.h"
#include "Window.h"

#include <vector>

using std::vector;

struct BoundingBox {
	Vector2 topLeft;
	Vector2 bottomRight;
};

class RenderObject;
class Texture;

class SoftwareRasteriser : public Window	{
public:
	SoftwareRasteriser(uint width, uint height);
	~SoftwareRasteriser(void);

	void	DrawObject(RenderObject*o);

	void	ClearBuffers();
	void	SwapBuffers();

	void	SetViewMatrix(const Matrix4 &m) {
		viewMatrix		= m;
		viewProjMatrix	= projectionMatrix * viewMatrix;
	}
	
	inline void	SetProjectionMatrix(const Matrix4 &m) {
		projectionMatrix	= m;
		viewProjMatrix		= projectionMatrix * viewMatrix;
	}

	static float ScreenAreaOfTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2);

protected:
	Colour*	GetCurrentBuffer();

	void	RasterisePointsMesh(RenderObject*o);
	void	RasteriseLinesMesh(RenderObject*o);

	/*NEW PRIMITIVE FUNCTIONS*/

	void RasteriseLinestripMesh(RenderObject*o);
	void RasteriseLineloopMesh(RenderObject*o);

	void RasteriseTriFanMesh(RenderObject*o);

	/*BACK TO WHERE YOU WERE*/

	inline bool DepthFunc(int x, int y, float depthValue){
		int index = (y * screenWidth) + x;

		unsigned int castVal = (unsigned int)depthValue;

		if (castVal > depthBuffer[index]){
			return false;
		}
		depthBuffer[index] = castVal;
		return true;
	}

	virtual void Resize();

	void	RasteriseLine(const Vector4 &vertA, const Vector4 &vertB,
		const Colour &colA = Colour(255,255,255,255), const Colour &colB = Colour(255,255,255,255), 
		const Vector2 &texA = Vector2(0,0) , const Vector2 &texB = Vector2(1,1));
	
	inline void	ShadePixel(uint x, uint y, const Colour&c) {
		if(y >= screenHeight) {
			return;
		}
		if(x >= screenWidth) {
			return;
		}

		int index =  (y * screenWidth) + x;

		buffers[currentDrawBuffer][index] = c;
	}


	void	RasteriseTriMesh(RenderObject*o);

	void	RasteriseTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2, 
		const Colour &c0 = Colour(), const Colour &c1 = Colour(), const Colour &c2= Colour(),
		const Vector3 &t0 = Vector3(), const Vector3 &t1= Vector3(), const Vector3 &t2	= Vector3());
	
	int		currentDrawBuffer;

	Colour*	buffers[2];

	unsigned short*	depthBuffer;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Matrix4 textureMatrix;

	Matrix4	viewProjMatrix;

	Matrix4	portMatrix;
	
	BoundingBox CalculateBoxForTri(const Vector4 &a, const Vector4 &b, const Vector4 &c);

};

