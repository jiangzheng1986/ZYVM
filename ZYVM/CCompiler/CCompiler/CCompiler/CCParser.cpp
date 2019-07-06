//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCParser.hpp"

CCParser::CCParser(void)
{
	m_eTokenType = enmCTokenType_Unknown;
	m_szTokenString[0] = '\0';
	m_nLineNumber = 0;
	m_bShowFirstErrorOnly = TRUE;
	m_bFirstErrorShowed = FALSE;
}

CCParser::~CCParser(void)
{

}

void CCParser::UpdateCurrentToken(void)
{
	m_eTokenType = CCScanner_GetInstance()->GetCurrentToken()->eTokenType;

	strcpy_s(m_szTokenString, 1000, CCScanner_GetInstance()->GetCurrentToken()->szTokenString.c_str());

	m_nLineNumber = 1;
}

void CCParser::NextToken(void)
{
	int nCurrentTokenIndex = CCScanner_GetInstance()->GetCurrentTokenIndex();

	CCScanner_GetInstance()->SetCurrentTokenIndex(nCurrentTokenIndex + 1);

	UpdateCurrentToken();
}

int CCParser::GetCurrentTokenIndex(void)
{
	return CCScanner_GetInstance()->GetCurrentTokenIndex();
}

void CCParser::SetCurrentTokenIndex(int nCurrentTokenIndex)
{
	CCScanner_GetInstance()->SetCurrentTokenIndex(nCurrentTokenIndex);

	UpdateCurrentToken();
}

void CCParser::MatchToken(enmCTokenType eExpectedTokenType)
{
	if ( m_eTokenType != eExpectedTokenType )
	{
		if ( m_bShowFirstErrorOnly )
		{
			if ( m_bFirstErrorShowed )
			{
				return;
			}
		}
		printf("Error(%d): Unexpected token type:", m_nLineNumber);
		CCScanner_GetInstance()->PrintTokenType(m_eTokenType, m_szTokenString);
		printf(",token string:");
		printf(m_szTokenString);
		printf(",expected token type:");
		CCScanner_GetInstance()->PrintTokenType(eExpectedTokenType, "");
		printf("\n");
		NextToken();
		m_bFirstErrorShowed = TRUE;

		ExitProcess(0);
	}
	else
	{
	}

	NextToken();
}

CCSyntaxTree *CCParser::Parse(void)
{
	CCScanner_GetInstance()->SetCurrentTokenIndex(0);
	UpdateCurrentToken();

	return ParseProgram();
}

CCSyntaxTree *CCParser::ParseProgram(void)
{
	return ParseDeclarationList();
}

CCSyntaxTree *CCParser::ParseDeclarationList(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("DeclarationList", m_nLineNumber);

	while ( m_eTokenType != enmCTokenType_Eof )
	{
		CCSyntaxTree *pChild = ParseDeclaration();

		pTree->AddChild(pChild);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseDeclaration(void)
{
	int nSavedTokenIndex=GetCurrentTokenIndex();

	CCSyntaxTree *pChild=ParseTypeSpecifier();
	delete pChild;

	MatchToken(enmCTokenType_Identifier);

	if ( m_eTokenType != enmCTokenType_LeftParen1 )
	{
		SetCurrentTokenIndex(nSavedTokenIndex);

		return ParseVarDeclaration();
	}
	else
	{
		SetCurrentTokenIndex(nSavedTokenIndex);

		return ParseFunDeclaration();
	}
}

CCSyntaxTree *CCParser::ParseVarDeclaration(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("VarDeclaration", m_nLineNumber);

	CCSyntaxTree *pChild = ParseTypeSpecifier();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "TypeNode");

		pTree->AddChild(pChild);
	}

	std::string szName = "";

	if ( m_eTokenType == enmCTokenType_Identifier )
	{
		szName = m_szTokenString;

		pTree->SetAttrValue("Name", m_szTokenString);
	}

	MatchToken(enmCTokenType_Identifier);

	BOOL bIsArray = FALSE;

	std::string szArraySize="0";

	if ( m_eTokenType == enmCTokenType_LeftParen2 )
	{
		MatchToken(enmCTokenType_LeftParen2);

		bIsArray = TRUE;

		if ( m_eTokenType == enmCTokenType_Number )
		{
			szArraySize = m_szTokenString;

			pTree->SetAttrValue("ArraySize", m_szTokenString);
		}

		MatchToken(enmCTokenType_Number);

		MatchToken(enmCTokenType_RightParen2);
	}

	if ( m_eTokenType == enmCTokenType_Assign )
	{
		MatchToken(enmCTokenType_Assign);

		CCSyntaxTree *pTree1 = new CCSyntaxTree("AssignExpression", m_nLineNumber);

		pChild = new CCSyntaxTree("Var", m_nLineNumber);

		if ( pChild )
		{
			pChild->SetAttrValue("Name", szName);

			if ( bIsArray )
			{
				pChild->SetAttrValue("ArraySize", szArraySize);
			}

			pChild->SetAttrValue("Tag", "Left");

			pTree1->AddChild(pChild);
		}

		pChild = ParseExpression();

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "Right");

			pTree1->AddChild(pChild);
		}

		pTree1->SetAttrValue("Tag", "Init");

		pTree->AddChild(pTree1);
	}

	MatchToken(enmCTokenType_SemiColon);

	return pTree;
}

