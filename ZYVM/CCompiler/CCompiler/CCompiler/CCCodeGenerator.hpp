//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Utility.hpp"

#include "CCSyntaxTree.hpp"

#include "CCSymbolTable.hpp"

//代码生成要点:
//1.所有带操作的表达式节点都有一个局部变量(TempVar)保存其值
//2.所有表达式节点均有一个字符串属性(Code属性),可直接将该属性赋值到寄存器
//3.内部有一个递增的标号,0号标号默认为函数结束
//4.参数变量,局部变量,临时变量等用ebp访问

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