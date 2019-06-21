#include "SoftwareRasteriser.h"
#include <cmath>
#include <math.h>
/*
While less 'neat' than just doing a 'new', like in the tutorials, it's usually
possible to render a bit quicker to use direct pointers to the drawing area
that the OS gives you. For a bit of a speedup, you can uncomment the define below
to switch to using this method.

For those of you new to the preprocessor, here's a quick explanation:

Preprocessor definitions like #define allow parts of a file to be selectively enabled
or disabled at compile time. This is useful for hiding parts of the codebase on a
per-platform basis: if you have support for linux and windows in your codebase, obviously
the linux platform won't have the windows platform headers available, so compilation will
fail. So instead you can hide away all the platform specific stuff:

#if PLATFORM_WINDOWS
 DoSomeWindowsStuff();
#elif PLATFORM_LINUX
 DoSomeLinuxStuff();
 #else
 #error Unsupported Platform Specified!
 #endif

 As in our usage, it also allows you to selectively compile in some different functionality
 without any 'run time' cost - if it's not enabled by the preprocessor, it won't make it to
 the compiler, so no assembly will be generated.

Also, I've implemented the Resize method for you, in a manner that'll behave itself
no matter which method you use. I kinda forgot to do that, so there was a chance you'd
get exceptions if you resized to a bigger screen area. Sorry about that.
*/
//#define USE_OS_BUFFERS

