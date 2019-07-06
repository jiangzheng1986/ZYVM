//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include <windows.h>

#include <stdio.h>

#include <vector>

#include <map>

#include <string>

#define DELETE_CPP(x) if(x){delete x;x=NULL;}

#define ASSERT(x) if((!(x))){int i=0;i=i/i;}

std::wstring ANSI_To_UTF16(const CHAR *pszStr);

std::string StringToLower(const CHAR *pszStr);

BOOL ReadAnsiFile(const WCHAR *pszFilename, CHAR **ppszCharData);

void DeleteWindowsFile(WCHAR *pszFilename);

void GetExtensionOfFile(const WCHAR *pszFilename, WCHAR *pszExtension);

void GetMainNameOfFile(const WCHAR *pszFilename, WCHAR *pszMainName);