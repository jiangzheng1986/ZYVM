//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCompiler.hpp"

std::wstring ANSI_To_UTF16(const CHAR *pszStr)
{
	WCHAR szBuf[10001];
	DWORD dwCodePage = 936;
	DWORD dwMinSize = MultiByteToWideChar(dwCodePage, 0, pszStr, -1, NULL, 0);
	ASSERT(dwMinSize < 10001);
	MultiByteToWideChar(dwCodePage, 0, pszStr, -1, szBuf, dwMinSize); 	
	return szBuf;
}

BOOL ReadAnsiFile(const WCHAR *pszFilename, CHAR **ppszCharData)
{
	if ( !pszFilename || !ppszCharData )
	{
		return FALSE;
	}

	*ppszCharData = NULL;

	FILE *pFile = NULL;
	_wfopen_s(&pFile, pszFilename, L"rb");

	if(!pFile)
	{
		//ShowEngineDebuggingWarningMessage(L"Failed to open file:%s",filename);

		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);

	int nFileLength = ftell(pFile);

	fseek(pFile, 0, SEEK_SET);

	int nCharLength = nFileLength;

	CHAR *pszCharData1 = new CHAR[nCharLength + 1];

	fread(pszCharData1, sizeof(CHAR), nCharLength, pFile);

	pszCharData1[nCharLength] = 0x0000;

	fclose(pFile);

	if ( ppszCharData )
	{
		*ppszCharData = pszCharData1;
	}

	return TRUE;
}