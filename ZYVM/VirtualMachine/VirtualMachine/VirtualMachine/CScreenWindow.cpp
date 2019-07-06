//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

LRESULT CALLBACK ScreenWindow_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CScreenWindow_GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

CScreenWindow *CScreenWindow_GetInstance()
{
	static CScreenWindow s_ScreenWindow;

	return &s_ScreenWindow;
}

CScreenWindow::CScreenWindow()
{
	hWnd = NULL;
}

CScreenWindow::~CScreenWindow()
{

}

void CScreenWindow::Initialize()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= ScreenWindow_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"ZYVM_ScreenWindow";
	wcex.hIconSm		= NULL;

	RegisterClassExW(&wcex);

	hWnd = CreateWindowW(L"ZYVM_ScreenWindow", L"ÐéÄâ»úÆÁÄ»", WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	SetClientSize(SCREEN_WINDOW_WIDTH, SCREEN_WINDOW_HEIGHT);

	MakeCenter();

	m_BackBuffer.Initialize(SCREEN_WINDOW_WIDTH, SCREEN_WINDOW_HEIGHT);

	m_hdcBackBuffer = m_BackBuffer.GetDC();

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
}

void CScreenWindow::SetClientSize(int nWidth, int nHeight)
{
	RECT ClientRect,WndRect;
	GetClientRect(hWnd,&ClientRect);
	GetWindowRect(hWnd,&WndRect);
	
	DWORD nWidth1=(WndRect.right-WndRect.left)+(nWidth-ClientRect.right);
	DWORD nHeight1=(WndRect.bottom-WndRect.top)+(nHeight-ClientRect.bottom);
	
	MoveWindow(hWnd,WndRect.left,WndRect.top,nWidth1,nHeight1,TRUE);
}

void CScreenWindow::MakeCenter()
{
	int parentX = 0;
	int parentY = 0;
	int parentWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int parentHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	int x = parentX + (parentWidth - width) / 2;
	int y = parentY + (parentHeight - height) / 2;

	MoveWindow(hWnd, x, y, width, height, TRUE);
}

void CScreenWindow::SetPixel(int nX, int nY, DWORD dwColor)
{
	::SetPixel(m_hdcBackBuffer, nX, nY, dwColor);
}

DWORD CScreenWindow::GetPixel(int nX, int nY)
{
	return ::GetPixel(m_hdcBackBuffer, nX, nY);
}

void CScreenWindow::ClearScreen(DWORD dwClearColor)
{
	HBRUSH hBrush = CreateSolidBrush(dwClearColor);

	RECT rect;
	rect.left = 0;
	rect.right = SCREEN_WINDOW_WIDTH;
	rect.top = 0;
	rect.bottom = SCREEN_WINDOW_HEIGHT;
	::FillRect(m_hdcBackBuffer, &rect, hBrush);
	
	DeleteObject((HGDIOBJ)hBrush);
}

void CScreenWindow::FillRect(DWORD dwClearColor, int nX, int nY, int nWidth, int nHeight)
{
	HBRUSH hBrush = CreateSolidBrush(dwClearColor);

	RECT rect;
	rect.left = nX;
	rect.right = nX + nWidth;
	rect.top = nY;
	rect.bottom = nY + nHeight;
	::FillRect(m_hdcBackBuffer, &rect, hBrush);

	DeleteObject((HGDIOBJ)hBrush);
}

void CScreenWindow::Invalidate()
{
	InvalidateRect(hWnd, NULL, FALSE);
}

LRESULT CScreenWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		m_BackBuffer.Draw(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			BYTE byVKey = (BYTE)wParam;
			
			CVirtualMachine_GetInstance()->OutPort(enmOprandLength_Byte, 0x60, byVKey);

			CVirtualMachine_GetInstance()->DoInterrupt(0x09);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
