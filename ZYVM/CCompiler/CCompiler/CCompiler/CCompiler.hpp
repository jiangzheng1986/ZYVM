//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author��AndersJiang(����)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CCScanner.hpp"

#include "CCSyntaxTree.hpp"

#include "CCParser.hpp"

#include "CCSymbolTable.hpp"

#include "CCAnalyzer.hpp"

#include "CCCodeGenerator.hpp"

//��չ����:

//10.���ӽṹ
//9.��������
//12.����ָ��

//7.����for��������
//8.����switch-case
//7.break,continue,goto...
//8.����ѭ��break
//13.֧��/**/ע��

//2.����bool����
//2.�޸�unsigned�͵ıȽ�����
//3.�޸�signed�͵�*/%����
//8.����ö��
//11.��������
//13.Ƕ����

//5.?:�����
//6.+=,-=,*=,/=�����
//14.Ϊ�������ע��
//15.����Ԥ���봦��
//21.��ֵ�ж�
//1.��ʾת��
