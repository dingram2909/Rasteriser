#include "Window.h"

Window::Window(uint width, uint height)	{
	hasInit = false;
	HINSTANCE hInstance = GetModuleHandle( NULL );

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	if(!GetClassInfoEx(hInstance,WINDOWCLASS,&windowClass))	{
		windowClass.cbSize		= sizeof(WNDCLASSEX);
		windowClass.style		= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc	= (WNDPROC)StaticWindowProc;
		windowClass.hInstance	= hInstance;
		windowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		windowClass.lpszClassName = WINDOWCLASS;

		if(!RegisterClassEx(&windowClass)) {
			return;
		}
	}

	windowHandle = CreateWindowEx(NULL,
		WINDOWCLASS,    // name of the window class
		"Software Rasteriser!",   // title of the window
		WS_OVERLAPPEDWINDOW|WS_POPUP|WS_VISIBLE|WS_SYSMENU|WS_MAXIMIZEBOX|WS_MINIMIZEBOX,    // window style
		(int)100,	// x-position of the window
		(int)100,	// y-position of the window
		(int)width,		// width of the window
		(int)height,		// height of the window
		NULL,				// No parent window!
		NULL,				// No Menus!
		hInstance,			// application handle
		this);				// 

	deviceContext=GetDC(windowHandle);

	RECT    rt; 
	GetClientRect(windowHandle, &rt);

	screenWidth		= rt.right;
	screenHeight	= rt.bottom;

	BuildBitmap();

	Keyboard::Initialise(windowHandle);
	Mouse::Initialise(windowHandle);

	hasInit		= true;
	forceQuit	= false;
}


Window::~Window(void)	{
	DeleteObject(bitBuffers[0]);
	DeleteObject(bitBuffers[1]);
	DeleteDC(drawDC);

	Keyboard::Destroy();
	Mouse::Destroy();
}

void Window::BuildBitmap() {
	DeleteObject(bitBuffers[0]);
	DeleteObject(bitBuffers[1]);
	DeleteDC(drawDC);

	BITMAPINFO bmi; 

	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= screenWidth;
	bmi.bmiHeader.biHeight		= screenHeight;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biBitCount	= 32;         // four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage	= screenWidth * screenHeight * 3;

	drawDC	= CreateCompatibleDC(deviceContext);

	for (int i = 0; i < 2; ++i) {
		bitBuffers[i] = CreateDIBSection(drawDC, &bmi, DIB_RGB_COLORS, &bufferData[i], NULL, 0x0);
	}
}

void Window::PresentBuffer(Colour*buffer) {
	if ((void*)buffer == bufferData[0]) {
		SelectObject(drawDC, bitBuffers[0]);
	}
	else {
		SelectObject(drawDC, bitBuffers[1]);
		//Using the student provided mem buffer, must memcopy!
		if ((void*)buffer != bufferData[0] && ((void*)buffer != bufferData[1])) {
			memcpy(bufferData[1], buffer, screenWidth * screenHeight * sizeof(unsigned int));
		}
	}
	BitBlt(deviceContext, 0, 0, screenWidth, screenHeight, drawDC, 0, 0, SRCCOPY);
}

LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)	{
	switch(message)	 {
		case(WM_CREATE):{
			Window* w = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtr(hWnd,GWL_USERDATA,reinterpret_cast<long>(w));
		}break;
		case(WM_DESTROY):	{
			PostQuitMessage(0);
			forceQuit = true;
		}break;
		case (WM_ACTIVATE): {
			if(LOWORD(wParam) == WA_INACTIVE)	{
				ReleaseCapture();
				ClipCursor(NULL);
				if (hasInit) {
					Mouse::instance->Sleep();
					Mouse::instance->Sleep();
				}
			}
			else{
				if(hasInit) {
					Mouse::instance->Wake();	
					Mouse::instance->Wake();
				}
			}
			return 0;
		}break;
		case (WM_LBUTTONDOWN): {

		}break;

		case (WM_MOUSEMOVE): {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = windowHandle;
			TrackMouseEvent(&tme);
		}break;
		case(WM_SIZE): {
			screenWidth		= LOWORD(lParam);
			screenHeight	= HIWORD(lParam);

			BuildBitmap();

			Resize();
		}break;

		case(WM_SETFOCUS) : {
			if (hasInit) {
				Mouse::instance->Wake(); 
				Mouse::instance->Wake();
			}
		}break;
		case(WM_KILLFOCUS) : {
			if (hasInit) {
				Mouse::instance->Sleep();
				Mouse::instance->Sleep();
			}
		}break;
	}
	return DefWindowProc (hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)	{
	Window* window = (Window*)GetWindowLongPtr(hWnd,GWL_USERDATA);
	return window->WindowProc(hWnd,message,wParam,lParam);
}

bool	Window::UpdateWindow() {
	Keyboard::instance->UpdateHolds();
	Mouse::instance->UpdateHolds();

	MSG		msg;
	while(PeekMessage(&msg,windowHandle,0,0,PM_REMOVE)) {
		CheckMessages(msg); 
	}

	return !forceQuit;
}

void Window::CheckMessages(MSG &msg)	{
	switch (msg.message)	{				// Is There A Message Waiting?
		case (WM_QUIT):
		case (WM_CLOSE): {					// Have We Received A Quit Message?
			forceQuit = true;
		}break;
		case (WM_INPUT): {
			UINT dwSize;
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &dwSize,sizeof(RAWINPUTHEADER));

			BYTE* lpb = new BYTE[dwSize];

			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize,sizeof(RAWINPUTHEADER));
			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (Keyboard::instance && raw->header.dwType == RIM_TYPEKEYBOARD) {
				Keyboard::instance->Update(raw);
			}
			else if (Mouse::instance && raw->header.dwType == RIM_TYPEMOUSE) {
				Mouse::instance->Update(raw);
			}
			delete lpb;
		}break;

		default: {								// If Not, Deal With Window Messages
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
	}
}