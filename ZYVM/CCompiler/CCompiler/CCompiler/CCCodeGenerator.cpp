//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "CCCodeGenerator.hpp"

CCCodeGenerator::CCCodeGenerator(void)
{
	m_nCurrentLabelIndex = 0;
	m_nInitVar = FALSE;
}

CCCodeGenerator::~CCCodeGenerator(void)
{

}

void CCCodeGenerator::OpenAsmCodeFile(WCHAR *pszFilename)
{
	m_pFile = NULL;
	_wfopen_s(&m_pFile, pszFilename, L"w");
}

void CCCodeGenerator::CloseAsmCodeFile(void)
{
	if ( m_pFile )
	{
		fclose(m_pFile);
	}
}

void CCCodeGenerator::EmitCode(const char *pszFormat, ...)
{
	va_list args;
	char szBuffer[1000];

	va_start(args, pszFormat);

	vsprintf_s(szBuffer, sizeof(szBuffer), pszFormat, args);

	printf("%s", szBuffer);

	if ( m_pFile )
	{
		fprintf(m_pFile, "%s", szBuffer);
	}

	va_end(args);
}

void CCCodeGenerator::GenerateCode(CCSyntaxTree *pTree)
{
	GenerateCode_Recursive(pTree);
}

void CCCodeGenerator::GenerateCode_Recursive(CCSyntaxTree *pTree)
{
	if ( pTree == NULL )
	{
		return;
	}

	std::string szType = pTree->GetType();

	if ( szType == "DeclarationList" )
	{
		GenerateCode_DeclarationList(pTree);
	}
	else if ( szType == "FunDeclaration" )
	{
		GenerateCode_FunDeclaration(pTree);
	}
	else if ( szType == "ParamList" )
	{
		GenerateCode_ParamList(pTree);
	}
	else if ( szType == "Param" )
	{
		GenerateCode_Param(pTree);
	}
	else if ( szType == "TempVars" )
	{
		GenerateCode_TempVars(pTree);
	}
	else if ( szType == "TempVar" )
	{
		GenerateCode_TempVar(pTree);
	}
	else if ( szType == "VarDeclaration" )
	{
		GenerateCode_VarDeclaration(pTree);
	}
	else if ( szType == "CompareExpression" )
	{
		GenerateCode_CompareExpression(pTree);
	}
	else if ( szType == "ArithExpression" )
	{
		GenerateCode_ArithExpression(pTree);
	}	
	else if ( szType == "LogicExpression" )
	{
		GenerateCode_LogicExpression(pTree);
	}	
	else if ( szType == "UnaryExpression" )
	{
		GenerateCode_UnaryExpression(pTree);
	}	
	else if ( szType == "PostfixExpression" )
	{
		GenerateCode_PostfixExpression(pTree);
	}	
	else if ( szType == "AssignExpression" )
	{
		GenerateCode_AssignExpression(pTree);
	}
	else if ( szType == "Conversion" )
	{
		GenerateCode_Conversion(pTree);
	}	
	else if ( szType == "IfStmt" )
	{
		GenerateCode_IfStmt(pTree);
	}	
	else if ( szType == "WhileStmt" )
	{
		GenerateCode_WhileStmt(pTree);
	}	
	else if ( szType == "DoWhileStmt" )
	{
		GenerateCode_DoWhileStmt(pTree);
	}	
	else if ( szType == "ReturnStmt" )
	{
		GenerateCode_ReturnStmt(pTree);
	}
	else if ( szType == "ArgList" )
	{
		GenerateCode_ArgList(pTree);
	}	
	else if ( szType == "Call" )
	{
		GenerateCode_Call(pTree);
	}
	else
	{
		GenerateCode_Other(pTree);
	}
}

void CCCodeGenerator::GenerateCode_Children(CCSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		GenerateCode_Recursive(pTree->GetChild(i));
	}
}

void CCCodeGenerator::GenerateCode_Children_Reverse(CCSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = nChildCount - 1; i >= 0; i-- )
	{
		GenerateCode_Recursive(pTree->GetChild(i));
	}
}

