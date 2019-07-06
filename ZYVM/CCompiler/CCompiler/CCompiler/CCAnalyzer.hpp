//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author��AndersJiang(����)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CCSyntaxTree.hpp"

#include "CCSymbolTable.hpp"

#include "Utility.hpp"

//�������Ҫ��:
//���Ƚϲ������������Ƿ���Ч
//��Ⲽ���������Ĳ������Ƿ���Ч
//��������������Ĳ������Ƿ���Ч
//��鸳ֵ���������Ƿ�ƥ��
//���if,while,for�����������Ƿ�ΪBOOLֵ
//���return���ķ���ֵ�Ƿ��뺯��ƥ��
//��麯�������еĲ����뺯�������Ƿ�ƥ��
//����Ƿ���main����,�����������ͷ���ֵ
//����ȫ�ֱ����ռ�
//�����������ջ�ռ�
//����ֲ�����ջ�ռ�
//������ʱ����ջ�ռ�
//����Ƿ�����·�����з���ֵ

class CCAnalyzer
{
public:
	CCAnalyzer(void);
	~CCAnalyzer(void);
	void Analyze(CCSyntaxTree *pTree);
private:
	void Analyze_Recursive(CCSyntaxTree *pTree);
	void Analyze_Children(CCSyntaxTree *pTree);
	void Analyze_Children_Reverse(CCSyntaxTree *pTree);
	void Analyze_FunDeclaration(CCSyntaxTree *pTree);
	void Analyze_ParamList(CCSyntaxTree *pTree);
	void Analyze_Param(CCSyntaxTree *pTree);
	void Analyze_VarDeclaration(CCSyntaxTree *pTree);
	void AllocateTempLocalVar(char *pszName, std::string szElementType);
	void Analyze_CompareExpression(CCSyntaxTree *pTree);
	void Analyze_ArithExpression(CCSyntaxTree *pTree);
	void Analyze_LogicExpression(CCSyntaxTree *pTree);
	void Analyze_UnaryExpression(CCSyntaxTree *pTree);
	void Analyze_PostfixExpression(CCSyntaxTree *pTree);
	void Analyze_AssignExpression(CCSyntaxTree *pTree);
	void Analyze_Conversion(CCSyntaxTree *pTree);
	void Analyze_Conversion_NoRecursive(CCSyntaxTree *pTree);
	void Analyze_Var(CCSyntaxTree *pTree);
	void Analyze_Number(CCSyntaxTree *pTree);
	void Analyze_Call(CCSyntaxTree *pTree);
	void Analyze_Other(CCSyntaxTree *pTree);
	std::string m_szCurrentScope;
};

CCAnalyzer *CCAnalyzer_GetInstance(void);