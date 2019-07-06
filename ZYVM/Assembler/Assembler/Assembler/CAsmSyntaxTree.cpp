//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CAsmSyntaxTree.hpp"

int CAsmSyntaxTree::s_nIndent=0;

CAsmSyntaxTree::CAsmSyntaxTree(const char *pszType, int nLineNumber)
{
	this->m_szType = pszType;

	this->m_nLineNumber = nLineNumber;
}

CAsmSyntaxTree::~CAsmSyntaxTree(void)
{
	Release();
}

void CAsmSyntaxTree::Release(void)
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

std::string CAsmSyntaxTree::GetType(void)
{
	return m_szType;
}

void CAsmSyntaxTree::SetAttrValue(const char *pszAttrName, const char *pszAttrValue)
{
	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		if ( strcmp(m_vecAttrList[i]->m_szAttrName.c_str(), pszAttrName) == 0 )
		{
			m_vecAttrList[i]->m_szAttrValue = pszAttrValue;

			return;
		}
	}

	CAsmSyntaxTreeAttr *pAttr = new CAsmSyntaxTreeAttr();
	pAttr->m_szAttrName = pszAttrName;
	pAttr->m_szAttrValue = pszAttrValue;

	m_vecAttrList.push_back(pAttr);
}

std::string CAsmSyntaxTree::GetAttrValue(const char *pAttrName)
{
	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		if ( strcmp(m_vecAttrList[i]->m_szAttrName.c_str(), pAttrName) == 0 )
		{
			return m_vecAttrList[i]->m_szAttrValue;
		}
	}

	return "";
}

void CAsmSyntaxTree::AddChild(CAsmSyntaxTree *pChild)
{
	if ( pChild )
	{
		m_vecChildList.push_back(pChild);
	}
}

void CAsmSyntaxTree::InsertChild(CAsmSyntaxTree *pChild, int i)
{
	if ( pChild )
	{
		m_vecChildList.insert(m_vecChildList.begin() + i, pChild);
	}
}

int CAsmSyntaxTree::GetChildCount(void)
{
	return m_vecChildList.size();
}

CAsmSyntaxTree *CAsmSyntaxTree::GetChild(int i)
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

CAsmSyntaxTree *CAsmSyntaxTree::GetChildByTag(const char *pszTag)
{
	int i;

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		if ( strcmp(m_vecChildList[i]->GetAttrValue("Tag").c_str(), pszTag) == 0 )
		{
			return m_vecChildList[i];
		}
	}

	return NULL;
}

CAsmSyntaxTree *CAsmSyntaxTree::GetChildByName(const char *pszName)
{
	int i;

	for ( i = 0; i < (int)m_vecChildList.size(); i++ )
	{
		if ( strcmp(m_vecChildList[i]->GetAttrValue("Name").c_str(), pszName) == 0 )
		{
			return m_vecChildList[i];
		}
	}

	return NULL;
}

int CAsmSyntaxTree::GetLineNumber(void)
{
	return m_nLineNumber;
}

void CAsmSyntaxTree::Print(void)
{
	s_nIndent = 0;

	Print1();
}

void CAsmSyntaxTree::PrintIndent(void)
{
	int i;

	for ( i = 0; i < s_nIndent; i++ )
	{
		printf(" ");
	}
}

void CAsmSyntaxTree::Print1(void)
{
	PrintIndent();

	printf("<%s", m_szType.c_str());

	int i;

	for ( i = 0; i < (int)m_vecAttrList.size(); i++ )
	{
		printf(" %s=\"%s\"", m_vecAttrList[i]->m_szAttrName.c_str(), m_vecAttrList[i]->m_szAttrValue.c_str());
	}

	if ( m_vecChildList.size() == 0 )
	{
		printf("/>\n");
	}
	else
	{
		printf(">\n");

		s_nIndent += 2;

		for ( i= 0; i < (int)m_vecChildList.size(); i++ )
		{
			m_vecChildList[i]->Print1();
		}

		s_nIndent -= 2;

		PrintIndent();

		printf("</%s>\n", m_szType.c_str());
	}
}

CAsmSyntaxTree *g_pRoot = NULL;

CAsmSyntaxTree *CAsmSyntaxTree_GetRoot(void)
{
	return g_pRoot;
}