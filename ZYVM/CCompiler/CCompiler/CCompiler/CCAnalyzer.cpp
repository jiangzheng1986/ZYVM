//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "CCAnalyzer.hpp"

CCAnalyzer::CCAnalyzer(void)
{

}

CCAnalyzer::~CCAnalyzer(void)
{

}

void CCAnalyzer::Analyze(CCSyntaxTree *pTree)
{
	CCSymbolItem *pSymbolItem = new CCSymbolItem();
	pSymbolItem->m_szScope = "::";
	pSymbolItem->m_szName = "init";
	pSymbolItem->m_szType = "function";
	pSymbolItem->m_szElementType = "void";
	pSymbolItem->m_nMaxParamAddress = INITIAL_PARAM_ADDRESS;
	pSymbolItem->m_nMinLocalAddress = 0;
	pSymbolItem->m_pTree = NULL;
	CCSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem);

	m_szCurrentScope = "::";
	Analyze_Recursive(pTree);
}

void CCAnalyzer::Analyze_Recursive(CCSyntaxTree *pTree)
{
	std::string szType = pTree->GetType();

	if ( szType == "FunDeclaration" )
	{
		Analyze_FunDeclaration(pTree);
	}
	else if ( szType == "ParamList" )
	{
		Analyze_ParamList(pTree);
	}
	else if ( szType == "Param" )
	{
		Analyze_Param(pTree);
	}
	else if ( szType == "VarDeclaration" )
	{
		Analyze_VarDeclaration(pTree);
	}
	else if ( szType == "CompareExpression" )
	{
		Analyze_CompareExpression(pTree);
	}
	else if ( szType == "ArithExpression" )
	{
		Analyze_ArithExpression(pTree);
	}	
	else if ( szType == "LogicExpression" )
	{
		Analyze_LogicExpression(pTree);
	}
	else if ( szType == "UnaryExpression" )
	{
		Analyze_UnaryExpression(pTree);
	}
	else if ( szType == "PostfixExpression" )
	{
		Analyze_PostfixExpression(pTree);
	}
	else if ( szType == "AssignExpression" )
	{
		Analyze_AssignExpression(pTree);
	}
	else if ( szType == "Conversion" )
	{
		Analyze_Conversion(pTree);
	}
	else if ( szType == "Var" )
	{
		Analyze_Var(pTree);
	}
	else if ( szType == "Number" )
	{
		Analyze_Number(pTree);
	}	
	else if ( szType == "Call" )
	{
		Analyze_Call(pTree);
	}	
	else
	{
		Analyze_Other(pTree);
	}
}

void CCAnalyzer::Analyze_Children(CCSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		Analyze_Recursive(pTree->GetChild(i));
	}
}

void CCAnalyzer::Analyze_Children_Reverse(CCSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = nChildCount - 1; i >= 0; i-- )
	{
		Analyze_Recursive(pTree->GetChild(i));
	}
}

void CCAnalyzer::Analyze_FunDeclaration(CCSyntaxTree *pTree)
{
	m_szCurrentScope = "::";

	std::string szName = pTree->GetAttrValue("Name");

	CCSyntaxTree *pTypeNode = pTree->GetChildByTag("TypeNode");

	std::string szElementType = pTypeNode->GetAttrValue("ElementType");
	pTree->SetAttrValue("ElementType", szElementType);

	CCSymbolItem *pSymbolItem = new CCSymbolItem();
	pSymbolItem->m_szScope = m_szCurrentScope;
	pSymbolItem->m_szName = szName;
	pSymbolItem->m_szType = "function";
	pSymbolItem->m_szElementType = szElementType;
	pSymbolItem->m_nMaxParamAddress = INITIAL_PARAM_ADDRESS;
	pSymbolItem->m_nMinLocalAddress = 0;
	pSymbolItem->m_pTree = pTree;
	CCSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem);

	m_szCurrentScope = szName;

	Analyze_Children(pTree);

	m_szCurrentScope = "::";
}

void CCAnalyzer::Analyze_ParamList(CCSyntaxTree *pTree)
{
	Analyze_Children_Reverse(pTree);
}

