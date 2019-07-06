//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#define INITIAL_PARAM_ADDRESS		8

BOOL GetValueOfNumberString_C(char *pszStr, int *pValue);

char CharTokenStringToChar(char *pszStr);

std::string GetRegisterName(int nRegisterNumber, const char *pszElementType);

std::string GetDataLengthStr(const char *pszElementType);

std::string GetDataDefineStr(const char *pezElementType);

bool IsElementTypeSigned(const char *pszElementType);

int GetLengthOfElementType(const char *pszElementType);
