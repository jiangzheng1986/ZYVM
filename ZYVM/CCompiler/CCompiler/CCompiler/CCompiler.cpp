//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCompiler.hpp"

void CompileCFile(const WCHAR *pszFilename)
{
	WCHAR szFilenameC[1000];

	wsprintfW(szFilenameC, L"%s.c", pszFilename);

	WCHAR szFilenameASM[1000];

	wsprintfW(szFilenameASM, L"%s.asm", pszFilename);

	BOOL b = CCScanner_GetInstance()->OpenSource(szFilenameC);

	if ( b == FALSE )
	{
		return;
	}

	CCScanner_GetInstance()->BuildTokenList();

	CCScanner_GetInstance()->CloseSource();

	CCScanner_GetInstance()->PrintTokenList();

	g_pRoot = CCParser_GetInstance()->Parse();

	CCSyntaxTree_GetRoot()->Print();

	CCAnalyzer_GetInstance()->Analyze(g_pRoot);

	CCSymbolTable_GetInstance()->PrintSymbolTable();

	CCSyntaxTree_GetRoot()->Print();

	//DeleteFile(szFilenameASM);

	//CCCodeGenerator_GetInstance()->OpenAsmCodeFile(szFilenameASM);

	//CCCodeGenerator_GetInstance()->GenerateCode(g_pRoot);

	//CCCodeGenerator_GetInstance()->CloseAsmCodeFile();

	DELETE_CPP(g_pRoot);
}

int main(int nArgc, char *apszArgv[])
{
	printf("Hello C Compiler!\n");

	//CompileCFile(L"Test1");

	//CompileCFile(L"Test2");

	//CompileCFile(L"Test3");

	//CompileCFile(L"Test4");

	//CompileCFile(L"Test5");

	//CompileCFile(L"Test6");

	CompileCFile(L"Test7");

	//CompileCFile(L"Test8");

	while ( TRUE )
	{
		Sleep(10);
	}

	return 0;
}