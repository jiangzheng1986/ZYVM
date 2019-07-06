//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

//基于XML表示的动态语法树

class CAsmSyntaxTreeAttr
{
public:
	std::string m_szAttrName;
	std::string m_szAttrValue;
};

class CAsmSyntaxTree
{
public:
	CAsmSyntaxTree(const char *pszType, int nLineNumber);
	~CAsmSyntaxTree(void);
	void Release(void);
	std::string GetType(void);
	void SetAttrValue(const char *pszAttrName, const char *pszAttrValue);
	std::string GetAttrValue(const char *pszAttrName);
	void AddChild(CAsmSyntaxTree *pszChild);
	void InsertChild(CAsmSyntaxTree *pszChild, int i);
	int GetChildCount(void);
	CAsmSyntaxTree *GetChild(int i);
	CAsmSyntaxTree *GetChildByTag(const char *pszTag);
	CAsmSyntaxTree *GetChildByName(const char *pszName);
	int GetLineNumber(void);
	void Print(void);
private:
	void PrintIndent(void);
	void Print1(void);
	std::string m_szType;
	int m_nLineNumber;
	std::vector<CAsmSyntaxTreeAttr *> m_vecAttrList;
	std::vector<CAsmSyntaxTree *> m_vecChildList;
	static int s_nIndent;
};

extern CAsmSyntaxTree *g_pRoot;

CAsmSyntaxTree *CAsmSyntaxTree_GetRoot(void);