//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author��AndersJiang(����)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Utility.hpp"

#include "CCSyntaxTree.hpp"

#include "CCSymbolTable.hpp"

//��������Ҫ��:
//1.���д������ı��ʽ�ڵ㶼��һ���ֲ�����(TempVar)������ֵ
//2.���б��ʽ�ڵ����һ���ַ�������(Code����),��ֱ�ӽ������Ը�ֵ���Ĵ���
//3.�ڲ���һ�������ı��,0�ű��Ĭ��Ϊ��������
//4.��������,�ֲ�����,��ʱ��������ebp����

class CCCodeGenerator
{
public:
	CCCodeGenerator(void);
	~CCCodeGenerator(void);
	void OpenAsmCodeFile(WCHAR *pszFilename);
	void CloseAsmCodeFile(void);
	void GenerateCode(CCSyntaxTree *pTree);
private:
	void GenerateCode_Recursive(CCSyntaxTree *pTree);
	void GenerateCode_Children(CCSyntaxTree *pTree);
	void GenerateCode_Children_Reverse(CCSyntaxTree *pTree);
	void GenerateCode_Children_Specific(CCSyntaxTree *pTree, std::string szType);
	void GenerateCode_Children_Except(CCSyntaxTree *pTree, std::string szType);
	void GenerateCode_Other(CCSyntaxTree *pTree);
	void GenerateCode_DeclarationList(CCSyntaxTree *pTree);
	void GenerateCode_FunDeclaration(CCSyntaxTree *pTree);
	void GenerateCode_TempVars(CCSyntaxTree *pTree);
	void GenerateCode_TempVar(CCSyntaxTree *pTree);
	void GenerateCode_ParamList(CCSyntaxTree *pTree);
	void GenerateCode_Param(CCSyntaxTree *pTree);
	void GenerateCode_VarDeclaration(CCSyntaxTree *pTree);
	void GenerateCode_InitExpression(CCSyntaxTree *pTree);
	void GenerateCode_CompareExpression(CCSyntaxTree *pTree);
	void GenerateCode_ArithExpression(CCSyntaxTree *pTree);
	void GenerateCode_LogicExpression(CCSyntaxTree *pTree);
	void GenerateCode_UnaryExpression(CCSyntaxTree *pTree);
	void GenerateCode_PostfixExpression(CCSyntaxTree *pTree);
	void GenerateCode_AssignExpression(CCSyntaxTree *pTree);
	void GenerateCode_Conversion(CCSyntaxTree *pTree);
	void GenerateCode_IfStmt(CCSyntaxTree *pTree);
	void GenerateCode_WhileStmt(CCSyntaxTree *pTree);
	void GenerateCode_DoWhileStmt(CCSyntaxTree *pTree);
	void GenerateCode_ReturnStmt(CCSyntaxTree *pTree);
	void GenerateCode_ArgList(CCSyntaxTree *pTree);
	void GenerateCode_Call(CCSyntaxTree *pTree);
	void EmitCode(const char *pszFormat, ...);
	FILE *m_pFile;
	std::string m_szCurrentScope;
	int m_nCurrentLabelIndex;
	BOOL m_nInitVar;
};

CCCodeGenerator *CCCodeGenerator_GetInstance(void);