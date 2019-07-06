//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#define DEBUG_WINDOW_WIDTH		800

#define DEBUG_WINDOW_HEIGHT		600

#define REGISTER_VIEW_WIDTH		120

#define DISASSEMBLER_VIEW_HEIGHT 105

class CDebugWindow
{
public:
	CDebugWindow();
	~CDebugWindow();
	void Initialize();
	void Invalidate();
inner:
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void SetClientSize(int nWidth, int nHeight);
	void MakeCenter();
	void FillRect(int nX, int nY, int nWidth, int nHeight, HBRUSH hBrush);
	void TextOut(int nX, int nY, const WCHAR *pszText);
	void OutputRegister(int nOutputX, int nOutputY, const WCHAR *pszRegisterName, enmDwordRegister eRegister);
	void OutputSegmentRegister(int nOutputX, int nOutputY, const WCHAR *pszRegisterName, enmWordRegister eRegister);
	void UpdateRegisterView();
	void UpdateDisassemblerView();
	void UpdateSourceCodeView();
	void UpdateDebugView();
	void StepIn();
	void StepOver();
private:
	HWND hWnd;
	CBackBuffer m_BackBuffer;
	HDC m_hdcBackBuffer;
	HBRUSH m_hBrush1;
	HBRUSH m_hBrush2;
	HBRUSH m_hBrush3;
	HFONT m_hFont1;
};

CDebugWindow *CDebugWindow_GetInstance();