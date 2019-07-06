//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

class tagAsmSymbolFileItem
{
public:
	DWORD dwAddress;
	DWORD dwLine;
};

class CSymbolItem
{
public:
	CSymbolItem();
public:
	DWORD dwAddress;
	DWORD dwFileIndex;
	DWORD dwLine;
};

class CSymbolFile
{
public:
	CSymbolFile();
	~CSymbolFile();
public:
	std::wstring szSourceFilename;
	CTextBuffer *pTextBuffer;
};

class CSymbolManager
{
public:
	CSymbolManager();
	~CSymbolManager();
	void LoadSymbolFile(const WCHAR *pszSymbolFilename, const WCHAR *pszSourceFilename);
	CSymbolItem *FindSymbolItem(DWORD dwAddress);
	CSymbolFile *FindSymbolFile(DWORD dwFileIndex);
private:
	std::vector<CSymbolItem *> vecSymbolItemList;
	std::vector<CSymbolFile *> vecSymbolFileList;
};

CSymbolManager *CSymbolManager_GetInstance();