//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Common.hpp"

std::wstring ANSI_To_UTF16(const CHAR *pszStr)
{
	WCHAR szBuf[10001];
	DWORD dwCodePage = 936;
	DWORD dwMinSize = MultiByteToWideChar(dwCodePage, 0, pszStr, -1, NULL, 0);
	ASSERT(dwMinSize < 10001);
	MultiByteToWideChar(dwCodePage, 0, pszStr, -1, szBuf, dwMinSize); 	
	return szBuf;
}

std::string StringToLower(const CHAR *pszStr)
{
	CHAR szBuf[10001];
	strcpy_s(szBuf,pszStr);
	int i;
	int n = strlen(szBuf);
	for ( i = 0; i < n; i++ )
	{
		szBuf[i] = tolower(szBuf[i]);
	}
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

	if ( pFile == NULL )
	{
		return FALSE;
	}

	fseek(pFile, 0, SEEK_END);

	int nFileLength = ftell(pFile);

	fseek(pFile, 0, SEEK_SET);
	
	int nCharLength = nFileLength;

	CHAR *pCharData1 = new CHAR[nCharLength + 1];

	fread(pCharData1, 1, nCharLength, pFile);

	pCharData1[nCharLength] = 0x0000;

	fclose(pFile);

	if ( ppszCharData )
	{
		*ppszCharData = pCharData1;
	}

	return TRUE;
}

void DeleteWindowsFile(WCHAR *pszFilename)
{
	::DeleteFileW(pszFilename);
}

void GetExtensionOfFile(const WCHAR *pszFilename, WCHAR *pszExtension)
{
	int n = wcslen(pszFilename);

	int i;
	for ( i = n - 1; i >= 0; i-- )
	{
		if ( pszFilename[i] == L'.' )
		{
			wcscpy_s(pszExtension, MAX_PATH, pszFilename + i + 1);

			return;
		}
	}

	wcscpy_s(pszExtension, MAX_PATH, L"");
}

void GetMainNameOfFile(const WCHAR *pszFilename, WCHAR *pszMainName)
{
	int n = wcslen(pszFilename);

	int i;
	for ( i = n - 1; i >= 0; i-- )
	{
		if ( pszFilename[i] == L'.' )
		{
			wcsncpy_s(pszMainName, MAX_PATH, pszFilename, i);

			return;
		}
	}

	wcscpy_s(pszMainName, MAX_PATH, pszFilename);
}