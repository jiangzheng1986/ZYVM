//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

LRESULT CALLBACK DebugWindow_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDebugWindow_GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

CDebugWindow *CDebugWindow_GetInstance()
{
	static CDebugWindow s_DebugWindow;

	return &s_DebugWindow;
}

CDebugWindow::CDebugWindow()
{
	hWnd = NULL;
}

CDebugWindow::~CDebugWindow()
{

}

void CDebugWindow::Initialize()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= DebugWindow_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"ZYVM_DebugWindow";
	wcex.hIconSm		= NULL;

	RegisterClassExW(&wcex);

	hWnd = CreateWindowW(L"ZYVM_DebugWindow", L"µ÷ÊÔÆÁÄ»", WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
		10, 10, 0, 0, NULL, NULL, hInstance, NULL);

	SetClientSize(DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT);

	MakeCenter();

	m_BackBuffer.Initialize(DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT);

	m_hdcBackBuffer = m_BackBuffer.GetDC();

	SetBkMode(m_hdcBackBuffer, TRANSPARENT);

	m_hBrush1 = CreateSolidBrush(RGB(128, 128, 128));

	m_hBrush2 = CreateSolidBrush(RGB(192, 192, 192));

	m_hBrush3 = CreateSolidBrush(RGB(156, 156, 156));

	m_hFont1 = CreateFont(16,0,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,TEXT("FIXEDSYS"));
	
	SelectObject(m_hdcBackBuffer, m_hFont1);

	CSymbolManager_GetInstance()->LoadSymbolFile(L"ZYOS_BOOT.sym", L"ZYOS_BOOT.asm");

	CSymbolManager_GetInstance()->LoadSymbolFile(L"ZYOS_KERNEL.sym", L"ZYOS_KERNEL.asm");

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
}

void CDebugWindow::SetClientSize(int nWidth, int nHeight)
{
	RECT ClientRect,WndRect;
	GetClientRect(hWnd,&ClientRect);
	GetWindowRect(hWnd,&WndRect);

	DWORD nWidth1=(WndRect.right-WndRect.left)+(nWidth-ClientRect.right);
	DWORD nHeight1=(WndRect.bottom-WndRect.top)+(nHeight-ClientRect.bottom);

	MoveWindow(hWnd,WndRect.left,WndRect.top,nWidth1,nHeight1,TRUE);
}

void CDebugWindow::MakeCenter()
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

void CDebugWindow::Invalidate()
{
	InvalidateRect(hWnd, NULL, FALSE);
}

LRESULT CDebugWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		UpdateDebugView();

		m_BackBuffer.Draw(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			switch ( wParam )
			{
			case VK_F10:
				StepOver();
				break;
			case VK_F11:
				StepIn();
				break;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CDebugWindow::FillRect(int nX, int nY, int nWidth, int nHeight, HBRUSH hBrush)
{
	RECT rect;
	rect.left = nX;
	rect.right = nX + nWidth;
	rect.top = nY;
	rect.bottom = nY + nHeight;
	::FillRect(m_hdcBackBuffer, &rect, hBrush);
}

void CDebugWindow::TextOut(int nX, int nY, const WCHAR *pszText)
{
	int nLength = (int)wcslen(pszText);

	::TextOutW(m_hdcBackBuffer, nX, nY, pszText, nLength);
}

void CDebugWindow::OutputRegister(int nOutputX, int nOutputY, const WCHAR *pszRegisterName, enmDwordRegister eRegister)
{
	WCHAR szBuffer[1000];
	DWORD dwRegister = CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Dword, eRegister);
	swprintf_s(szBuffer, L"%s=%08X", pszRegisterName, dwRegister);
	TextOut(nOutputX, nOutputY, szBuffer);
}

void CDebugWindow::OutputSegmentRegister(int nOutputX, int nOutputY, const WCHAR *pszRegisterName, enmWordRegister eRegister)
{
	WCHAR szBuffer[1000];
	WORD wSegmentRegister = (WORD)CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Word, eRegister);
	swprintf_s(szBuffer, L"%s=%04X", pszRegisterName, wSegmentRegister);
	TextOut(nOutputX, nOutputY, szBuffer);
}

