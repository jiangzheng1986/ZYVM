//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "CAsmSyntaxTree.hpp"

//基于过程作用域的符号表

class CAsmSymbolItem
{
public:
	CAsmSymbolItem(void);
	std::string m_szName;
	std::string m_szType;
	int m_nAddress;
	int m_nSize;
	CAsmSyntaxTree *m_pTree;
};

class CAsmSymbolTable
{
public:
	CAsmSymbolTable(void);
	~CAsmSymbolTable(void);
	void Release(void);
	void AddSymbolItem(CAsmSymbolItem *pSymbolItem);
	CAsmSymbolItem *SearchSymbolItem(const char *pszName);
	void PrintSymbolTable(void);
	void ClearLocalSymbols(void);
private:
	std::map<std::string,CAsmSymbolItem *> m_mapSymbolItemMap;
	std::vector<CAsmSymbolItem *> m_vecLocalSymbolItemList;
};

CAsmSymbolTable *CAsmSymbolTable_GetInstance(void);

class CAsmLabelRef
{
public:
	CAsmLabelRef(void);
	~CAsmLabelRef(void);
	std::string m_szLabel;
	int m_nAddress;
	int m_nBinCodeEmitPos;
	CAsmSyntaxTree *m_pTree;
};

class CAsmLabelRefTable
{
public:
	CAsmLabelRefTable(void);
	~CAsmLabelRefTable(void);
	void AddLabelRef(const char *pszLabel, int nAddress, int nBinCodeEmitPos, CAsmSyntaxTree *pTree);
	void RemoveLabelRefsOfLabel(const char *pszLabel);
	int GetLabelRefCount();
	CAsmLabelRef *GetLabelRef(int i);
private:
	std::vector<CAsmLabelRef *> m_vecLabelRefList;
};

CAsmLabelRefTable *CAsmLabelRefTable_GetInstance(void);