CCSyntaxTree *CCParser::ParseTypeSpecifier(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("TypeSpecifier", m_nLineNumber);

	bool isUnsigned = false;

	if ( m_eTokenType == enmCTokenType_Unsigned ||
		 m_eTokenType == enmCTokenType_Signed )
	{
		if ( m_eTokenType == enmCTokenType_Unsigned )
		{
			isUnsigned = true;
		}

		MatchToken(m_eTokenType);
	}

	if( IsTokenTypeSpecifier(m_eTokenType) )
	{
		if ( isUnsigned )
		{
			std::string szElementType = "unsigned ";
			szElementType = szElementType + m_szTokenString;
			pTree->SetAttrValue("ElementType", szElementType.c_str());
		}
		else
		{
			pTree->SetAttrValue("ElementType",m_szTokenString);
		}

		MatchToken(m_eTokenType);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseFunDeclaration(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("FunDeclaration", m_nLineNumber);

	CCSyntaxTree *pChild = ParseTypeSpecifier();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "TypeNode");

		pTree->AddChild(pChild);
	}

	if ( m_eTokenType == enmCTokenType_Identifier )
	{
		pTree->SetAttrValue("Name", m_szTokenString);
	}

	MatchToken(enmCTokenType_Identifier);

	MatchToken(enmCTokenType_LeftParen1);

	pChild = ParseParams();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Params");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	pChild = new CCSyntaxTree("TempVars", m_nLineNumber);

	pChild->SetAttrValue("Tag", "TempVars");

	pTree->AddChild(pChild);

	pChild = ParseCompoundStmt();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Body");

		pTree->AddChild(pChild);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseParams(void)
{
	if ( m_eTokenType == enmCTokenType_RightParen1 )
	{
		return NULL;
	}
	else if ( m_eTokenType == enmCTokenType_Void )
	{
		MatchToken(enmCTokenType_Void);

		return NULL;
	}
	else
	{
		return ParseParamList();
	}
}

CCSyntaxTree *CCParser::ParseParamList(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("ParamList", m_nLineNumber);

	while ( m_eTokenType != enmCTokenType_RightParen1 )
	{
		CCSyntaxTree *pChild = ParseParam();

		pTree->AddChild(pChild);

		if ( m_eTokenType != enmCTokenType_RightParen1 )
		{
			MatchToken(enmCTokenType_Comma);
		}
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseParam(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("Param", m_nLineNumber);

	CCSyntaxTree *pChild = ParseTypeSpecifier();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "TypeNode");

		pTree->AddChild(pChild);
	}

	if ( m_eTokenType == enmCTokenType_Identifier )
	{
		pTree->SetAttrValue("Name", m_szTokenString);
	}

	MatchToken(enmCTokenType_Identifier);

	if ( m_eTokenType == enmCTokenType_LeftParen2 )
	{
		MatchToken(enmCTokenType_LeftParen2);

		pTree->SetAttrValue("IsArray", "true");

		MatchToken(enmCTokenType_RightParen2);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseCompoundStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("CompoundStmt", m_nLineNumber);

	MatchToken(enmCTokenType_LeftParen3);

	ParseLocalDeclarations(pTree);

	ParseStatementList(pTree);

	MatchToken(enmCTokenType_RightParen3);

	return pTree;
}

void CCParser::ParseLocalDeclarations(CCSyntaxTree *pTree)
{
	while( IsTokenTypeSpecifier(m_eTokenType) )
	{
		CCSyntaxTree *pChild = ParseVarDeclaration();

		pTree->AddChild(pChild);
	}
}

void CCParser::ParseStatementList(CCSyntaxTree *pTree)
{
	while ( m_eTokenType != enmCTokenType_RightParen3 )
	{
		CCSyntaxTree *pChild = ParseStatement();

		pTree->AddChild(pChild);
	}
}

CCSyntaxTree *CCParser::ParseStatement(void)
{
	if ( m_eTokenType == enmCTokenType_If )
	{
		return ParseIfStmt();
	}
	else if ( m_eTokenType == enmCTokenType_While )
	{
		return ParseWhileStmt();
	}
	else if ( m_eTokenType == enmCTokenType_Do )
	{
		return ParseDoWhileStmt();
	}
	else if ( m_eTokenType == enmCTokenType_For )
	{
		return ParseForStmt();
	}
	else if ( m_eTokenType == enmCTokenType_Break )
	{
		return ParseBreakStmt();
	}
	else if ( m_eTokenType == enmCTokenType_Continue )
	{
		return ParseContinueStmt();
	}
	else if ( m_eTokenType == enmCTokenType_Return )
	{
		return ParseReturnStmt();
	}
	else
	{
		return ParseExpressionStmt();
	}

	return NULL;
}

CCSyntaxTree *CCParser::ParseExpressionStmt(void)
{
	if ( m_eTokenType == enmCTokenType_SemiColon )
	{
		MatchToken(enmCTokenType_SemiColon);

		return NULL;
	}

	CCSyntaxTree *pTree = new CCSyntaxTree("ExpressionStmt", m_nLineNumber);

	CCSyntaxTree *pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Expression");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_SemiColon);

	return pTree;
}

CCSyntaxTree *CCParser::ParseExpression(void)
{
	int nSavedTokenIndex = GetCurrentTokenIndex();

	if ( m_eTokenType == enmCTokenType_Identifier )
	{
		CCSyntaxTree *pChild = ParseVar();

		if ( pChild )
		{
			if ( m_eTokenType == enmCTokenType_Assign )
			{
				MatchToken(enmCTokenType_Assign);

				CCSyntaxTree *pTree = new CCSyntaxTree("AssignExpression", m_nLineNumber);

				if ( pChild )
				{
					pChild->SetAttrValue("Tag", "Left");

					pTree->AddChild(pChild);
				}

				pChild = ParseExpression();

				if ( pChild )
				{
					pChild->SetAttrValue("Tag", "Right");

					pTree->AddChild(pChild);
				}

				return pTree;
			}

			delete pChild;
		}
	}

	SetCurrentTokenIndex(nSavedTokenIndex);

	return ParseLogic1Expression();
}

CCSyntaxTree *CCParser::ParseLogic1Expression(void)
{
	CCSyntaxTree *pLeft=ParseLogic2Expression();

	while ( m_eTokenType == enmCTokenType_Or )
	{
		MatchToken(enmCTokenType_Or);

		CCSyntaxTree *pRight = ParseLogic2Expression();

		if ( pRight )
		{
			CCSyntaxTree *pTree = new CCSyntaxTree("LogicExpression", m_nLineNumber);

			pTree->SetAttrValue("Operation", "||");

			if ( pLeft )
			{
				pLeft->SetAttrValue("Tag", "Left");

				pTree->AddChild(pLeft);
			}

			if ( pRight )
			{
				pRight->SetAttrValue("Tag", "Right");

				pTree->AddChild(pRight);
			}

			pLeft = pTree;
		}
	}

	return pLeft;
}

CCSyntaxTree *CCParser::ParseLogic2Expression(void)
{
	CCSyntaxTree *pLeft = ParseSimpleExpression();

	while ( m_eTokenType == enmCTokenType_And )
	{
		MatchToken(enmCTokenType_And);

		CCSyntaxTree *pRight = ParseSimpleExpression();

		if ( pRight )
		{
			CCSyntaxTree *pTree = new CCSyntaxTree("LogicExpression", m_nLineNumber);

			pTree->SetAttrValue("Operation", "&&");

			if ( pLeft )
			{
				pLeft->SetAttrValue("Tag", "Left");

				pTree->AddChild(pLeft);
			}

			if ( pRight )
			{
				pRight->SetAttrValue("Tag", "Right");

				pTree->AddChild(pRight);
			}

			pLeft = pTree;
		}
	}

	return pLeft;
}

CCSyntaxTree *CCParser::ParseSimpleExpression(void)
{
	CCSyntaxTree *pLeft = ParseAdditiveExpression();

	if ( m_eTokenType == enmCTokenType_LessEqualTo ||
		 m_eTokenType == enmCTokenType_LessTo ||
		 m_eTokenType == enmCTokenType_GreaterEqualTo ||
		 m_eTokenType == enmCTokenType_GreaterTo ||
		 m_eTokenType == enmCTokenType_Equal ||
		 m_eTokenType == enmCTokenType_Unequal )
	{
		std::string szOperation = m_szTokenString;

		ParseRelop();

		CCSyntaxTree *pRight = ParseAdditiveExpression();

		if ( pRight )
		{
			CCSyntaxTree *pTree = new CCSyntaxTree("CompareExpression", m_nLineNumber);

			pTree->SetAttrValue("Operation", szOperation);

			if ( pLeft )
			{
				pLeft->SetAttrValue("Tag", "Left");

				pTree->AddChild(pLeft);
			}

			if ( pRight )
			{
				pRight->SetAttrValue("Tag", "Right");

				pTree->AddChild(pRight);
			}

			pLeft = pTree;
		}
	}

	return pLeft;
}

void CCParser::ParseRelop(void)
{
	if ( m_eTokenType == enmCTokenType_LessEqualTo ||
		 m_eTokenType == enmCTokenType_LessTo ||
		 m_eTokenType == enmCTokenType_GreaterEqualTo ||
		 m_eTokenType == enmCTokenType_GreaterTo ||
		 m_eTokenType == enmCTokenType_Equal ||
		 m_eTokenType == enmCTokenType_Unequal )
	{
		MatchToken(m_eTokenType);
	}
}

CCSyntaxTree *CCParser::ParseAdditiveExpression(void)
{
	CCSyntaxTree *pLeft = ParseTerm();

	while ( m_eTokenType == enmCTokenType_Plus ||
			m_eTokenType == enmCTokenType_Minus )
	{
		std::string szOperation = m_szTokenString;

		ParseAddop();

		CCSyntaxTree *pRight = ParseTerm();

		if ( pRight )
		{
			CCSyntaxTree *pTree = new CCSyntaxTree("ArithExpression", m_nLineNumber);

			pTree->SetAttrValue("Operation", szOperation);

			if ( pLeft )
			{
				pLeft->SetAttrValue("Tag", "Left");

				pTree->AddChild(pLeft);
			}

			if ( pRight )
			{
				pRight->SetAttrValue("Tag", "Right");

				pTree->AddChild(pRight);
			}

			pLeft = pTree;
		}
	}

	return pLeft;
}

void CCParser::ParseAddop(void)
{
	if ( m_eTokenType == enmCTokenType_Plus ||
		 m_eTokenType == enmCTokenType_Minus )
	{
		MatchToken(m_eTokenType);
	}
}

CCSyntaxTree *CCParser::ParseTerm(void)
{
	CCSyntaxTree *pLeft=ParseUnaryExpression();

	while( m_eTokenType == enmCTokenType_Times ||
		   m_eTokenType == enmCTokenType_Divide ||
		   m_eTokenType == enmCTokenType_Mod )
	{
		std::string szOperation = m_szTokenString;

		ParseMulop();

		CCSyntaxTree *pRight = ParseUnaryExpression();

		if ( pRight )
		{
			CCSyntaxTree *pTree = new CCSyntaxTree("ArithExpression", m_nLineNumber);

			pTree->SetAttrValue("Operation", szOperation);

			if ( pLeft )
			{
				pLeft->SetAttrValue("Tag", "Left");

				pTree->AddChild(pLeft);
			}

			if ( pRight )
			{
				pRight->SetAttrValue("Tag", "Right");

				pTree->AddChild(pRight);
			}

			pLeft = pTree;
		}
	}

	return pLeft;
}

void CCParser::ParseMulop(void)
{
	if ( m_eTokenType == enmCTokenType_Times ||
		 m_eTokenType == enmCTokenType_Divide ||
		 m_eTokenType == enmCTokenType_Mod )
	{
		MatchToken(m_eTokenType);
	}
}

CCSyntaxTree *CCParser::ParseUnaryExpression(void)
{
	if ( m_eTokenType == enmCTokenType_Not || m_eTokenType == enmCTokenType_Minus ||
		 m_eTokenType == enmCTokenType_PlusPlus || m_eTokenType == enmCTokenType_MinusMinus )
	{
		std::string szTokenString1 = m_szTokenString;

		MatchToken(m_eTokenType);

		CCSyntaxTree *pChild=ParseUnaryExpression();

		CCSyntaxTree *pTree=NULL;
		
		if ( szTokenString1 == "!" )
		{
			pTree = new CCSyntaxTree("LogicExpression", m_nLineNumber);
		}
		else
		{
			pTree = new CCSyntaxTree("UnaryExpression", m_nLineNumber);
		}

		pTree->SetAttrValue("Operation", szTokenString1);

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "Exp");

			pTree->AddChild(pChild);
		}

		return pTree;
	}
	else
	{
		return ParsePostfixExpression();
	}
}

