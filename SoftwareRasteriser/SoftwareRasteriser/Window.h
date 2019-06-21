#pragma once
#include "Common.h"
#include "Colour.h"

#include "Mouse.h"
#include "Keyboard.h"


#include <windows.h>
#include <fcntl.h>

//These two defines cut a lot of crap out of the Windows libraries
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#define WINDOWCLASS "WindowClass"

//This is the OS-specific crap required to render our pixel blocks on screen
class Window	{
public:
	Window(uint width, uint height);
	~Window(void);

	void PresentBuffer(Colour*buffer);

	bool	UpdateWindow();	

protected:
	void CheckMessages(MSG &msg);

	void BuildBitmap();

	virtual void Resize() {

	};

	//Windows requires a static callback function to handle certain incoming messages.
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	LRESULT WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	HWND	windowHandle;	//OS handle
	HDC		deviceContext;

	uint	screenWidth;
	uint	screenHeight;


	HDC		drawDC;

	HBITMAP bitBuffers[2];
	void *bufferData[2];

	VOID *pvBits;          // pointer to DIB section 

	bool	forceQuit;
	bool	hasInit;
};