SoftwareRasteriser::SoftwareRasteriser(uint width, uint height)	: Window(width, height){
	currentDrawBuffer	= 0;

#ifndef USE_OS_BUFFERS
	//Hi! In the tutorials, it's mentioned that we need to form our front + back buffer like so:
	for (int i = 0; i < 2; ++i) {
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	//This works, but we can actually save a memcopy by rendering directly into the memory the 
	//windowing system gives us, which I've added to the Window class as the 'bufferData' pointers
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	depthBuffer		=	new unsigned short[screenWidth * screenHeight];

	float zScale	= (pow(2.0f,16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

SoftwareRasteriser::~SoftwareRasteriser(void)	{
#ifndef USE_OS_BUFFERS
	for(int i = 0; i < 2; ++i) {
		delete[] buffers[i];
	}
#endif
	delete[] depthBuffer;
}

void SoftwareRasteriser::Resize() {
	Window::Resize(); //make sure our base class gets to do anything it needs to

#ifndef USE_OS_BUFFERS
	for (int i = 0; i < 2; ++i) {
		delete[] buffers[i];
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	delete[] depthBuffer;
	depthBuffer = new unsigned short[screenWidth * screenHeight];

	float zScale = (pow(2.0f, 16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

Colour*	SoftwareRasteriser::GetCurrentBuffer() {
	return buffers[currentDrawBuffer];
}

void	SoftwareRasteriser::ClearBuffers() {
	Colour* buffer = GetCurrentBuffer();

	unsigned int clearVal = 0xFF000000;
	unsigned int depthVal = ~0;

	for(uint y = 0; y < screenHeight; ++y) {
		for(uint x = 0; x < screenWidth; ++x) {
			buffer[(y * screenWidth) + x].Reset();//c  = clearVal;
			depthBuffer[(y * screenWidth) + x] = depthVal;
		}
	}
}

void	SoftwareRasteriser::SwapBuffers() {
	PresentBuffer(buffers[currentDrawBuffer]);
	currentDrawBuffer = !currentDrawBuffer;
}

void	SoftwareRasteriser::DrawObject(RenderObject*o) {
	switch (o->GetMesh()->GetType()){
		case PRIMITIVE_POINTS:{
			RasterisePointsMesh(o);
		}break;
		case PRIMITIVE_LINES:{
			RasteriseLinesMesh(o);
		}break;
		case PRIMITIVE_LINESTRIP:{
			RasteriseLinestripMesh(o);
		}break;
		case PRIMITIVE_LINELOOP:{
			RasteriseLineloopMesh(o);
		}break;
		case PRIMITIVE_TRIANGLES:{
			RasteriseTriMesh(o);
		}break;
		case PRIMITIVE_TRIFAN:{
			RasteriseTriFanMesh(o);
		}break;
	}
}

void	SoftwareRasteriser::RasterisePointsMesh(RenderObject*o) {

	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	for (uint i = 0; i < o->GetMesh()->numVertices; ++i)
	{
		Vector4 vertexPos = mvp * o->GetMesh()->vertices[i];
		vertexPos.SelfDivisionByW();

		Vector4 screenPos = portMatrix * vertexPos;
		ShadePixel((uint)screenPos.x, (uint)screenPos.y, Colour::White);
	}

}

void	SoftwareRasteriser::RasteriseLinesMesh(RenderObject*o) {

	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	for (uint i = 0; i < o->GetMesh()->numVertices; i += 2)
	{
		Vector4 v0 = mvp * o->GetMesh()->vertices[i];
		Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];

		Colour c0 = o->GetMesh()->colours[i];
		Colour c1 = o->GetMesh()->colours[i + 1];

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();

		RasteriseLine(v0, v1, c0, c1);
	}

}

void SoftwareRasteriser::RasteriseLinestripMesh(RenderObject*o){

	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	for (uint i = 0; i < o->GetMesh()->numVertices; ++i)
	{
		Vector4 v0 = mvp * o->GetMesh()->vertices[i];
		Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();

		RasteriseLine(v0, v1);
	}

}

void SoftwareRasteriser::RasteriseLineloopMesh(RenderObject*o){

	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
	uint max = o->GetMesh()->numVertices;

	for (uint i = 0; i < max; ++i)
	{
		if (i == max - 1)
		{
			Vector4 v0 = mvp * o->GetMesh()->vertices[i];
			Vector4 v1 = mvp * o->GetMesh()->vertices[0];

			v0.SelfDivisionByW();
			v1.SelfDivisionByW();

			RasteriseLine(v0, v1);
		}
		else
		{
			Vector4 v0 = mvp * o->GetMesh()->vertices[i];
			Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];

			v0.SelfDivisionByW();
			v1.SelfDivisionByW();

			RasteriseLine(v0, v1);
		}
	}

}

void SoftwareRasteriser::RasteriseLine(	const Vector4 &vertA, const Vector4 &vertB,
	const Colour &colA, const Colour &colB,
										const Vector2 &texA, const Vector2 &texB){

	// Transform our NDC coordinates to screen coordinates

	Vector4 v0 = portMatrix * vertA;
	Vector4 v1 = portMatrix * vertB;
	Vector4 dir = v1 - v0; //What direction is the line going?

	int xDir = (dir.x < 0.0f) ? -1 : 1; //Left or right?
	int yDir = (dir.y < 0.0f) ? -1 : 1; //Up or down?

	int x = (int)v0.x;
	int y = (int)v0.y;

	int* target = NULL; //Increment on error overflow, x or y?
	int* scan = NULL; //Increment every iteration, x or y?
	int scanVal = 0;
	int targetVal = 0;

	float slope = 0.0f;

	int range = 0; //Bound our loop by this value;

	// If we are steep, we have to scan over the y axis
	if (abs(dir.y) > abs(dir.x))
	{
		slope = (dir.x / dir.y); // Error on x
		range = (int)abs(dir.y); // Iterate over y

		target = &x;
		scan = &y;
		scanVal = yDir;
		targetVal = xDir;
	}
	else
	{
		slope = (dir.y / dir.x); // Error on y
		range = (int)abs(dir.x); // Iterate over x

		target = &y;
		scan = &x;
		scanVal = xDir;
		targetVal = yDir;
	}

	float absSlope = abs(slope);
	float error = 0.0f;
	
	float reciprocalRange = 1.0f / range;

	for (int i = 0; i < range; ++i)
	{
		float t = i*reciprocalRange;
		Colour currentCol = colB * t + colA * (1.0f - t);

		ShadePixel(x, y, currentCol);

		error += absSlope;

		if (error > 0.5f)
		{
			error -= 1.0f;
			(*target) += targetVal;
		}
		(*scan) += scanVal;
	}
}

BoundingBox SoftwareRasteriser::CalculateBoxForTri(const Vector4 &a, const Vector4 &b, const Vector4 &c)
{
	BoundingBox box;

	box.topLeft.x = a.x; // Start with the first vertex value
	box.topLeft.x = min(box.topLeft.x, b.x); // Swap with 2 if less
	box.topLeft.x = min(box.topLeft.x, c.x); // Swap with 3 if less
	box.topLeft.x = max(box.topLeft.x, 0.0f); // Screen Bound.

	box.topLeft.y = a.y; // Start with the first vertex value
	box.topLeft.y = min(box.topLeft.y, b.y); // Swap with 2 if less
	box.topLeft.y = min(box.topLeft.y, c.y); // Swap with 3 if less
	box.topLeft.y = max(box.topLeft.y, 0.0f); // Screen Bound.

	box.bottomRight.x = a.x; // Start with the first vertex value
	box.bottomRight.x = max(box.bottomRight.x, b.x); // Swap with 2 if more
	box.bottomRight.x = max(box.bottomRight.x, c.x); // Swap with 3 if more
	box.bottomRight.x = min(box.bottomRight.x, screenWidth); // Screen Bound.

	box.bottomRight.y = a.y; // Start with the first vertex value
	box.bottomRight.y = max(box.bottomRight.y, b.y); // Swap with 2 if more
	box.bottomRight.y = max(box.bottomRight.y, c.y); // Swap with 3 if more
	box.bottomRight.y = min(box.bottomRight.y, screenHeight); // Screen Bound.

	return box;
}

float SoftwareRasteriser::ScreenAreaOfTri(const Vector4 &a, const Vector4 &b, const Vector4 &c)
{
	float area = ((a.x*b.y) + (b.x*c.y) + (c.x*a.y)) -
			((b.x*a.y) + (c.x*b.y) + (a.x*c.y));
	return area * 0.5f;
}

void	SoftwareRasteriser::RasteriseTriMesh(RenderObject*o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	for (uint i = 0; i < o->GetMesh()->numVertices; i += 3)
	{
		Vector4 v0 = mvp * o->GetMesh()->vertices[i];
		Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];
		Vector4 v2 = mvp * o->GetMesh()->vertices[i + 2];

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();
		v2.SelfDivisionByW();

		RasteriseTri(v0, v1, v2,
			o->GetMesh()->colours[i],
			o->GetMesh()->colours[i + 1],
			o->GetMesh()->colours[i + 2]);
	}
}

void SoftwareRasteriser::RasteriseTri(const Vector4 &triA, const Vector4 &triB, const Vector4 &triC,
	const Colour &colA, const Colour &colB, const Colour &colC,
	const Vector3 &texA, const Vector3 &texB, const Vector3 &texC)
{
	//Incoming triangles are in NDC space
	Vector4 v0 = portMatrix * triA;
	Vector4 v1 = portMatrix * triB;
	Vector4 v2 = portMatrix * triC;

	BoundingBox b = CalculateBoxForTri(v0, v1, v2);

	float triArea = ScreenAreaOfTri(v0, v1, v2);
	float areaRecip = 1.0f / triArea;

	float subTriArea[3];
	Vector4 screenPos(0, 0, 0, 1);

	for (float y = b.topLeft.y; y < b.bottomRight.y; ++y)
	{
		for (float x = b.topLeft.x; x < b.bottomRight.x; ++x)
		{
			screenPos.x = x; // Create vertex 'p'
			screenPos.y = y; // Create vertex 'p'

			subTriArea[0] = abs(ScreenAreaOfTri(v0, screenPos, v1));
			subTriArea[1] = abs(ScreenAreaOfTri(v1, screenPos, v2));
			subTriArea[2] = abs(ScreenAreaOfTri(v2, screenPos, v0));

			float triSum = subTriArea[0] + subTriArea[1] + subTriArea[2];

			if (triSum > (triArea + 0.01f))
			{
				continue; // Current Pixel is NOT IN this triangle
			}
			if (triSum < 1.0f)
			{
				continue; // Tiny triangle we don't care about...
			}
			float alpha = subTriArea[1] * areaRecip;
			float beta = subTriArea[2] * areaRecip;
			float gamma = subTriArea[0] * areaRecip;

			float zVal = (v0.z * alpha) +
				(v1.z * beta) +
				(v2.z * gamma);

			if (!DepthFunc((int)x, (int)y, zVal)){
				continue;
			}

			Colour subColour = ((colA * alpha) +
				(colB * beta) +
				(colC * gamma));

			ShadePixel((uint)x, (uint)y, subColour);
		}
	}
}

void SoftwareRasteriser::RasteriseTriFanMesh(RenderObject*o){
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	Vector4 v0 = mvp * o->GetMesh()->vertices[0];
	v0.SelfDivisionByW();
	for (uint i = 1; i < o->GetMesh()->numVertices; ++i)
	{
		
		Vector4 v1 = mvp * o->GetMesh()->vertices[i];
		Vector4 v2 = mvp * o->GetMesh()->vertices[i + 1];

		
		v1.SelfDivisionByW();
		v2.SelfDivisionByW();

		RasteriseTri(v0, v1, v2,
			o->GetMesh()->colours[0],
			o->GetMesh()->colours[i + 1],
			o->GetMesh()->colours[i + 2]
			);
	}
}