CCSyntaxTree *CCParser::ParsePostfixExpression(void)
{
	CCSyntaxTree *pChild=ParseFactor();

	if ( m_eTokenType == enmCTokenType_PlusPlus || m_eTokenType == enmCTokenType_MinusMinus )
	{
		std::string szTokenString1 = m_szTokenString;
		
		MatchToken(m_eTokenType);

		CCSyntaxTree *pTree = NULL;

		pTree = new CCSyntaxTree("PostfixExpression", m_nLineNumber);

		pTree->SetAttrValue("Operation", szTokenString1);

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "Exp");

			pTree->AddChild(pChild);
		}

		return pTree;
	}
	else
	{
		return pChild;
	}
}

CCSyntaxTree *CCParser::ParseFactor(void)
{
	CCSyntaxTree *pTree = NULL;

	if ( m_eTokenType == enmCTokenType_LeftParen1 )
	{
		BOOL bConversion = FALSE;

		MatchToken(enmCTokenType_LeftParen1);

		if ( IsTokenTypeSpecifier(m_eTokenType) )
		{
			bConversion = TRUE;
			pTree = ParseTypeSpecifier();
			pTree->SetType("Conversion");
		}
		else
		{
			pTree = ParseExpression();
		}

		MatchToken(enmCTokenType_RightParen1);

		if ( bConversion )
		{
			CCSyntaxTree *pChild = ParseExpression();

			if ( pChild )
			{
				pChild->SetAttrValue("Tag", "Exp");

				pTree->AddChild(pChild);
			}
		}
	}
	else if ( m_eTokenType == enmCTokenType_Number )
	{
		pTree = new CCSyntaxTree("Number", m_nLineNumber);

		pTree->SetAttrValue("Value", m_szTokenString);

		MatchToken(enmCTokenType_Number);
	}
	else if ( m_eTokenType == enmCTokenType_Character )
	{
		pTree = new CCSyntaxTree("Number", m_nLineNumber);

		char ch = CharTokenStringToChar(m_szTokenString);

		char szBuf[1000];
		sprintf_s(szBuf,"%d", ch);
		pTree->SetAttrValue("Value", szBuf);

		MatchToken(enmCTokenType_Character);
	}
	else if ( m_eTokenType == enmCTokenType_String )
	{
		pTree = new CCSyntaxTree("String", m_nLineNumber);

		pTree->SetAttrValue("Value", m_szTokenString);

		MatchToken(enmCTokenType_String);
	}
	else if ( m_eTokenType == enmCTokenType_Identifier )
	{
		int nSavedTokenIndex = GetCurrentTokenIndex();

		MatchToken(enmCTokenType_Identifier);

		if ( m_eTokenType != enmCTokenType_LeftParen1 )
		{
			SetCurrentTokenIndex(nSavedTokenIndex);

			pTree = ParseVar();
		}
		else
		{
			SetCurrentTokenIndex(nSavedTokenIndex);

			pTree = ParseCall();
		}
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseVar(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("Var", m_nLineNumber);

	pTree->SetAttrValue("Name", m_szTokenString);

	MatchToken(enmCTokenType_Identifier);

	if ( m_eTokenType == enmCTokenType_LeftParen2 )
	{
		MatchToken(enmCTokenType_LeftParen2);

		CCSyntaxTree *pChild = ParseExpression();

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "Subscript");

			pTree->AddChild(pChild);
		}

		MatchToken(enmCTokenType_RightParen2);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseCall(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("Call", m_nLineNumber);

	pTree->SetAttrValue("Name", m_szTokenString);

	MatchToken(enmCTokenType_Identifier);

	MatchToken(enmCTokenType_LeftParen1);

	CCSyntaxTree *pChild = ParseArgs();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Args");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	return pTree;
}

CCSyntaxTree *CCParser::ParseArgs(void)
{
	if ( m_eTokenType == enmCTokenType_RightParen1 )
	{
		return NULL;
	}

	return ParseArgList();
}

CCSyntaxTree *CCParser::ParseArgList(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("ArgList", m_nLineNumber);

	CCSyntaxTree *pChild = ParseExpression();

	if ( pChild )
	{
		pTree->AddChild(pChild);
	}

	while ( m_eTokenType == enmCTokenType_Comma )
	{
		MatchToken(enmCTokenType_Comma);

		pChild = ParseExpression();

		if ( pChild )
		{
			pTree->AddChild(pChild);
		}
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseIfStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("IfStmt", m_nLineNumber);

	MatchToken(enmCTokenType_If);

	MatchToken(enmCTokenType_LeftParen1);

	CCSyntaxTree *pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Condition");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	pChild = ParseCompoundStmt();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Body");

		pTree->AddChild(pChild);
	}

	if ( m_eTokenType == enmCTokenType_Else )
	{
		MatchToken(enmCTokenType_Else);

		pChild = ParseCompoundStmt();

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "Else");

			pTree->AddChild(pChild);
		}
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseWhileStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("WhileStmt", m_nLineNumber);

	MatchToken(enmCTokenType_While);

	MatchToken(enmCTokenType_LeftParen1);

	CCSyntaxTree *pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Condition");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	pChild = ParseCompoundStmt();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Body");

		pTree->AddChild(pChild);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseDoWhileStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("DoWhileStmt", m_nLineNumber);

	MatchToken(enmCTokenType_Do);

	CCSyntaxTree *pChild = ParseCompoundStmt();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Body");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_While);

	MatchToken(enmCTokenType_LeftParen1);

	pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Condition");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	return pTree;
}

