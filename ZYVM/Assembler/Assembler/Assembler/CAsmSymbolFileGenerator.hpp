//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

class tagAsmSymbolFileItem
{
public:
	DWORD dwAddress;
	DWORD dwLine;
};

class CAsmSymbolFileGenerator
{
public:
	CAsmSymbolFileGenerator();
	~CAsmSymbolFileGenerator();
	void AddSymbolFileItem(DWORD dwAddress, DWORD dwLine);
	void SaveToFile(const WCHAR *pszFilename);
private:
	std::vector<tagAsmSymbolFileItem> m_vecSymbolItemList;
};

CAsmSymbolFileGenerator *CAsmSymbolFileGenerator_GetInstance();