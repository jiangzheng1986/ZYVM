//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include <windows.h>

#include <stdio.h>

#include <vector>

#include <queue>

#include <map>

#include <string>

#define DELETE_CPP(x) if(x){delete x;x=NULL;}

#define DELETE_CPP_ARRAY(x) if(x){delete [] x;x=NULL;}

#define ASSERT(x) if ( !(x) ) { int i = 0; i = i / i; }

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MIN(x, y) ((x) < (y) ? (x) : (y))

std::wstring ANSI_To_UTF16(const CHAR *pszStr);

BOOL ReadAnsiFile(const WCHAR *pszFilename, CHAR **ppszCharData);