//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

class CBackBuffer
{
public:
	CBackBuffer(void);
	~CBackBuffer(void);
	void Initialize(int w,int h);
	void Release(void);
	HDC GetDC(void);
	void Draw(HDC hdc);
	void Draw(HDC hdc, int x, int y, int w, int h);
private:
	int w;
	int h;
	HBITMAP hBitmapBackBuffer;
	HDC hdcBackBuffer;
};