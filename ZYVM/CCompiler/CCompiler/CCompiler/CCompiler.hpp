//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CCScanner.hpp"

#include "CCSyntaxTree.hpp"

#include "CCParser.hpp"

#include "CCSymbolTable.hpp"

#include "CCAnalyzer.hpp"

#include "CCCodeGenerator.hpp"

//扩展方向:

//10.增加结构
//9.增加数组
//12.增加指针

//7.生成for语句机器码
//8.增加switch-case
//7.break,continue,goto...
//8.多重循环break
//13.支持/**/注释

//2.增加bool类型
//2.修改unsigned型的比较运算
//3.修改signed型的*/%运算
//8.增加枚举
//11.增加联合
//13.嵌入汇编

//5.?:运算符
//6.+=,-=,*=,/=运算符
//14.为程序添加注释
//15.增加预编译处理
//21.左值判定
//1.显示转换
