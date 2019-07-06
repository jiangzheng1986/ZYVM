//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include <windows.h>

#include <stdio.h>

#include <string>

#include <vector>

#define inner public

#define ASSERT(x) if(!(x)){int i=0;i=i/i;}

#define Main() PASCAL WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR szCmdLine,int nCmdShow)

#include "CTextBuffer.h"

#include "CBackBuffer.h"

#include "CSymbolManager.h"

#include "CDisassembler.h"

#include "CVirtualMachine.h"

#include "CScreenWindow.h"

#include "CDebugWindow.h"
