//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "AsmCompiler.hpp"

void AssembleAsmFile(const WCHAR *pszFilename)
{
	WCHAR szFilename1[1000];

	GetMainNameOfFile(pszFilename, szFilename1);

	WCHAR szFilenameASM[1000];

	wsprintfW(szFilenameASM, L"%s.asm", szFilename1);

	WCHAR szFilenameBIN[1000];

	wsprintfW(szFilenameBIN, L"%s.bin", szFilename1);

	WCHAR szFilenameSYM[1000];

	wsprintfW(szFilenameSYM, L"%s.sym", szFilename1);

	BOOL b = CAsmScanner_GetInstance()->OpenSource(szFilenameASM);

	if ( b == FALSE )
	{
		return;
	}
	
	CAsmScanner_GetInstance()->BuildTokenList();

	CAsmScanner_GetInstance()->CloseSource();

	//CAsmScanner_GetInstance()->PrintTokenList();

	g_pRoot = CAsmParser_GetInstance()->Parse();

	bool bError = CAsmParser_GetInstance()->GetError();

	if ( bError )
	{
		printf("Syntax error, compilation failed.\n");
		DELETE_CPP(g_pRoot);

		return;
	}

	//YAsmSyntaxTree_GetRoot()->Print();

	CAsmInstrTable_GetInstance()->BuildInstrTable();

	DeleteWindowsFile(szFilenameBIN);

	CAsmCodeGenerator_GetInstance()->OpenBinCodeFile(szFilenameBIN);

	CAsmCodeGenerator_GetInstance()->GenerateCode(g_pRoot);

	CAsmCodeGenerator_GetInstance()->CloseBinCodeFile();

	DeleteWindowsFile(szFilenameSYM);

	CAsmSymbolFileGenerator_GetInstance()->SaveToFile(szFilenameSYM);

	//YAsmSymbolTable_GetInstance()->PrintSymbolTable();

	bError = CAsmCodeGenerator_GetInstance()->GetError();

	if ( bError )
	{
		printf("Code generate error, compilation failed.\n");
		DELETE_CPP(g_pRoot);

		return;
	}

	DELETE_CPP(g_pRoot);
}

int main(int nArgc, char *apArgv[])
{
	printf("ZY Assembly Compiler!\n");

	if ( nArgc == 2 )
	{
		std::wstring szFilename = ANSI_To_UTF16(apArgv[1]);

		AssembleAsmFile(szFilename.c_str());
	}

	//while ( TRUE )
	//{
		//Sleep(100);
	//}
	return 0;
}