void CCCodeGenerator::GenerateCode_Children_Specific(CCSyntaxTree *pTree, std::string szType)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		CCSyntaxTree *pChild = pTree->GetChild(i);
		if ( pChild->GetType() == szType )
		{
			GenerateCode_Recursive(pChild);
		}
	}
}

void CCCodeGenerator::GenerateCode_Children_Except(CCSyntaxTree *pTree, std::string szType)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		CCSyntaxTree *pChild = pTree->GetChild(i);
		if ( pChild->GetType() != szType )
		{
			GenerateCode_Recursive(pChild);
		}
	}
}

void CCCodeGenerator::GenerateCode_Other(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);
}

void CCCodeGenerator::GenerateCode_DeclarationList(CCSyntaxTree *pTree)
{
	EmitCode("    org 00100000h\n");

	EmitCode("\n");

	EmitCode("    jmp start\n");

	EmitCode("\n");

	GenerateCode_Children_Specific(pTree, "VarDeclaration");

	GenerateCode_Children_Except(pTree, "VarDeclaration");

	EmitCode("init proc\n");

	EmitCode("\n");

	m_nInitVar = TRUE;

	GenerateCode_Children_Specific(pTree, "VarDeclaration");

	m_nInitVar = FALSE;

	EmitCode("    ret\n");

	EmitCode("\n");

	EmitCode("init endp\n");

	EmitCode("\n");

	EmitCode("start:\n");

	EmitCode("    mov esp,00200000h\n");		//暂时用2M结束作为栈

	EmitCode("\n");

	EmitCode("    call init\n");

	EmitCode("\n");

	EmitCode("    call _main\n");

	EmitCode("\n");

	EmitCode("    hlt\n");

	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_FunDeclaration(CCSyntaxTree *pTree)
{
	m_nCurrentLabelIndex = 0;

	std::string szName = pTree->GetAttrValue("Name");

	m_szCurrentScope = szName;

	int nLabelIndex_Return = m_nCurrentLabelIndex++;
	char szLabel_Return[100];
	sprintf_s(szLabel_Return, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex_Return);

	CCSymbolItem *pSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem("::", szName);

	EmitCode("_%s proc\n", szName.c_str());

	EmitCode("\n");

	EmitCode("    push ebp\n");

	EmitCode("    mov ebp,esp\n");

	int nMinLocalAddress = pSymbolItem->m_nMinLocalAddress;
	if ( nMinLocalAddress < 0 )
	{
		EmitCode("    sub esp,%d\n", -nMinLocalAddress);
	}

	EmitCode("\n");

	GenerateCode_Children(pTree);

	EmitCode("%s:\n", szLabel_Return);

	EmitCode("    mov esp,ebp\n");

	EmitCode("    pop ebp\n");

	EmitCode("    ret\n");

	EmitCode("\n");

	EmitCode("_%s endp\n", szName.c_str());

	EmitCode("\n");

	m_szCurrentScope = "";
}

void CCCodeGenerator::GenerateCode_ParamList(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);
}

void CCCodeGenerator::GenerateCode_Param(CCSyntaxTree *pTree)
{
	std::string szName = pTree->GetAttrValue("Name");

	CCSymbolItem *pSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem(m_szCurrentScope, szName);

	std::string szDataLengthStr = GetDataLengthStr(pSymbolItem->m_szElementType.c_str());

	EmitCode("_%s_%s equ %s ptr [ebp+%d]\n", m_szCurrentScope.c_str(), szName.c_str(), szDataLengthStr.c_str(), pSymbolItem->m_nAddress);
	EmitCode("\n");

	GenerateCode_Children(pTree);
}

void CCCodeGenerator::GenerateCode_TempVars(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);
}

void CCCodeGenerator::GenerateCode_TempVar(CCSyntaxTree *pTree)
{
	std::string szName = pTree->GetAttrValue("Name");

	CCSymbolItem *pSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem(m_szCurrentScope, szName);

	std::string szDataLengthStr = GetDataLengthStr(pSymbolItem->m_szElementType.c_str());

	EmitCode("_%s_%s equ %s ptr [ebp-%d]\n", m_szCurrentScope.c_str(), szName.c_str(), szDataLengthStr.c_str(), -pSymbolItem->m_nAddress);
	EmitCode("\n");

	GenerateCode_Children(pTree);
}

