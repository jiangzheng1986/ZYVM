//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

CBackBuffer::CBackBuffer(void)
{
	hBitmapBackBuffer=0;
	hdcBackBuffer=0;
}

CBackBuffer::~CBackBuffer(void)
{
	Release();
}

void CBackBuffer::Initialize(int w,int h)
{
	Release();
	this->w=w;
	this->h=h;
	HDC hdc=::GetDC(NULL);
	hBitmapBackBuffer=CreateCompatibleBitmap(hdc,w,h);
	hdcBackBuffer=CreateCompatibleDC(hdc);
	SelectObject(hdcBackBuffer,hBitmapBackBuffer);
	DeleteDC(hdc);
}

void CBackBuffer::Release(void)
{
	if(hBitmapBackBuffer)
	{
		DeleteObject(hBitmapBackBuffer);
		hBitmapBackBuffer=NULL;
	}
	if(hdcBackBuffer)
	{
		DeleteDC(hdcBackBuffer);
		hdcBackBuffer=NULL;
	}
}

HDC CBackBuffer::GetDC(void)
{
	return hdcBackBuffer;
}

void CBackBuffer::Draw(HDC hdc)
{
	BitBlt(hdc,0,0,w,h,hdcBackBuffer,0,0,SRCCOPY);
}

void CBackBuffer::Draw(HDC hdc, int x, int y, int w, int h)
{
	BitBlt(hdc,x,y,w,h,hdcBackBuffer,x,y,SRCCOPY);
}