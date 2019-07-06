//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

std::wstring ANSI_To_UTF16(const CHAR *str)
{
	WCHAR buf[10001];
	DWORD codePage = 936;
	DWORD minSize = MultiByteToWideChar(codePage, 0, str, -1, NULL, 0);
	MultiByteToWideChar(codePage, 0, str, -1, buf, minSize); 	
	return buf;
}

BOOL ReadAnsiFile(const WCHAR *filename,CHAR **charData)
{
	if(!filename||!charData)
	{
		return FALSE;
	}

	*charData=NULL;

	FILE *file = NULL;

	_wfopen_s(&file, filename, L"rb");

	if(file == NULL)
	{
		return FALSE;
	}

	fseek(file, 0, SEEK_END);

	int fileLength = ftell(file);

	fseek(file, 0, SEEK_SET);

	int charLength=fileLength;

	CHAR *charData1=new CHAR[charLength+1];

	fread(charData1, 1, charLength, file);

	charData1[charLength]=0x0000;

	fclose(file);

	if(charData)
	{
		*charData=charData1;
	}

	return TRUE;
}

CTextBuffer::CTextBuffer()
{

}

CTextBuffer::~CTextBuffer()
{
	ClearLines();
}

void CTextBuffer::LoadTextFile(const WCHAR *pszFilename)
{
	CHAR *pszCharData = NULL;

	BOOL b1 = ReadAnsiFile(pszFilename, &pszCharData);

	if ( !b1 )
	{
		return;
	}

	int j = 0;

	std::string *strLine = new std::string();

	int i;
	
	int n = strlen(pszCharData);

	for ( i = 0; i < n; i++ )
	{
		if ( pszCharData[i] == '\n' )
		{
			vecLineList.push_back(strLine);

			strLine = new std::string();

			j = 0;
		}
		else if ( pszCharData[i] == 0x0D )
		{

		}
		else if ( pszCharData[i] == '\t' )
		{
			int n1 = 4 - (j % 4);
			int i1;
			for ( i1 = 0; i1 < n1; i1++ )
			{
				(*strLine) += ' ';
				j ++;
			}
		}
		else
		{
			(*strLine) += pszCharData[i];
			j++;
		}
	}

	vecLineList.push_back(strLine);

	delete [] pszCharData;
}

int CTextBuffer::GetLineCount()
{
	return vecLineList.size();
}

const CHAR *CTextBuffer::GetLine(int i)
{
	return vecLineList[i]->c_str();
}

void CTextBuffer::ClearLines()
{
	int i;
	for ( i = 0; i < (int)vecLineList.size(); i++ )
	{
		delete vecLineList[i];
	}
	vecLineList.clear();
}