void CCCodeGenerator::GenerateCode_VarDeclaration(CCSyntaxTree *pTree)
{
	std::string szName = pTree->GetAttrValue("Name");

	CCSymbolItem *pSymbolItem = CCSymbolTable_GetInstance()->SearchSymbolItem(m_szCurrentScope, szName);

	if ( pSymbolItem->m_szScope == "::" )
	{
		if ( m_nInitVar == FALSE )
		{
			std::string szDataDefineStr = GetDataDefineStr(pSymbolItem->m_szElementType.c_str());

			EmitCode("_%s %s ?\n", szName.c_str(), szDataDefineStr.c_str());
			EmitCode("\n");
		}
		else
		{
			GenerateCode_Children(pTree);
		}
	}
	else
	{
		std::string szDataLengthStr = GetDataLengthStr(pSymbolItem->m_szElementType.c_str());

		EmitCode("_%s_%s equ %s ptr [ebp-%d]\n", m_szCurrentScope.c_str(), szName.c_str(), szDataLengthStr.c_str(), -pSymbolItem->m_nAddress);
		EmitCode("\n");

		GenerateCode_Children(pTree);
	}
}

void CCCodeGenerator::GenerateCode_CompareExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	int nLabelIndex1 = m_nCurrentLabelIndex++;
	char szLabel1[100];
	sprintf_s(szLabel1, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex1);

	int nLabelIndex2 = m_nCurrentLabelIndex++;
	char szLabel2[100];
	sprintf_s(szLabel2, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex2);	

	CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CCSyntaxTree *pRight = pTree->GetChildByTag("Right");

	std::string szElementType_Left = pLeft->GetAttrValue("ElementType");

	std::string szElementType_Right = pRight->GetAttrValue("ElementType");

	ASSERT(szElementType_Left == szElementType_Right);

	std::string szElementType_Child = szElementType_Left;

	std::string szRegister1 = GetRegisterName(0, szElementType_Child.c_str());

	std::string szRegister2 = GetRegisterName(1, szElementType_Child.c_str());

	EmitCode("    mov %s,%s\n", szRegister1.c_str(), pLeft->GetAttrValue("Code").c_str());
	EmitCode("    mov %s,%s\n", szRegister2.c_str(), pRight->GetAttrValue("Code").c_str());
	EmitCode("    cmp %s,%s\n", szRegister1.c_str(), szRegister2.c_str());

	std::string szOperation = pTree->GetAttrValue("Operation");
	if ( szOperation == ">" )
	{
		EmitCode("    jg %s\n", szLabel1);
	}
	else if ( szOperation == ">=" )
	{
		EmitCode("    jge %s\n", szLabel1);
	}
	else if ( szOperation == "<" )
	{
		EmitCode("    jl %s\n", szLabel1);
	}
	else if ( szOperation == "<=" )
	{
		EmitCode("    jle %s\n", szLabel1);
	}
	else if ( szOperation == "==" )
	{
		EmitCode("    je %s\n", szLabel1);
	}
	else if ( szOperation == "!=" )
	{
		EmitCode("    jne %s\n", szLabel1);
	}
	EmitCode("    mov al,0\n");
	EmitCode("    jmp short %s\n", szLabel2);
	EmitCode("%s:\n", szLabel1);
	EmitCode("    mov al,1\n");
	EmitCode("%s:\n", szLabel2);
	EmitCode("    mov %s,al\n", pTree->GetAttrValue("Code").c_str());
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_ArithExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CCSyntaxTree *pRight = pTree->GetChildByTag("Right");

	std::string szElementType_Left = pLeft->GetAttrValue("ElementType");

	std::string szElementType_Right = pRight->GetAttrValue("ElementType");

	std::string szElementType_Child = szElementType_Left;

	std::string szRegister1 = GetRegisterName(0, szElementType_Child.c_str());

	std::string szRegister2 = GetRegisterName(1, szElementType_Child.c_str());

	std::string szRegister3 = GetRegisterName(2, szElementType_Child.c_str());

	EmitCode("    mov %s,%s\n", szRegister1.c_str(), pLeft->GetAttrValue("Code").c_str());
	EmitCode("    mov %s,%s\n", szRegister2.c_str(), pRight->GetAttrValue("Code").c_str());

	std::string szOperation = pTree->GetAttrValue("Operation");
	if ( szOperation == "+" )
	{
		EmitCode("    add %s,%s\n", szRegister1.c_str(), szRegister2.c_str());
	}
	else if ( szOperation == "-" )
	{
		EmitCode("    sub %s,%s\n", szRegister1.c_str(), szRegister2.c_str());
	}
	else if ( szOperation == "*" )
	{
		EmitCode("    imul %s\n", szRegister2.c_str());
	}
	else if ( szOperation == "/" )
	{
		EmitCode("    mov %s,0\n", szRegister3.c_str());
		EmitCode("    idiv %s\n", szRegister2.c_str());
	}
	else if ( szOperation == "%" )
	{
		EmitCode("    mov %s,0\n", szRegister3.c_str());
		EmitCode("    idiv %s\n", szRegister2.c_str());
		EmitCode("    mov %s,%s\n", szRegister1.c_str(), szRegister3.c_str());
	}
	EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_LogicExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	std::string szOperation = pTree->GetAttrValue("Operation");

	if ( szOperation == "!" )
	{
		CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");
		EmitCode("    mov al,%s\n", pExp->GetAttrValue("Code").c_str());
		EmitCode("    xor al,1\n");
	}
	else
	{
		CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");
		CCSyntaxTree *pRight = pTree->GetChildByTag("Right");
		EmitCode("    mov al,%s\n", pLeft->GetAttrValue("Code").c_str());
		EmitCode("    mov bl,%s\n", pRight->GetAttrValue("Code").c_str());

		if ( szOperation == "&&" )
		{
			EmitCode("    and al,bl\n");
		}
		else if ( szOperation == "||" )
		{
			EmitCode("    or al,bl\n");
		}
	}

	EmitCode("    mov %s,al\n", pTree->GetAttrValue("Code").c_str());
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_UnaryExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	std::string szOperation = pTree->GetAttrValue("Operation");

	CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");

	std::string szElementType = pExp->GetAttrValue("ElementType");

	std::string szRegister1 = GetRegisterName(0, szElementType.c_str());

	EmitCode("    mov %s,%s\n", szRegister1.c_str(), pExp->GetAttrValue("Code").c_str());

	if ( szOperation == "-" )
	{
		EmitCode("    neg %s\n", szRegister1.c_str());
	}
	else if ( szOperation == "++" )
	{
		EmitCode("    inc %s\n", szRegister1.c_str());
	}
	else if ( szOperation == "--" )
	{
		EmitCode("    dec %s\n", szRegister1.c_str());
	}
	else
	{
		ASSERT(false);
	}

	EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_PostfixExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	std::string szOperation = pTree->GetAttrValue("Operation");

	CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");

	std::string szElementType = pExp->GetAttrValue("ElementType");

	std::string szRegister1 = GetRegisterName(0, szElementType.c_str());

	EmitCode("    mov %s,%s\n", szRegister1.c_str(), pExp->GetAttrValue("Code").c_str());

	EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());

	if ( szOperation == "-" )
	{
		EmitCode("    neg %s\n", szRegister1.c_str());
	}
	else if ( szOperation == "++" )
	{
		EmitCode("    inc %s\n", szRegister1.c_str());
	}
	else if ( szOperation == "--" )
	{
		EmitCode("    dec %s\n", szRegister1.c_str());
	}
	else
	{
		ASSERT(false);
	}

	EmitCode("    mov %s,%s\n", pExp->GetAttrValue("Code").c_str(), szRegister1.c_str());

	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_AssignExpression(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	CCSyntaxTree *pRight = pTree->GetChildByTag("Right");
	CCSyntaxTree *pLeft = pTree->GetChildByTag("Left");

	std::string szElementType_Left=pLeft->GetAttrValue("ElementType");

	std::string szElementType_Right=pRight->GetAttrValue("ElementType");

	std::string szElementType_Child = szElementType_Left;

	std::string szRegister1 = GetRegisterName(0, szElementType_Child.c_str());

	std::string szRightCode = pRight->GetAttrValue("Code");

	std::string szCode = pTree->GetAttrValue("Code");

	if ( szCode != "" )
	{
		EmitCode("    mov %s,%s\n", szRegister1.c_str(), pRight->GetAttrValue("Code").c_str());
		EmitCode("    mov %s,%s\n", pLeft->GetAttrValue("Code").c_str(), szRegister1.c_str());
		EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());
		EmitCode("\n");
	}
	else
	{
		EmitCode("    mov %s,%s\n", szRegister1.c_str(), pRight->GetAttrValue("Code").c_str());
		EmitCode("    mov %s,%s\n", pLeft->GetAttrValue("Code").c_str(), szRegister1.c_str());
		EmitCode("\n");
	}
}

