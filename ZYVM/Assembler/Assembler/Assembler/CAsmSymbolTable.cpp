//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CAsmSymbolTable.hpp"

CAsmSymbolItem::CAsmSymbolItem(void)
{
	m_nAddress = 0;
	m_nSize = 0;
	m_pTree = NULL;
}

CAsmSymbolTable::CAsmSymbolTable(void)
{
}

CAsmSymbolTable::~CAsmSymbolTable(void)
{
	Release();
}

void CAsmSymbolTable::Release(void)
{
	std::map<std::string,CAsmSymbolItem *>::iterator it;
	for ( it = m_mapSymbolItemMap.begin(); it != m_mapSymbolItemMap.end(); it++ )
	{
		DELETE_CPP(it->second);
	}
	m_mapSymbolItemMap.clear();
}

void CAsmSymbolTable::AddSymbolItem(CAsmSymbolItem *pSymbolItem)
{
	if ( pSymbolItem->m_szName.length() > 0 && pSymbolItem->m_szName[0] == '@' )
	{
		m_vecLocalSymbolItemList.push_back(pSymbolItem);
	}

	m_mapSymbolItemMap[pSymbolItem->m_szName] = pSymbolItem;
}

CAsmSymbolItem *CAsmSymbolTable::SearchSymbolItem(const char *name)
{
	std::map<std::string,CAsmSymbolItem *>::iterator it;
	it = m_mapSymbolItemMap.find(name);
	if ( it != m_mapSymbolItemMap.end() )
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CAsmSymbolTable::PrintSymbolTable(void)
{
	std::map<std::string,CAsmSymbolItem *>::iterator it;
	for ( it = m_mapSymbolItemMap.begin(); it != m_mapSymbolItemMap.end(); it++ )
	{
		CAsmSymbolItem *pSymbolItem = it->second;

		printf("%s\t%s\t%d\t%d\n",
			pSymbolItem->m_szName.c_str(),
			pSymbolItem->m_szType.c_str(),
			pSymbolItem->m_nAddress,
			pSymbolItem->m_nSize);
	}
}

void CAsmSymbolTable::ClearLocalSymbols(void)
{
	int i;
	for ( i = m_vecLocalSymbolItemList.size() - 1; i >= 0; i-- )
	{
		CAsmSymbolItem *pSymbolItem = m_vecLocalSymbolItemList[i];

		m_mapSymbolItemMap.erase(pSymbolItem->m_szName);

		delete pSymbolItem;
	}

	m_vecLocalSymbolItemList.clear();
}

CAsmSymbolTable *CAsmSymbolTable_GetInstance(void)
{
	static CAsmSymbolTable s_SymbolTable;

	return &s_SymbolTable;
}

CAsmLabelRef::CAsmLabelRef(void)
{
	m_nAddress = 0;
	m_nBinCodeEmitPos = 0;
	m_pTree = NULL;
}

CAsmLabelRef::~CAsmLabelRef(void)
{

}

CAsmLabelRefTable::CAsmLabelRefTable(void)
{

}

CAsmLabelRefTable::~CAsmLabelRefTable(void)
{
}

void CAsmLabelRefTable::AddLabelRef(const char *pLabel, int nAddress, int nBinCodeEmitPos, CAsmSyntaxTree *pTree)
{
	CAsmLabelRef *pLabelRef = new CAsmLabelRef();

	pLabelRef->m_szLabel = pLabel;
	pLabelRef->m_nAddress = nAddress;
	pLabelRef->m_nBinCodeEmitPos = nBinCodeEmitPos;
	pLabelRef->m_pTree = pTree;

	m_vecLabelRefList.push_back(pLabelRef);
}

void CAsmLabelRefTable::RemoveLabelRefsOfLabel(const char *pLabel)
{
	int i;

	for ( i = m_vecLabelRefList.size() - 1; i >= 0; i-- )
	{
		if ( strcmp(m_vecLabelRefList[i]->m_szLabel.c_str(), pLabel) == 0 )
		{
			delete m_vecLabelRefList[i];
			m_vecLabelRefList.erase(m_vecLabelRefList.begin() + i);
		}
	}
}

int CAsmLabelRefTable::GetLabelRefCount()
{
	return m_vecLabelRefList.size();
}

CAsmLabelRef *CAsmLabelRefTable::GetLabelRef(int i)
{
	return m_vecLabelRefList[i];
}

CAsmLabelRefTable g_LabelRefTable;

CAsmLabelRefTable *CAsmLabelRefTable_GetInstance(void)
{
	return &g_LabelRefTable;
}