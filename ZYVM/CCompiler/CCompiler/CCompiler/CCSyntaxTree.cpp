//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCSyntaxTree.hpp"

int CCSyntaxTree::s_nIndent = 0;

CCSyntaxTree::CCSyntaxTree(std::string szType, int nLineNumber)
{
	this->m_szType = szType;
	this->m_nLineNumber = nLineNumber;
}

CCSyntaxTree::~CCSyntaxTree(void)
{
	Release();
}

void CCSyntaxTree::Release(void)
{
	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		delete m_vecAttrList[i];
	}

	m_vecAttrList.clear();

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		delete m_vecChildList[i];
	}

	m_vecChildList.clear();
}

void CCSyntaxTree::SetType(std::string szType)
{
	this->m_szType = szType;
}

std::string CCSyntaxTree::GetType(void)
{
	return m_szType;
}

void CCSyntaxTree::SetAttrValue(std::string szAttrName, std::string szAttrValue)
{
	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		if ( m_vecAttrList[i]->szAttrName == szAttrName )
		{
			m_vecAttrList[i]->szAttrValue = szAttrValue;

			return;
		}
	}

	CCSyntaxTreeAttr *pAttr = new CCSyntaxTreeAttr();
	pAttr->szAttrName = szAttrName;
	pAttr->szAttrValue = szAttrValue;

	m_vecAttrList.push_back(pAttr);
}

std::string CCSyntaxTree::GetAttrValue(std::string szAttrName)
{
	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		if ( m_vecAttrList[i]->szAttrName == szAttrName )
		{
			return m_vecAttrList[i]->szAttrValue;
		}
	}

	return "";
}

void CCSyntaxTree::AddChild(CCSyntaxTree *pChild)
{
	if(pChild)
	{
		m_vecChildList.push_back(pChild);
	}
}

void CCSyntaxTree::RemoveChild(CCSyntaxTree *pChild)
{
	int i;

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		if ( m_vecChildList[i] == pChild )
		{
			m_vecChildList.erase(m_vecChildList.begin() + i);
			return;
		}
	}
}

void CCSyntaxTree::InsertChild(CCSyntaxTree *pChild, int i)
{
	if ( pChild )
	{
		m_vecChildList.insert(m_vecChildList.begin() + i, pChild);
	}
}

int CCSyntaxTree::GetChildCount(void)
{
	return (int)m_vecChildList.size();
}

CCSyntaxTree *CCSyntaxTree::GetChild(int i)
{
	if ( i >= 0 && i < (int)m_vecChildList.size() )
	{
		return m_vecChildList[i];
	}
	else
	{
		return NULL;
	}
}

CCSyntaxTree *CCSyntaxTree::GetChildByTag(std::string szTag)
{
	int i;

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		if ( m_vecChildList[i]->GetAttrValue("Tag") == szTag )
		{
			return m_vecChildList[i];
		}
	}

	return NULL;
}

CCSyntaxTree *CCSyntaxTree::GetChildByName(std::string szName)
{
	int i;

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		if ( m_vecChildList[i]->GetAttrValue("Name") == szName )
		{
			return m_vecChildList[i];
		}
	}

	return NULL;
}

int CCSyntaxTree::GetLineNumber(void)
{
	return m_nLineNumber;
}

void CCSyntaxTree::Print(void)
{
	s_nIndent = 0;

	Print1();
}

void CCSyntaxTree::PrintIndent(void)
{
	int i;

	for ( i = 0; i < s_nIndent; i++ )
	{
		printf(" ");
	}
}

void CCSyntaxTree::Print1(void)
{
	PrintIndent();

	printf("<%s", m_szType.c_str());

	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		printf(" %s=\"%s\"", m_vecAttrList[i]->szAttrName.c_str(), m_vecAttrList[i]->szAttrValue.c_str());
	}

	if ( m_vecChildList.size() == 0 )
	{
		printf("/>\n");
	}
	else
	{
		printf(">\n");

		s_nIndent += 2;

		for ( i = 0; i < (int)m_vecChildList.size(); i++ )
		{
			m_vecChildList[i]->Print1();
		}

		s_nIndent -= 2;

		PrintIndent();

		printf("</%s>\n", m_szType.c_str());
	}
}

CCSyntaxTree *g_pRoot = NULL;

CCSyntaxTree *CCSyntaxTree_GetRoot(void)
{
	return g_pRoot;
}