//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Utility.hpp"

BOOL GetValueOfNumberString_C(char *pszStr, int *pValue)
{
	int i = 0;
	int nLen = (int)strlen(pszStr);

	if ( pValue )
	{
		*pValue = 0;
	}

	if ( nLen <= i )
	{
		return FALSE;
	}

	int nResult = 0;

	BOOL bNeg = FALSE;

	if ( pszStr[i] == '-' )
	{
		bNeg = TRUE;
		i++;
	}

	if ( nLen <= i )
	{
		return FALSE;
	}

	char ch1 = pszStr[i];
	char ch2 = pszStr[i + 1];

	int nRadix = 10;

	if ( ch1 == '0' )
	{
		if ( ch2 == '\x00' )
		{
			*pValue = 0;
			return TRUE;
		}
		else if ( ch2 == 'x' )
		{
			nRadix = 16;
			i++;
			i++;
		}
		else
		{
			nRadix = 8;
			i++;
		}
	}

	if ( nLen <= i )
	{
		return FALSE;
	}

	for ( ; i < nLen; i++ )
	{
		char ch = pszStr[i];
		int nDigit1 = 0;
		if ( ch >= '0' && ch <= '9' )
		{
			nDigit1 = ch - '0';
		}
		else if ( ch >= 'a' && ch <= 'f' )
		{
			nDigit1 = ch - 'a' + 10;
		}
		else if ( ch >= 'A' && ch <= 'F' )
		{
			nDigit1 = ch - 'A' + 10;
		}
		else
		{
			return FALSE;
		}
		if ( nDigit1 >= nRadix )
		{
			return FALSE;
		}
		nResult *= nRadix;
		nResult += nDigit1;
	}

	if ( bNeg )
	{
		nResult *= -1;
	}

	if ( pValue )
	{
		*pValue = nResult;
	}

	return TRUE;
}

char CharTokenStringToChar(char *pszStr)
{
	int nLen = (int)strlen(pszStr);

	if ( nLen == 1 )
	{
		return pszStr[0];
	}

	if ( nLen == 2 )
	{
		if ( pszStr[0] == '\\' )
		{
			if ( pszStr[1] == 'n' )
			{
				return '\n';
			}
		}
	}

	return 0;
}

std::string GetRegisterName(int registerNumber, const char *elementType)
{
	std::string elementType1 = elementType;

	if ( elementType1 == "int" || elementType1 == "unsigned int" )
	{
		switch(registerNumber)
		{
		case 0:
			return "eax";
		case 1:
			return "ebx";
		case 2:
			return "edx";
		}
	}
	else if ( elementType1 == "char" || elementType1 == "unsigned char" )
	{
		switch(registerNumber)
		{
		case 0:
			return "al";
		case 1:
			return "bl";
		case 2:
			return "ah";
		}
	}
	else
	{
		ASSERT(false);
	}
	return "ax";
}

std::string GetDataLengthStr(const char *elementType)
{
	std::string elementType1 = elementType;
	if ( elementType1 == "char" )
	{
		return "byte";
	}
	else if ( elementType1 == "bool" )
	{
		return "byte";
	}
	else if ( elementType1 == "int" )
	{
		return "dword";
	}
	else if ( elementType1 == "unsigned char" )
	{
		return "byte";
	}
	else if ( elementType1 == "unsigned bool" )
	{
		return "byte";
	}
	else if ( elementType1 == "unsigned int" )
	{
		return "dword";
	}
	else
	{
		return "dword";
	}
}

std::string GetDataDefineStr(const char *elementType)
{
	std::string elementType1 = elementType;
	if ( elementType1 == "char" )
	{
		return "db";
	}
	else if ( elementType1 == "bool" )
	{
		return "db";
	}
	else if ( elementType1 == "int" )
	{
		return "dd";
	}
	else if ( elementType1 == "unsigned char" )
	{
		return "db";
	}
	else if ( elementType1 == "unsigned bool" )
	{
		return "db";
	}
	else if ( elementType1 == "unsigned int" )
	{
		return "dd";
	}
	else
	{
		return "dw";
	}
}

bool IsElementTypeSigned(const char *elementType)
{
	std::string elementType1 = elementType;
	if ( elementType1 == "char" )
	{
		return true;
	}
	else if ( elementType1 == "bool" )
	{
		return true;
	}
	else if ( elementType1 == "int" )
	{
		return true;
	}
	else if ( elementType1 == "unsigned char" )
	{
		return false;
	}
	else if ( elementType1 == "unsigned bool" )
	{
		return false;
	}
	else if ( elementType1 == "unsigned int" )
	{
		return false;
	}
	else
	{
		return false;
	}
}

int GetLengthOfElementType(const char *elementType)
{
	std::string elementType1 = elementType;
	if(elementType1=="void")
	{
		return 0;
	}
	else if(elementType1=="char")
	{
		return 1;
	}
	else if(elementType1=="bool")
	{
		return 1;
	}
	else if(elementType1=="int")
	{
		return 4;
	}
	else if(elementType1=="unsigned char")
	{
		return 1;
	}
	else if(elementType1=="unsigned bool")
	{
		return 1;
	}
	else if(elementType1=="unsigned int")
	{
		return 4;
	}

	return 0;
}