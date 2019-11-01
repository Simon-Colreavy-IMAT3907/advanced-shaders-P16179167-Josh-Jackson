#include "stdafx.h"

/*! Fast timer. 
*	We wend a message to ourselves each timer tick and then utilise peek message in the
*	message loop for speed drawing. If the fast timer causes any issues, we can fall back on a standard
*	windows timers to drive the frame rate.
*/
#define	WM_FASTTIMERTICK	(WM_USER + 1) 
#define USE_FAST_TIMER (1)

#include "InitProgram.h"
#include "Main.h" //!< Used to call the function which interface with OpenGL and start the program flow.

#define MAX_LOADSTRING 100

/*!Global Variables:*/
HINSTANCE hInst;                                //!< Current instance
WCHAR szTitle[MAX_LOADSTRING];                  //!< The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            //!< The main window class name
HWND hWnd;

int fps = 60;
unsigned int fastTimerDelay = 1000 / fps;		//!< Compute number of ms per frame

#if USE_FAST_TIMER
HANDLE ptrTimerHandle;
#else
int windowsTimerID = 1;
#endif

/*! keyboard state stored here 256 bits = 32 unsigned chars*/
static unsigned char keyState[32]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned char masks[8]{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

Main main; //!< Holds the application code and interfaces with OpenGL.

/*! Forward declarations of functions included in this code module:*/
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void DealWithKeyPress(HWND hWnd, WPARAM wParam, bool isKeydown);
bool InitialiseFastTimer();
void ShutdownFastTimer();
void __stdcall FastTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*! Initialize global strings*/
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IMAT2902_EX06, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	/*! Perform application initialization:*/
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

#if USE_FAST_TIMER
	if (!InitialiseFastTimer())
	{
		return FALSE;
	}
#else
	/*! use the windows timer - less accurate as a fallback*/
	SetTimer(hWnd, windowsTimerID, fastTimerDelay, (TIMERPROC)NULL);
#endif

	//High Res Mouse Input

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif


	// hires mouse
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));


	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32GL_EX1));

	HDC hdc = GetDC(hWnd);
	MSG msg;

	/*! prime the message structure*/

	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	while (msg.message != WM_QUIT)  //!< last message will exit
	{
		/*! Check to see if any messages are waiting in the queue*/
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)			//!< | PM_QS_POSTMESSAGE
		{
			/*! Translate the message and dispatch it to WindowProc()*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (hdc != NULL)
			{
				main.Draw();
				SwapBuffers(hdc);
			}
		}
	} // while(msg.message != WM_QUIT)
	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMAT2902_EX06));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IMAT2902_EX06);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//   In this function, we save the instance handle and window handle in a global variable and
//   create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(
		szWindowClass,			// szWindowClass: the name of the application
		szTitle,				// szTitle: the text that appears in the title bar
		WS_OVERLAPPEDWINDOW,	// WS_OVERLAPPEDWINDOW: the type of window to create 
		CW_USEDEFAULT,			// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
		CW_USEDEFAULT,
		1024,					// initial size (width, length) 
		768,
		nullptr,				// nullptr: the parent of this window  
		nullptr,				// nullptr: this application does not have a menu bar
		hInstance,				// hInstance: the first parameter from WinMain
		nullptr);				// nullptr: not used in this application

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int wmId, wmEvent;
	//PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	switch (message)
	{
	case WM_FASTTIMERTICK:
	case WM_TIMER:

		main.Update();
		//invalidate the window to force a WM_PAINT call
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;

		// no WM_PAINT handler - we draw outside peek message in pump (so is passed to DefWinProc())


	case WM_CREATE:

		// here is where we create our open gl context
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		main.CreateGLWindow(hWnd, rect);

		// prepare vbo's and vao's and setup shader
		main.PrepareToDraw();
		ReleaseDC(hWnd, hdc);
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;


	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		hdc = GetDC(hWnd);
		// pass to opengl
		main.Resize(hdc, rect);
		ReleaseDC(hWnd, hdc);

		ClipCursor(&rect);
		break;

	case WM_KEYDOWN:
		DealWithKeyPress(hWnd, wParam, true);
		break;

	case WM_KEYUP:
		DealWithKeyPress(hWnd, wParam, false);
		break;

	case WM_CLOSE:
		//if (MessageBox(hWnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		main.DestroyGLWindow();
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_INPUT:
	{
		UINT dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;

			main.HandleMouseInput(xPosRelative, yPosRelative);
		}

		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#if USE_FAST_TIMER

// revised to use modern timer functions 24/10/17
void __stdcall
FastTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	PostMessage(hWnd, WM_FASTTIMERTICK, 0, 0);
}

bool InitialiseFastTimer()
{
	CreateTimerQueueTimer(&ptrTimerHandle, NULL, FastTimerCallback, NULL, fastTimerDelay, fastTimerDelay, WT_EXECUTEDEFAULT);
	return true;
}

void ShutdownFastTimer()
{
	DeleteTimerQueueTimer(NULL, ptrTimerHandle, NULL);
}
#endif


//-----------------------------------------------------------------------------
//
// void	gameDealWithKeyPress(HWND hWnd,WPARAM wParam)
//
//-----------------------------------------------------------------------------

void	DealWithKeyPress(HWND hWnd, WPARAM wParam, bool isKeydown)
{
	// see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	//
	// for the VK_ codes defined in windows.h
	// trap escape to exit program
	if (isKeydown && wParam == VK_ESCAPE)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	// deal with other keys here
	// we start 256 bits (32 unsigned chars) each holding 1 == pressed 0 == not
	int keyByte = (wParam & 0xF8) >> 3;
	unsigned char keyMask = masks[(wParam & 0x07)];
	if (isKeydown)
	{
		// set the relevant bit
		keyState[keyByte] |= keyMask;
		// pass the keypress to the example object
		main.HandleInput(wParam);
	}
	else
	{
		// clear the bit
		keyState[keyByte] &= (~keyMask);
	}
}

bool IsKeyPressed(unsigned char keyCode)
{
	// we could test keys state here...
	// keyCode is in 0-0xFF  - see ms VK_ codes.
	int keyByte = (keyCode & 0xF8) >> 3;
	unsigned char keyMask = masks[(keyCode & 0x07)];
	return ((keyState[keyByte] & keyMask) != 0);
}


