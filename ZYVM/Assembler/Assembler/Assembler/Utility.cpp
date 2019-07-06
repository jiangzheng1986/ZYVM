//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Utility.hpp"

BOOL GetValueOfNumberString_Asm(char *pszStr,int *pnValue)
{
	int nLen = strlen(pszStr);

	if ( pnValue )
	{
		*pnValue=0;
	}

	if ( nLen <= 0 )
	{
		return FALSE;
	}

	char chChar = pszStr[nLen - 1];

	int nRadix = 10;

	if ( chChar == 'h' || chChar == 'H' )
	{
		nRadix = 16;
		nLen--;
	}
	else if ( chChar == 'b' || chChar == 'B' )
	{
		nRadix = 2;
		nLen--;
	}

	if ( nLen <= 0 )
	{
		return FALSE;
	}

	int i;

	int nResult = 0;

	BOOL bNeg = FALSE;

	i = 0;

	if ( pszStr[0] == '-' )
	{
		bNeg = TRUE;
		i++;
	}

	for ( ; i < nLen; i++ )
	{
		char chChar = pszStr[i];
		int nDigit1 = 0;
		if ( chChar >= '0' && chChar <= '9' )
		{
			nDigit1 = chChar - '0';
		}
		else if ( chChar >= 'a' && chChar <= 'f' )
		{
			nDigit1 = chChar - 'a' + 10;
		}
		else if ( chChar >= 'A' && chChar <= 'F' )
		{
			nDigit1 = chChar - 'A' + 10;
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

	if ( pnValue )
	{
		*pnValue = nResult;
	}

	return TRUE;
}