void CDebugWindow::UpdateRegisterView()
{
	int nRegisterViewX = DEBUG_WINDOW_WIDTH - REGISTER_VIEW_WIDTH;
	int nRegisterViewY = 0;
	FillRect(nRegisterViewX, nRegisterViewY, REGISTER_VIEW_WIDTH, DEBUG_WINDOW_HEIGHT, m_hBrush2);

	int nOutputX = nRegisterViewX + 10;
	int nOutputY = nRegisterViewY + 5;
	
	OutputRegister(nOutputX, nOutputY, L"EAX", enmDwordRegister_EAX);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"ECX", enmDwordRegister_ECX);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EDX", enmDwordRegister_EDX);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EBX", enmDwordRegister_EBX);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"ESP", enmDwordRegister_ESP);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EBP", enmDwordRegister_EBP);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"ESI", enmDwordRegister_ESI);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EDI", enmDwordRegister_EDI);
	nOutputY += 20;

	OutputSegmentRegister(nOutputX, nOutputY, L"ES", enmWordRegister_ES);
	nOutputY += 20;

	OutputSegmentRegister(nOutputX, nOutputY, L"CS", enmWordRegister_CS);
	nOutputY += 20;

	OutputSegmentRegister(nOutputX, nOutputY, L"SS", enmWordRegister_SS);
	nOutputY += 20;

	OutputSegmentRegister(nOutputX, nOutputY, L"DS", enmWordRegister_DS);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EIP", enmDwordRegister_EIP);
	nOutputY += 20;

	OutputRegister(nOutputX, nOutputY, L"EFL", enmDwordRegister_EFLAGS);
	nOutputY += 20;

	int nFlagOffsetX = 8;

	bool bOF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_OF);

	if ( bOF )
	{
		TextOut(nOutputX, nOutputY, L"O");
		nOutputX += nFlagOffsetX;
	}

	bool bDF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_DF);

	if ( bDF )
	{
		TextOut(nOutputX, nOutputY, L"D");
		nOutputX += nFlagOffsetX;
	}

	bool bIF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_IF);

	if ( bIF )
	{
		TextOut(nOutputX, nOutputY, L"I");
		nOutputX += nFlagOffsetX;
	}

	bool bTF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_TF);

	if ( bTF )
	{
		TextOut(nOutputX, nOutputY, L"T");
		nOutputX += nFlagOffsetX;
	}

	bool bSF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_SF);

	if ( bSF )
	{
		TextOut(nOutputX, nOutputY, L"S");
		nOutputX += nFlagOffsetX;
	}

	bool bZF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_ZF);

	if ( bZF )
	{
		TextOut(nOutputX, nOutputY, L"Z");
		nOutputX += nFlagOffsetX;
	}

	bool bAF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_AF);

	if ( bAF )
	{
		TextOut(nOutputX, nOutputY, L"A");
		nOutputX += nFlagOffsetX;
	}

	bool bPF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_PF);

	if ( bPF )
	{
		TextOut(nOutputX, nOutputY, L"P");
		nOutputX += nFlagOffsetX;
	}

	bool bCF = CVirtualMachine_GetInstance()->GetEflagsBit(EFLAGS_BIT_CF);

	if ( bCF )
	{
		TextOut(nOutputX, nOutputY, L"C");
		nOutputX += nFlagOffsetX;
	}
}

