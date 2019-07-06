//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CAsmSymbolFileGenerator.hpp"

CAsmSymbolFileGenerator *CAsmSymbolFileGenerator_GetInstance()
{
	static CAsmSymbolFileGenerator s_AsmSymbolFileGenerator;

	return &s_AsmSymbolFileGenerator;
}

CAsmSymbolFileGenerator::CAsmSymbolFileGenerator()
{

}

CAsmSymbolFileGenerator::~CAsmSymbolFileGenerator()
{

}

void CAsmSymbolFileGenerator::AddSymbolFileItem(DWORD dwAddress, DWORD dwLine)
{
	tagAsmSymbolFileItem stSymbolFileItem;
	stSymbolFileItem.dwAddress = dwAddress;
	stSymbolFileItem.dwLine = dwLine;

	m_vecSymbolItemList.push_back(stSymbolFileItem);
}

void CAsmSymbolFileGenerator::SaveToFile(const WCHAR *pszFilename)
{
	FILE *pFile = NULL;
	_wfopen_s(&pFile, pszFilename, L"wb");
	
	DWORD dwInstrCount = m_vecSymbolItemList.size();
	fwrite(&dwInstrCount, sizeof(DWORD), 1, pFile);

	int i;
	for ( i = 0; i < (int)dwInstrCount; i++ )
	{
		fwrite(&(m_vecSymbolItemList[i]), sizeof(tagAsmSymbolFileItem), 1, pFile);
	}

	fclose(pFile);
}