void CCAnalyzer::Analyze_Param(CCSyntaxTree *pTree)
{
	std::string szName = pTree->GetAttrValue("Name");

	CCSyntaxTree *pTypeNode = pTree->GetChildByTag("TypeNode");

	CCSymbolItem *pFunSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem("::", m_szCurrentScope);

	std::string szElementType = pTypeNode->GetAttrValue("ElementType");
	pTree->SetAttrValue("ElementType", szElementType);

	CCSymbolItem *pSymbolItem = new CCSymbolItem();
	pSymbolItem->m_szScope = m_szCurrentScope;
	pSymbolItem->m_szName = szName;
	pSymbolItem->m_szType = "param   ";
	pSymbolItem->m_szElementType = szElementType;
	pSymbolItem->m_nAddress = pFunSymbolItem->m_nMaxParamAddress;
	pSymbolItem->m_pTree = pTree;
	CCSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem);

	int nLengthOfElementType = GetLengthOfElementType(szElementType.c_str());
	int nMinStackSize = 1;
	pFunSymbolItem->m_nMaxParamAddress += MAX(nMinStackSize, nLengthOfElementType);

	Analyze_Children(pTree);
}

void CCAnalyzer::Analyze_VarDeclaration(CCSyntaxTree *pTree)
{
	std::string szName = pTree->GetAttrValue("Name");

	CCSyntaxTree *pTypeNode = pTree->GetChildByTag("TypeNode");

	std::string szElementType = pTypeNode->GetAttrValue("ElementType");
	pTree->SetAttrValue("ElementType", szElementType);

	CCSymbolItem *pSymbolItem = new CCSymbolItem();
	pSymbolItem->m_szScope = m_szCurrentScope;
	pSymbolItem->m_szName = szName;
	if ( m_szCurrentScope == "::" )
	{
		pSymbolItem->m_szType = "global   ";
	}
	else
	{
		CCSymbolItem *pFunSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem("::", m_szCurrentScope);
		pSymbolItem->m_szType = "local    ";
		pFunSymbolItem->m_nMinLocalAddress -= GetLengthOfElementType(szElementType.c_str());
		pSymbolItem->m_nAddress = pFunSymbolItem->m_nMinLocalAddress;
	}
	pSymbolItem->m_szElementType = szElementType;
	pSymbolItem->m_pTree = pTree;
	CCSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem);

	Analyze_Children(pTree);
}

void CCAnalyzer::AllocateTempLocalVar(char *pszName, std::string szElementType)
{
	CCSymbolItem *pFunSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem("::", m_szCurrentScope);

	int nTempVarIndex = pFunSymbolItem->m_nCurrentTempVarIndex;

	sprintf_s(pszName, 1000, "t%d", nTempVarIndex);
	pFunSymbolItem->m_nCurrentTempVarIndex++;

	pFunSymbolItem->m_nMinLocalAddress -= GetLengthOfElementType(szElementType.c_str());

	int nLineNumber = 1;

	if ( pFunSymbolItem->m_pTree )
	{
		nLineNumber = pFunSymbolItem->m_pTree->GetLineNumber();
	}

	CCSyntaxTree *pTree = new CCSyntaxTree("TempVar", nLineNumber);
	pTree->SetAttrValue("Name", pszName);

	CCSyntaxTree *pChild = new CCSyntaxTree("TypeSpecifier", nLineNumber);
	pChild->SetAttrValue("ElementType", szElementType);
	pChild->SetAttrValue("Tag", "TypeNode");
	pTree->AddChild(pChild);

	CCSyntaxTree *pTempVarsTree = pFunSymbolItem->m_pTree->GetChildByTag("TempVars");
	pTempVarsTree->AddChild(pTree);

	CCSymbolItem *pSymbolItem = new CCSymbolItem();
	pSymbolItem->m_szScope = m_szCurrentScope;
	pSymbolItem->m_szName = pszName;
	pSymbolItem->m_szType = "temp    ";
	pSymbolItem->m_szElementType = szElementType; //暂时这样
	pSymbolItem->m_nAddress = pFunSymbolItem->m_nMinLocalAddress;
	pSymbolItem->m_pTree = pTree;
	CCSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem);
}

