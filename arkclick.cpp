// arkclick.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "arkclick.h"
#include <gl/glew.h>
#include <opencv2/opencv.hpp>
#include <immintrin.h>  


#ifdef _DEBUG
#pragma comment(lib,"opencv_world401d.lib")
#else
#pragma comment(lib,"opencv_world401.lib")
#endif 

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HDC hdc1, hdc2;
HGLRC m_hrc;
int mx, my, cx, cy;
double ang1, ang2, len, cenx, ceny, cenz;
GLuint imgtex;

HWND bbhwnd;
HWND bbhwnd1;
HDC bbhdc, bbhdcc;
HBITMAP hbitmap;
BITMAP bmp;
RECT bbrect;
unsigned char* data;
cv::Mat tar;
float* imgf, * tarf;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void draw(void) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(0x00004100);

	glBindTexture(GL_TEXTURE_2D, imgtex);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0, -1.0f, 0.0f);
	glEnd();

	SwapBuffers(hdc1);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ARKCLICK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARKCLICK));

    MSG msg;

    // Main message loop:
	for (;;) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (bbhwnd) {
				//BitBlt(bbhdcc, 0, 0, bbrect.right - bbrect.left, bbrect.bottom - bbrect.top, bbhdc, 0, 0, SRCCOPY);
				PrintWindow(bbhwnd, bbhdcc, PW_CLIENTONLY);
				GetBitmapBits(hbitmap, bmp.bmHeight * bmp.bmWidthBytes, data);
				int i, j;
				for (i = 0; i < 8; i++) {
					for (j = 0; j < 8; j++) {
						tarf[(i * 8 + j) * 4] = (float)tar.data[(i * 2 * 16 + j * 2) * 3] + (float)tar.data[(i * 2 * 16 + j * 2 + 1) * 3] + (float)tar.data[(i * 2 * 16 + j * 2 + 16) * 3] + (float)tar.data[(i * 2 * 16 + j * 2 + 17) * 3];
						tarf[(i * 8 + j) * 4 + 1] = (float)tar.data[(i * 2 * 16 + j * 2) * 3 + 1] + (float)tar.data[(i * 2 * 16 + j * 2 + 1) * 3 + 1] + (float)tar.data[(i * 2 * 16 + j * 2 + 16) * 3 + 1] + (float)tar.data[(i * 2 * 16 + j * 2 + 17) * 3 + 1];
						tarf[(i * 8 + j) * 4 + 2] = (float)tar.data[(i * 2 * 16 + j * 2) * 3 + 2] + (float)tar.data[(i * 2 * 16 + j * 2 + 1) * 3 + 2] + (float)tar.data[(i * 2 * 16 + j * 2 + 16) * 3 + 2] + (float)tar.data[(i * 2 * 16 + j * 2 + 17) * 3 + 2];
						tarf[(i * 8 + j) * 4 + 3] = 0.0;
					}
				}
				for (i = 0; i < bmp.bmHeight / 2; i++) {
					for (j = 0; j < bmp.bmWidth/2; j++) {
						imgf[(i * (bmp.bmWidth/2) + j) * 4] = (float)data[(i * 2 * bmp.bmWidth + j * 2) *4] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + 1) *4] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth) *4] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth + 1) *4];
						imgf[(i * (bmp.bmWidth / 2) + j) * 4 + 1] = (float)data[(i * 2 * bmp.bmWidth + j * 2) *4 + 1] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + 1) *4 + 1] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth) *4 + 1] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth + 1) *4 + 1];
						imgf[(i * (bmp.bmWidth / 2) + j) * 4 + 2] = (float)data[(i * 2 * bmp.bmWidth + j * 2) *4 + 2] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + 1) *4 + 2] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth) *4 + 2] + (float)data[(i * 2 * bmp.bmWidth + j * 2 + bmp.bmWidth + 1) *4 + 2];
						imgf[(i * (bmp.bmWidth / 2) + j) * 4 + 3] = 0.0;
					}
				}
				int m, l;
				int  mx, my;
				__m256 sum;
				float min, sumf;
				min = 255 * 4 * 255 * 4 * 8 * 8 * 3 + 1;
				for (i = 0; i < bmp.bmHeight / 2 - 7; i++) {
					for (j = 0; j < bmp.bmWidth/2 - 7; j++) {
						sum = _mm256_set1_ps(0.0f);
						for (m = 0; m < 8; m+=1) {
							for (l = 0; l < 8; l+=2) {
								__m256 x1, x2, x3, x4;
								x1 = _mm256_loadu_ps(imgf + ((i + m) * (bmp.bmWidth / 2) + j + l) * 4);
								x2 = _mm256_loadu_ps(tarf + (m * 8 + l) * 4);
								x3 = _mm256_sub_ps(x1, x2);
								sum= _mm256_add_ps(sum, _mm256_mul_ps(x3, x3));
							}
						}
						sumf = sum.m256_f32[0] + sum.m256_f32[1] + sum.m256_f32[2] + sum.m256_f32[3] + sum.m256_f32[4] + sum.m256_f32[5] + sum.m256_f32[6] + sum.m256_f32[7];
						if (sumf < min) {
							min = sumf;
							my = i*2;
							mx = j*2;
						}
					}
				}
				for (m = 0; m < 16; m += 1) {
					for (l = 0; l < 16; l += 1) {
						data[((my + m) * bmp.bmWidth + (mx + l)) * 4] = 255;
						data[((my + m) * bmp.bmWidth + (mx + l)) * 4+1] = 0;
						data[((my + m) * bmp.bmWidth + (mx + l)) * 4 + 2] = 0;
					}
				}
				glBindTexture(GL_TEXTURE_2D, imgtex);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.bmWidth, bmp.bmHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
				draw();
				glBindTexture(GL_TEXTURE_2D, 0);
				if (min < 4500000) {
					//SendMessageA(bbhwnd1, WM_LBUTTONDOWN, 0, mx+12 + ((my+12) << 16));
					//SendMessageA(bbhwnd1, WM_LBUTTONUP, 0, mx+12 + ((my+12) << 16)); 
					SetCursorPos(mx +8,my+8); 
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					Sleep(1000);
				}
				else {
					Sleep(100);
				}
			}
			else {
				bbhwnd = FindWindowA(NULL, "雷电模拟器");
				bbhdc = GetDC(bbhwnd);
			}
		}
	}

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARKCLICK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ARKCLICK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_CREATE: {
		int i, j;
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,0,0,
			0,
			0,0,0,0,
			32,
			0,0,
			PFD_MAIN_PLANE,
			0,0,0,0
		};
		hdc1 = GetDC(hWnd);
		hdc2 = GetDC(NULL);
		int uds = ::ChoosePixelFormat(hdc1, &pfd);
		::SetPixelFormat(hdc1, uds, &pfd);
		m_hrc = ::wglCreateContext(hdc1);
		::wglMakeCurrent(hdc1, m_hrc);
		//glewInit();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);

		glGenTextures(1, &imgtex);
		glBindTexture(GL_TEXTURE_2D, imgtex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		bbhwnd1 = FindWindowA(NULL, "雷电模拟器");
		bbhwnd = bbhwnd1; GetDesktopWindow();
		bbhdc = GetDC(bbhwnd);
		bbhdcc = CreateCompatibleDC(bbhdc);
		GetClientRect(bbhwnd, &bbrect);
		hbitmap = CreateCompatibleBitmap(bbhdc, bbrect.right - bbrect.left, bbrect.bottom - bbrect.top);
		SelectObject(bbhdcc, hbitmap);
		//BitBlt(bbhdcc, 0, 0, bbrect.right - bbrect.left, bbrect.bottom - bbrect.top, bbhdc, 0, 0, SRCCOPY);
		PrintWindow(bbhwnd, bbhdcc, PW_CLIENTONLY);
		GetObject(hbitmap, sizeof(BITMAP), &bmp);
		//frame = new cv::Mat(bmp.bmHeight, bmp.bmWidth, CV_8UC4, cv::Scalar(0, 0, 0, 0));
		data = (unsigned char*)malloc(bmp.bmHeight * bmp.bmWidth * 4);
		GetBitmapBits(hbitmap, bmp.bmHeight * bmp.bmWidthBytes, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.bmWidth, bmp.bmHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		RECT ra, rb;
		GetClientRect(hWnd, &ra);
		GetWindowRect(hWnd, &rb);
		MoveWindow(hWnd, 0, 0, bmp.bmWidth / 4 + rb.right - ra.right + ra.left - rb.left, bmp.bmHeight / 4 + rb.bottom - ra.bottom + ra.top - rb.top, true);
		tar=cv::imread("C:/files/0.png");
		tarf = (float*)malloc(8 * 8 * 4 * 4);
		imgf = (float*)malloc((bmp.bmHeight/2) * (bmp.bmWidth/2) * 4 * 4);
		break;
	}
	case WM_SIZE: {
		cx = lParam & 0xffff;
		cy = (lParam & 0xffff0000) >> 16;
		glViewport(0, 0, cx, cy);
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
