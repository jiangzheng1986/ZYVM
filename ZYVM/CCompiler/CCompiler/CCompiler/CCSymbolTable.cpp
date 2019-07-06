//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCSymbolTable.hpp"

CCSymbolItem::CCSymbolItem(void)
{
	m_nCurrentTempVarIndex = 0;
	m_nCurrentLocalLabelIndex = 0;
	m_nMaxParamAddress = 0; 
	m_nMinLocalAddress = 0;
	m_nAddress = 0;
	m_nSize = 0;
	m_pTree = NULL;
}

CCSymbolTable::CCSymbolTable(void)
{
}

CCSymbolTable::~CCSymbolTable(void)
{
	Release();
}

void CCSymbolTable::Release(void)
{
	int i;
	for ( i = 0; i < (int)m_vecSymbolItemList.size(); i++ )
	{
		delete m_vecSymbolItemList[i];
	}

	m_vecSymbolItemList.clear();
}

void CCSymbolTable::AddSymbolItem(CCSymbolItem *pSymbolItem)
{
	m_vecSymbolItemList.push_back(pSymbolItem);
}

CCSymbolItem *CCSymbolTable::GetSymbolItem(std::string szScope, std::string szName)
{
	int i;
	for ( i = 0; i < (int)m_vecSymbolItemList.size(); i++ )
	{
		CCSymbolItem *pSymbolItem = m_vecSymbolItemList[i];

		if ( pSymbolItem->m_szScope == szScope && pSymbolItem->m_szName == szName )
		{
			return pSymbolItem;
		}
	}

	return NULL;
}

CCSymbolItem *CCSymbolTable::SearchSymbolItem(std::string szScope, std::string szName)
{
	CCSymbolItem *pSymbolItem = GetSymbolItem(szScope, szName);

	if ( pSymbolItem )
	{
		return pSymbolItem;
	}

	return GetSymbolItem("::", szName);
}

void CCSymbolTable::PrintSymbolTable(void)
{
	int i;
	for ( i = 0; i < (int)m_vecSymbolItemList.size(); i++ )
	{
		CCSymbolItem *pSymbolItem = m_vecSymbolItemList[i];

		printf("%s\t%s\t%s\t%s\t%d\t%d\t%d\n",
			pSymbolItem->m_szScope.c_str(),
			pSymbolItem->m_szName.c_str(),
			pSymbolItem->m_szType.c_str(),
			pSymbolItem->m_szElementType.c_str(),
			pSymbolItem->m_nMaxParamAddress,
			pSymbolItem->m_nMinLocalAddress,
			pSymbolItem->m_nAddress);
	}
}

CCSymbolTable *CCSymbolTable_GetInstance(void)
{
	static CCSymbolTable s_CSymbolTable;

	return &s_CSymbolTable;
}