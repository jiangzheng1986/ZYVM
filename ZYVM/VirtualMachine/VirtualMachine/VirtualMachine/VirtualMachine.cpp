//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

int Main()
{
	printf("Hello Virtual Machine!\n\n");

	CVirtualMachine_GetInstance()->Initialize();

	CVirtualMachine_GetInstance()->SetWritePosition(0x0000400);

	CScreenWindow_GetInstance()->Initialize();

	CDebugWindow_GetInstance()->Initialize();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}