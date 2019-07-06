//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CCScanner.hpp"

#include "CCSyntaxTree.hpp"

#include "Utility.hpp"

//基于回溯的语法分析器

class CCParser
{
public:
	CCParser(void);
	~CCParser(void);
	CCSyntaxTree *Parse(void);
private:
	void UpdateCurrentToken(void);
	void NextToken(void);
	int GetCurrentTokenIndex(void);
	void SetCurrentTokenIndex(int nCurrentTokenIndex);
	void MatchToken(enmCTokenType eExpectedTokenType);
	CCSyntaxTree *ParseProgram(void);
	CCSyntaxTree *ParseDeclarationList(void);
	CCSyntaxTree *ParseDeclaration(void);
	CCSyntaxTree *ParseVarDeclaration(void);
	CCSyntaxTree *ParseTypeSpecifier(void);
	CCSyntaxTree *ParseFunDeclaration(void);
	CCSyntaxTree *ParseParams(void);
	CCSyntaxTree *ParseParamList(void);
	CCSyntaxTree *ParseParam(void);
	CCSyntaxTree *ParseCompoundStmt(void);
	void ParseLocalDeclarations(CCSyntaxTree *pTree);
	void ParseStatementList(CCSyntaxTree *pTree);
	CCSyntaxTree *ParseStatement(void);
	CCSyntaxTree *ParseExpressionStmt(void);
	CCSyntaxTree *ParseExpression(void);
	CCSyntaxTree *ParseLogic1Expression(void);
	CCSyntaxTree *ParseLogic2Expression(void);
	CCSyntaxTree *ParseSimpleExpression(void);
	void ParseRelop(void);
	CCSyntaxTree *ParseAdditiveExpression(void);
	void ParseAddop(void);
	CCSyntaxTree *ParseTerm(void);
	void ParseMulop(void);
	CCSyntaxTree *ParseUnaryExpression(void);
	CCSyntaxTree *ParsePostfixExpression(void);
	CCSyntaxTree *ParseFactor(void);
	CCSyntaxTree *ParseVar(void);
	CCSyntaxTree *ParseCall(void);
	CCSyntaxTree *ParseArgs(void);
	CCSyntaxTree *ParseArgList(void);
	CCSyntaxTree *ParseIfStmt(void);
	CCSyntaxTree *ParseWhileStmt(void);
	CCSyntaxTree *ParseDoWhileStmt(void);
	CCSyntaxTree *ParseForStmt(void);
	CCSyntaxTree *ParseBreakStmt(void);
	CCSyntaxTree *ParseContinueStmt(void);
	CCSyntaxTree *ParseReturnStmt(void);
	BOOL IsTokenTypeSpecifier(enmCTokenType eTokenType);
	enmCTokenType m_eTokenType;
	char m_szTokenString[1000];
	int m_nLineNumber;
	BOOL m_bShowFirstErrorOnly;
	BOOL m_bFirstErrorShowed;
};

CCParser *CCParser_GetInstance(void);