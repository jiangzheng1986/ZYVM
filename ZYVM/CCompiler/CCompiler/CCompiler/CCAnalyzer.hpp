//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CCSyntaxTree.hpp"

#include "CCSymbolTable.hpp"

#include "Utility.hpp"

//语义分析要点:
//检测比较操作符操作数是否有效
//检测布尔操作符的操作数是否有效
//检测算术操作符的操作数是否有效
//检查赋值语句的两边是否匹配
//检测if,while,for等语句的条件是否为BOOL值
//检测return语句的返回值是否与函数匹配
//检查函数调用中的参数与函数定义是否匹配
//检查是否有main函数,并检查其参数和返回值
//分配全局变量空间
//分配参数变量栈空间
//分配局部变量栈空间
//分配临时变量栈空间
//检查是否所有路径都有返回值

class CCAnalyzer
{
public:
	CCAnalyzer(void);
	~CCAnalyzer(void);
	void Analyze(CCSyntaxTree *pTree);
private:
	void Analyze_Recursive(CCSyntaxTree *pTree);
	void Analyze_Children(CCSyntaxTree *pTree);
	void Analyze_Children_Reverse(CCSyntaxTree *pTree);
	void Analyze_FunDeclaration(CCSyntaxTree *pTree);
	void Analyze_ParamList(CCSyntaxTree *pTree);
	void Analyze_Param(CCSyntaxTree *pTree);
	void Analyze_VarDeclaration(CCSyntaxTree *pTree);
	void AllocateTempLocalVar(char *pszName, std::string szElementType);
	void Analyze_CompareExpression(CCSyntaxTree *pTree);
	void Analyze_ArithExpression(CCSyntaxTree *pTree);
	void Analyze_LogicExpression(CCSyntaxTree *pTree);
	void Analyze_UnaryExpression(CCSyntaxTree *pTree);
	void Analyze_PostfixExpression(CCSyntaxTree *pTree);
	void Analyze_AssignExpression(CCSyntaxTree *pTree);
	void Analyze_Conversion(CCSyntaxTree *pTree);
	void Analyze_Conversion_NoRecursive(CCSyntaxTree *pTree);
	void Analyze_Var(CCSyntaxTree *pTree);
	void Analyze_Number(CCSyntaxTree *pTree);
	void Analyze_Call(CCSyntaxTree *pTree);
	void Analyze_Other(CCSyntaxTree *pTree);
	std::string m_szCurrentScope;
};

CCAnalyzer *CCAnalyzer_GetInstance(void);