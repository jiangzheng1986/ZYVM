//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "CCSyntaxTree.hpp"

//基于函数作用域的符号表

class CCSymbolItem
{
public:
	CCSymbolItem(void);
	std::string m_szScope;
	std::string m_szName;
	std::string m_szType;
	std::string m_szElementType;
	int m_nCurrentTempVarIndex;
	int m_nCurrentLocalLabelIndex;
	int m_nMaxParamAddress;
	int m_nMinLocalAddress;
	int m_nAddress;
	int m_nSize;
	CCSyntaxTree *m_pTree;
};

class CCSymbolTable
{
public:
	CCSymbolTable(void);
	~CCSymbolTable(void);
	void Release(void);
	void AddSymbolItem(CCSymbolItem *pSymbolItem);
	CCSymbolItem *GetSymbolItem(std::string szScope, std::string szName);
	CCSymbolItem *SearchSymbolItem(std::string szScope, std::string szName);
	void PrintSymbolTable(void);
private:
	std::vector<CCSymbolItem *> m_vecSymbolItemList;
};

CCSymbolTable *CCSymbolTable_GetInstance(void);