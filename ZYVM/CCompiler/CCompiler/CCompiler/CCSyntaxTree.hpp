//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

//基于XML表示的动态语法树

class CCSyntaxTreeAttr
{
public:
	std::string szAttrName;
	std::string szAttrValue;
};

class CCSyntaxTree
{
public:
	CCSyntaxTree(std::string szType, int nLineNumber);
	~CCSyntaxTree(void);
	void Release(void);
	void SetType(std::string szType);
	std::string GetType(void);
	void SetAttrValue(std::string szAttrName, std::string szAttrValue);
	std::string GetAttrValue(std::string szAttrName);
	void AddChild(CCSyntaxTree *pChild);
	void RemoveChild(CCSyntaxTree *pChild);
	void InsertChild(CCSyntaxTree *pChild, int i);
	int GetChildCount(void);
	CCSyntaxTree *GetChild(int i);
	CCSyntaxTree *GetChildByTag(std::string szTag);
	CCSyntaxTree *GetChildByName(std::string szName);
	int GetLineNumber(void);
	void Print(void);
private:
	void PrintIndent(void);
	void Print1(void);
	std::string m_szType;
	int m_nLineNumber;
	std::vector<CCSyntaxTreeAttr *> m_vecAttrList;
	std::vector<CCSyntaxTree *> m_vecChildList;
	static int s_nIndent;
};

extern CCSyntaxTree *g_pRoot;

CCSyntaxTree *CCSyntaxTree_GetRoot(void);