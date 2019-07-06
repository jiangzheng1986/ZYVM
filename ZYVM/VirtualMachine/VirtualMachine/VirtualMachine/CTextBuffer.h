//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

std::wstring ANSI_To_UTF16(const CHAR *str);

class CTextBuffer
{
public:
	CTextBuffer();
	~CTextBuffer();
	void LoadTextFile(const WCHAR *pszFilename);
	int GetLineCount();
	const CHAR *GetLine(int i);
	void ClearLines();
private:
	std::vector<std::string *> vecLineList;
};