CCSyntaxTree *CCParser::ParseForStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("WhileStmt", m_nLineNumber);

	MatchToken(enmCTokenType_For);

	MatchToken(enmCTokenType_LeftParen1);

	CCSyntaxTree *pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Init");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_SemiColon);

	pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Condition");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_SemiColon);

	pChild = ParseExpression();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Next");

		pTree->AddChild(pChild);
	}

	MatchToken(enmCTokenType_RightParen1);

	pChild = ParseCompoundStmt();

	if ( pChild )
	{
		pChild->SetAttrValue("Tag", "Body");

		pTree->AddChild(pChild);
	}

	return pTree;
}

CCSyntaxTree *CCParser::ParseBreakStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("BreakStmt", m_nLineNumber);

	MatchToken(enmCTokenType_Break);

	MatchToken(enmCTokenType_SemiColon);

	return pTree;
}

CCSyntaxTree *CCParser::ParseContinueStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("ContinueStmt", m_nLineNumber);

	MatchToken(enmCTokenType_Continue);

	MatchToken(enmCTokenType_SemiColon);

	return pTree;
}

CCSyntaxTree *CCParser::ParseReturnStmt(void)
{
	CCSyntaxTree *pTree = new CCSyntaxTree("ReturnStmt", m_nLineNumber);

	MatchToken(enmCTokenType_Return);

	if ( m_eTokenType != enmCTokenType_SemiColon )
	{
		CCSyntaxTree *pChild = ParseExpression();

		if ( pChild )
		{
			pChild->SetAttrValue("Tag", "ReturnValue");

			pTree->AddChild(pChild);
		}
	}

	MatchToken(enmCTokenType_SemiColon);

	return pTree;
}

BOOL CCParser::IsTokenTypeSpecifier(enmCTokenType eTokenType)
{
	if( eTokenType == enmCTokenType_Int||
		eTokenType == enmCTokenType_Void||
		eTokenType == enmCTokenType_Char||
		eTokenType == enmCTokenType_Unsigned||
		eTokenType == enmCTokenType_Signed )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

CCParser *CCParser_GetInstance(void)
{
	static CCParser s_CParser;

	return &s_CParser;
}