void CCAnalyzer::Analyze_CompareExpression(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	char szName[1000];
	std::string szElementType = "bool";
	AllocateTempLocalVar(szName, szElementType);

	pTree->SetAttrValue("TempVarName", szName);

	char szCode[1000];
	sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_ArithExpression(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CCSyntaxTree *pRight = pTree->GetChildByTag("Right");

	std::string szElementType_Left = pLeft->GetAttrValue("ElementType");

	std::string szElementType_Right = pRight->GetAttrValue("ElementType");

	std::string szElementType = "int";

	int nLengthOfElementType_Left = GetLengthOfElementType(szElementType_Left.c_str());

	int nLengthOfElementType_Right = GetLengthOfElementType(szElementType_Right.c_str());

	if ( szElementType_Left == szElementType_Right )
	{
		szElementType = szElementType_Left;
	}
	else if ( nLengthOfElementType_Left >= nLengthOfElementType_Right )
	{
		pTree->RemoveChild(pRight);

		CCSyntaxTree *pConvertNode = new CCSyntaxTree("Conversion",pRight->GetLineNumber());

		pConvertNode->SetAttrValue("ElementType",szElementType_Left);

		pConvertNode->SetAttrValue("Tag", "Right");

		pTree->AddChild(pConvertNode);

		pRight->SetAttrValue("Tag", "Exp");

		pConvertNode->AddChild(pRight);

		Analyze_Conversion(pConvertNode);
	}
	else if ( nLengthOfElementType_Left < nLengthOfElementType_Right )
	{
		pTree->RemoveChild(pLeft);

		CCSyntaxTree *pConvertNode = new CCSyntaxTree("Conversion",pLeft->GetLineNumber());

		pConvertNode->SetAttrValue("ElementType",szElementType_Right);

		pConvertNode->SetAttrValue("Tag", "Left");

		pTree->AddChild(pConvertNode);

		pRight->SetAttrValue("Tag", "Exp");

		pConvertNode->AddChild(pLeft);

		Analyze_Conversion(pConvertNode);
	}

	pTree->SetAttrValue("ElementType", szElementType);

	char szName[1000];
	AllocateTempLocalVar(szName, szElementType);

	pTree->SetAttrValue("TempVarName", szName);

	char szCode[1000];
	sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_LogicExpression(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	char szName[1000];
	std::string szElementType = "bool";
	AllocateTempLocalVar(szName, szElementType);

	pTree->SetAttrValue("TempVarName", szName);

	char szCode[1000];
	sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_UnaryExpression(CCSyntaxTree *pTree)
{ 
	Analyze_Children(pTree);

	CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");

	char szName[1000];
	std::string szElementType = pExp->GetAttrValue("ElementType");
	AllocateTempLocalVar(szName, szElementType);

	pTree->SetAttrValue("TempVarName", szName);

	char szCode[1000];
	sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_PostfixExpression(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");

	char szName[1000];
	std::string szElementType = pExp->GetAttrValue("ElementType");
	AllocateTempLocalVar(szName, szElementType);

	pTree->SetAttrValue("TempVarName", szName);

	char szCode[1000];
	sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_AssignExpression(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CCSyntaxTree *pRight = pTree->GetChildByTag("Right");

	std::string szElementType_Left = pLeft->GetAttrValue("ElementType");

	std::string szElementType_Right = pRight->GetAttrValue("ElementType");

	std::string szElementType = "int";

	int nLengthOfElementType_Left = GetLengthOfElementType(szElementType_Left.c_str());

	int nLengthOfElementType_Right = GetLengthOfElementType(szElementType_Right.c_str());

	szElementType = szElementType_Left;
	
	if ( nLengthOfElementType_Left >= nLengthOfElementType_Right )
	{
	}
	else if ( nLengthOfElementType_Left > nLengthOfElementType_Right )
	{
		pTree->RemoveChild(pRight);

		CCSyntaxTree *pConvertNode = new CCSyntaxTree("Conversion", pRight->GetLineNumber());

		pConvertNode->SetAttrValue("ElementType", szElementType);

		pConvertNode->SetAttrValue("Tag", "Right");

		pTree->AddChild(pConvertNode);

		pRight->SetAttrValue("Tag", "Exp");

		pConvertNode->AddChild(pRight);

		Analyze_Conversion(pConvertNode);
	}
	else
	{
		ASSERT(false);
	}

	if ( m_szCurrentScope != "::" )
	{
		pTree->SetAttrValue("ElementType", szElementType);

		char szName[1000];
		AllocateTempLocalVar(szName, szElementType);

		pTree->SetAttrValue("TempVarName", szName);

		char szCode[1000];
		sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

		pTree->SetAttrValue("Code", szCode);
	}
}

void CCAnalyzer::Analyze_Conversion(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	Analyze_Conversion_NoRecursive(pTree);
}

void CCAnalyzer::Analyze_Conversion_NoRecursive(CCSyntaxTree *pTree)
{
	std::string szElementType = pTree->GetAttrValue("ElementType");

	if ( m_szCurrentScope != "::" )
	{
		char szName[1000];
		AllocateTempLocalVar(szName, szElementType);

		pTree->SetAttrValue("TempVarName", szName);

		char szCode[1000];
		sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

		pTree->SetAttrValue("Code", szCode);
	}
	else
	{
		std::string szRegister1 = GetRegisterName(0, szElementType.c_str());

		pTree->SetAttrValue("Code", szRegister1);
	}
}

void CCAnalyzer::Analyze_Var(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	std::string szName = pTree->GetAttrValue("Name");

	CCSymbolItem *pSymbolItem=CCSymbolTable_GetInstance()->SearchSymbolItem(m_szCurrentScope,szName);

	char szCode[1000];

	if ( pSymbolItem == NULL )
	{
		szCode[0] = 0;
	}
	else
	{
		pTree->SetAttrValue("ElementType", pSymbolItem->m_szElementType.c_str());

		if ( pSymbolItem->m_szScope != "::" )
		{
			sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName.c_str());
		}
		else
		{
			sprintf_s(szCode,"_%s", szName.c_str());
		}
	}

	pTree->SetAttrValue("Code", szCode);
}

void CCAnalyzer::Analyze_Number(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	std::string szValue = pTree->GetAttrValue("Value");

	int i = 0;
	BOOL b1 = GetValueOfNumberString_C((char *)szValue.c_str(), &i);

	if ( i <= CHAR_MAX && i >= CHAR_MIN )
	{
		pTree->SetAttrValue("ElementType", "char");
	}
	else
	{
		pTree->SetAttrValue("ElementType", "int");
	}

	char szBuf[1000];
	sprintf_s(szBuf, 1000, "%d", i);
	pTree->SetAttrValue("Code", szBuf);
}

void CCAnalyzer::Analyze_Call(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);

	std::string szFuncName = pTree->GetAttrValue("Name");

	CCSymbolItem *pSymbolItem = CCSymbolTable_GetInstance()->SearchSymbolItem("::", szFuncName);

	ASSERT(pSymbolItem);

	pTree->SetAttrValue("ElementType", pSymbolItem->m_szElementType);

	if ( pSymbolItem->m_szElementType != "void" )
	{
		char szName[1000];
		std::string szElementType = pSymbolItem->m_szElementType;
		AllocateTempLocalVar(szName, szElementType);

		pTree->SetAttrValue("TempVarName", szName);

		char szCode[1000];
		sprintf_s(szCode, "_%s_%s", m_szCurrentScope.c_str(), szName);

		pTree->SetAttrValue("Code", szCode);
	}
}

void CCAnalyzer::Analyze_Other(CCSyntaxTree *pTree)
{
	Analyze_Children(pTree);
}

CCAnalyzer *CCAnalyzer_GetInstance(void)
{
	static CCAnalyzer s_CAnalyzer;

	return &s_CAnalyzer;
}