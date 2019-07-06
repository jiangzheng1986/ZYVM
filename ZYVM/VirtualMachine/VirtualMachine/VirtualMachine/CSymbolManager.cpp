//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

CSymbolFile::CSymbolFile()
{
	pTextBuffer = NULL;
}

CSymbolFile::~CSymbolFile()
{
	if ( pTextBuffer )
	{
		delete pTextBuffer;
		pTextBuffer = NULL;
	}
}

CSymbolItem::CSymbolItem()
{
	dwAddress = 0;
	dwFileIndex = -1;
	dwLine = 0;
}

CSymbolManager *CSymbolManager_GetInstance()
{
	static CSymbolManager s_SymbolManager;

	return &s_SymbolManager;
}

CSymbolManager::CSymbolManager()
{

}

CSymbolManager::~CSymbolManager()
{

}

void CSymbolManager::LoadSymbolFile(const WCHAR *pszSymbolFilename, const WCHAR *pszSourceFilename)
{
	FILE *pFile = NULL;
	_wfopen_s(&pFile, pszSymbolFilename, L"rb");

	DWORD dwInstrCount = 0;
	fread(&dwInstrCount, sizeof(DWORD), 1, pFile);

	CSymbolFile *pSymbolFile = new CSymbolFile();

	pSymbolFile->szSourceFilename = pszSourceFilename;

	pSymbolFile->pTextBuffer = new CTextBuffer();

	pSymbolFile->pTextBuffer->LoadTextFile(pszSourceFilename);

	vecSymbolFileList.push_back(pSymbolFile);

	DWORD dwFileIndex = vecSymbolFileList.size() - 1;

	tagAsmSymbolFileItem stSymbolFileItem;

	int i;
	for ( i = 0; i < (int)dwInstrCount; i++ )
	{
		fread(&stSymbolFileItem, sizeof(tagAsmSymbolFileItem), 1, pFile);

		CSymbolItem *pSymbolItem = new CSymbolItem();
		pSymbolItem->dwAddress = stSymbolFileItem.dwAddress;
		pSymbolItem->dwFileIndex = dwFileIndex;
		pSymbolItem->dwLine = stSymbolFileItem.dwLine;
		vecSymbolItemList.push_back(pSymbolItem);
	}

	fclose(pFile);
}

CSymbolItem *CSymbolManager::FindSymbolItem(DWORD dwAddress)
{
	int i;
	for ( i = 0; i < (int)vecSymbolItemList.size(); i++ )
	{
		CSymbolItem *pSymbolItem = vecSymbolItemList[i];

		if ( pSymbolItem->dwAddress == dwAddress )
		{
			return pSymbolItem;
		}
	}

	return NULL;
}

CSymbolFile *CSymbolManager::FindSymbolFile(DWORD dwFileIndex)
{
	if ( dwFileIndex != -1 )
	{
		return vecSymbolFileList[dwFileIndex];
	}
	else
	{
		return NULL;
	}
}
