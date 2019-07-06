//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#define SCREEN_WINDOW_WIDTH		1024

#define SCREEN_WINDOW_HEIGHT	768

class CScreenWindow
{
public:
	CScreenWindow();
	~CScreenWindow();
	void Initialize();
	void SetPixel(int nX, int nY, DWORD dwColor);
	DWORD GetPixel(int nX, int nY);
	void ClearScreen(DWORD dwClearColor);
	void FillRect(DWORD dwClearColor, int nX, int nY, int nWidth, int nHeight);
	void Invalidate();
inner:
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void SetClientSize(int nWidth, int nHeight);
	void MakeCenter();
private:
	HWND hWnd;
	CBackBuffer m_BackBuffer;
	HDC m_hdcBackBuffer;
};

CScreenWindow *CScreenWindow_GetInstance();