void CCCodeGenerator::GenerateCode_Conversion(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	CCSyntaxTree *pExp = pTree->GetChildByTag("Exp");

	std::string szElementType = pTree->GetAttrValue("ElementType");

	std::string szElementType_Exp = pExp->GetAttrValue("ElementType");

	std::string szRegister1_Exp = GetRegisterName(0, szElementType_Exp.c_str());

	std::string szRegister1 = GetRegisterName(0, szElementType.c_str());

	EmitCode("    mov %s,%s\n", szRegister1_Exp.c_str(), pExp->GetAttrValue("Code").c_str());
	
	if ( szElementType == "int" && szElementType_Exp == "char" )
	{
		EmitCode("    cbw\n");
	}
	else if ( szElementType == "unsigned int" && szElementType_Exp == "unsigned char" )
	{
		EmitCode("    xor ah, ah\n");
	}
	else if ( szElementType == "char" && szElementType_Exp == "int" )
	{
		EmitCode("    xor ah, ah\n");
	}
	else if ( szElementType == "unsigned char" && szElementType_Exp == "unsigned int" )
	{
		EmitCode("    xor ah, ah\n");
	}

	EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_IfStmt(CCSyntaxTree *pTree)
{
	CCSyntaxTree *pCondition = pTree->GetChildByTag("Condition");

	CCSyntaxTree *pBody = pTree->GetChildByTag("Body");

	CCSyntaxTree *pElse = pTree->GetChildByTag("Else");

	GenerateCode_Recursive(pCondition);

	int nLabelIndex1 = m_nCurrentLabelIndex++;
	char szLabel1[100];
	sprintf_s(szLabel1, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex1);

	int nLabelIndex2 = m_nCurrentLabelIndex++;
	char szLabel2[100];
	sprintf_s(szLabel2, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex2);	

	int nLabelIndex3 = m_nCurrentLabelIndex++;
	char szLabel3[100];
	sprintf_s(szLabel3, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex3);	

	EmitCode("    mov al,%s\n", pCondition->GetAttrValue("Code").c_str());
	EmitCode("    cmp al,0\n");
	EmitCode("    jne %s\n", szLabel1);
	EmitCode("    jmp %s\n", szLabel2);
	EmitCode("%s:\n", szLabel1);

	GenerateCode_Recursive(pBody);

	if ( pElse )
	{
		EmitCode("    jmp %s\n", szLabel3);
	}
	EmitCode("%s:\n", szLabel2);

	GenerateCode_Recursive(pElse);

	if ( pElse )
	{
		EmitCode("%s:\n", szLabel3);
	}

	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_WhileStmt(CCSyntaxTree *pTree)
{
	CCSyntaxTree *pCondition = pTree->GetChildByTag("Condition");

	CCSyntaxTree *pBody = pTree->GetChildByTag("Body");

	int nLabelIndex1 = m_nCurrentLabelIndex++;
	char szLabel1[100];
	sprintf_s(szLabel1, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex1);

	int nLabelIndex2 = m_nCurrentLabelIndex++;
	char szLabel2[100];
	sprintf_s(szLabel2, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex2);	

	int nLabelIndex3 = m_nCurrentLabelIndex++;
	char szLabel3[100];
	sprintf_s(szLabel3, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex3);	

	EmitCode("%s:\n", szLabel1);
	EmitCode("\n");

	GenerateCode_Recursive(pCondition);

	EmitCode("    mov al,%s\n", pCondition->GetAttrValue("Code").c_str());
	EmitCode("    cmp al,0\n");
	EmitCode("    jne %s\n", szLabel2);
	EmitCode("    jmp %s\n", szLabel3);
	EmitCode("%s:\n", szLabel2);
	EmitCode("\n");

	GenerateCode_Recursive(pBody);

	EmitCode("    jmp %s\n", szLabel1);
	EmitCode("%s:\n", szLabel3);
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_DoWhileStmt(CCSyntaxTree *pTree)
{
	CCSyntaxTree *pCondition = pTree->GetChildByTag("Condition");

	CCSyntaxTree *pBody = pTree->GetChildByTag("Body");

	int nLabelIndex1 = m_nCurrentLabelIndex++;
	char szLabel1[100];
	sprintf_s(szLabel1, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex1);

	int nLabelIndex2=m_nCurrentLabelIndex++;
	char szLabel2[100];
	sprintf_s(szLabel2, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex2);

	EmitCode("%s:\n", szLabel1);
	EmitCode("\n");

	GenerateCode_Recursive(pBody);

	GenerateCode_Recursive(pCondition);

	EmitCode("    mov al,%s\n", pCondition->GetAttrValue("Code").c_str());
	EmitCode("    cmp al,0\n");
	EmitCode("    je %s\n", szLabel2);
	EmitCode("    jmp %s\n", szLabel1);
	EmitCode("\n");
	EmitCode("%s:\n", szLabel2);
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_ReturnStmt(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	CCSyntaxTree *pChild = pTree->GetChildByTag("ReturnValue");

	std::string szElementType_Child = pChild->GetAttrValue("ElementType");

	std::string szRegister1 = GetRegisterName(0, szElementType_Child.c_str());

	int nLabelIndex_Return=0;
	char szLabel_Return[100];
	sprintf_s(szLabel_Return, "_%s_%d", m_szCurrentScope.c_str(), nLabelIndex_Return);

	EmitCode("    mov %s,%s\n", szRegister1.c_str(), pChild->GetAttrValue("Code").c_str());
	EmitCode("    jmp %s\n", szLabel_Return);
	EmitCode("\n");
}

void CCCodeGenerator::GenerateCode_ArgList(CCSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		CCSyntaxTree *pChild = pTree->GetChild(i);

		GenerateCode_Recursive(pChild);

		std::string szElementType_Child = pChild->GetAttrValue("ElementType");

		std::string szRegister1 = GetRegisterName(0, szElementType_Child.c_str());

		EmitCode("    mov %s,%s\n", szRegister1.c_str(), pChild->GetAttrValue("Code").c_str());
		EmitCode("    push %s\n", szRegister1.c_str());
		EmitCode("\n");
	}
}

void CCCodeGenerator::GenerateCode_Call(CCSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);

	std::string szName = pTree->GetAttrValue("Name");

	EmitCode("    call _%s\n", szName.c_str());

	CCSymbolItem *pFunSymbolItem = CCSymbolTable_GetInstance()->GetSymbolItem("::", szName);

	int nSubSP = pFunSymbolItem->m_nMaxParamAddress - INITIAL_PARAM_ADDRESS;

	if ( nSubSP > 0 )
	{
		EmitCode("    add esp,%d\n", nSubSP);
	}

	std::string szElementType = pTree->GetAttrValue("ElementType");

	if ( szElementType != "void" )
	{
		std::string szRegister1 = GetRegisterName(0, szElementType.c_str());

		EmitCode("    mov %s,%s\n", pTree->GetAttrValue("Code").c_str(), szRegister1.c_str());
	}

	EmitCode("\n");
}

CCCodeGenerator *CCCodeGenerator_GetInstance(void)
{
	static CCCodeGenerator s_CCodeGenerator;

	return &s_CCodeGenerator;
}