void CDebugWindow::UpdateDisassemblerView()
{
	int nDisassemblerViewWidth = DEBUG_WINDOW_WIDTH - REGISTER_VIEW_WIDTH;

	FillRect(0, 0, nDisassemblerViewWidth, DISASSEMBLER_VIEW_HEIGHT, m_hBrush3);

	DWORD dwEIP = CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

	int nCodeBytes = 0;

	BYTE *pbySystemMemory = CVirtualMachine_GetInstance()->GetSystemMemoryPointer();

	int nOutputX = 22;
	int nOutputY = 5;

	int i;
	for ( i = 0; i < 5; i++ )
	{
		std::string szCode = CDisassembler_GetInstance()->Disassemble(dwEIP, &(pbySystemMemory[dwEIP]), nCodeBytes);

		WCHAR szBuf[1000];
		swprintf_s(szBuf, L"%S", szCode.c_str());
		TextOut(nOutputX, nOutputY, szBuf);
		nOutputY += 20;

		dwEIP += nCodeBytes;
	}

	TextOut(4, 5, L">>");
}

void CDebugWindow::UpdateSourceCodeView()
{
	DWORD dwEIP = CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

	CSymbolItem *pSymbolItem = CSymbolManager_GetInstance()->FindSymbolItem(dwEIP);

	if ( !pSymbolItem )
	{
		return;
	}

	CSymbolFile *pSymbolFile = CSymbolManager_GetInstance()->FindSymbolFile(pSymbolItem->dwFileIndex);

	if ( !pSymbolFile )
	{
		return;
	}

	CTextBuffer *pTextBuffer = pSymbolFile->pTextBuffer;

	int nTargetLine = pSymbolItem->dwLine;

	int nOutputX = 22;
	int nOutputY = DISASSEMBLER_VIEW_HEIGHT + 5;

	int i;
	int n = pTextBuffer->GetLineCount();
	int nStart = max(nTargetLine - 12, 0);
	int nEnd = min(n, nStart + 25);
	for ( i = nStart; i < nEnd; i++ )
	{
		const CHAR *pszLine = pTextBuffer->GetLine(i);
		
		std::wstring pszLine1 = ANSI_To_UTF16(pszLine);

		TextOut(nOutputX, nOutputY, pszLine1.c_str());
		nOutputY += 20;
	}

	int nCurrentLine = nTargetLine - nStart - 1;
	TextOut(4, DISASSEMBLER_VIEW_HEIGHT + 5 + 20 * nCurrentLine, L">>");
}

void CDebugWindow::UpdateDebugView()
{
	FillRect(0, 0, DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT, m_hBrush1);

	UpdateSourceCodeView();

	UpdateDisassemblerView();

	UpdateRegisterView();
}

void CDebugWindow::StepIn()
{
	bool bContinue = CVirtualMachine_GetInstance()->Execute();

	if ( !bContinue )
	{
		PostQuitMessage(0);
	}

	Invalidate();
}

void CDebugWindow::StepOver()
{
	DWORD dwEIP = CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

	BYTE *pbySystemMemory = CVirtualMachine_GetInstance()->GetSystemMemoryPointer();

	if ( pbySystemMemory[dwEIP] == 0xCC ||	//CALL
		 pbySystemMemory[dwEIP] == 0xD0 ||	//INT
		 pbySystemMemory[dwEIP] == 0xD6 )	//INT 3
	{
		int nCodeBytes = 0;

		std::string szCode = CDisassembler_GetInstance()->Disassemble(dwEIP, &(pbySystemMemory[dwEIP]), nCodeBytes);

		DWORD dwNextInstrEIP = dwEIP + nCodeBytes;

		while(TRUE)
		{
			bool bContinue = CVirtualMachine_GetInstance()->Execute();

			if ( !bContinue )
			{
				PostQuitMessage(0);
				break;
			}

			DWORD dwNewEIP = CVirtualMachine_GetInstance()->GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

			if ( dwNewEIP == dwNextInstrEIP )
			{
				break;
			}
		}
	}
	else
	{
		bool bContinue = CVirtualMachine_GetInstance()->Execute();

		if ( !bContinue )
		{
			PostQuitMessage(0);
		}
	}

	Invalidate();
}