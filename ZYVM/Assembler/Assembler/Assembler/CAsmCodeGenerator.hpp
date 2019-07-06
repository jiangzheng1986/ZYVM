//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "CAsmSyntaxTree.hpp"

#include "CAsmSymbolTable.hpp"

#include "CAsmInstrTable.hpp"

#include "Utility.hpp"

#include "CAsmSymbolFileGenerator.hpp"

#define ASM_CODE_NOP	0x00

enum enmAsmWordLength
{
	enmAsmWordLength_Byte,
	enmAsmWordLength_Word,
	enmAsmWordLength_Dword,
};

struct tagAsmOprandNodeStateBlock
{
public:
	tagAsmOprandNodeStateBlock(void);
	BOOL bIsLeftOprandNodeByte;
	BOOL bIsLeftOprandNodeWord;
	BOOL bIsLeftOprandNodeDword;
	BOOL bIsRightOprandNodeByte;
	BOOL bIsRightOprandNodeWord;
	BOOL bIsRightOprandNodeDword;
};

class CAsmCodeGenerator
{
public:
	CAsmCodeGenerator(void);
	~CAsmCodeGenerator(void);
	void OpenBinCodeFile(const WCHAR *pszFilename);
	void CloseBinCodeFile(void);
	void GenerateCode(CAsmSyntaxTree *pTree);
	bool GetError();
private:
	void GenerateCode_Recursive(CAsmSyntaxTree *pTree);
	void GenerateCode_Children(CAsmSyntaxTree *pTree);
	void GenerateCode_Other(CAsmSyntaxTree *pTree);
	void GenerateCode_StatementList(CAsmSyntaxTree *pTree);
	void GenerateCode_DataDefine(CAsmSyntaxTree *pTree);
	void GenerateCode_Label(CAsmSyntaxTree *pTree);
	void GenerateCode_EndProc(CAsmSyntaxTree *pTree);
	void GenerateCode_UngeneratedJumpStatements(const char *pszLabel);
	void GenerateCode_Statement(CAsmSyntaxTree *pTree);
	BOOL GenerateCode_Statement_NoneOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock);
	BOOL GenerateCode_Statement_SingleOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock);
	BOOL GenerateCode_Statement_BinaryOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock);
	void GenerateCode_Immediate(CAsmSyntaxTree *pTree, enmAsmWordLength eInstrWordLength);
	BOOL GenerateCode_Oprand(CAsmSyntaxTree *pParent, CAsmSyntaxTree *pTree, enmAsmWordLength eWordLength);
	void GenerateCode_Statement_Org(CAsmSyntaxTree *pTree);
	bool IsNumberNodeSigned(CAsmSyntaxTree *pTree);
	enmAsmWordLength GetNumberNodeWordLength(CAsmSyntaxTree *pTree, bool bIsNumberNodeSigned, DWORD dwValue);
	DWORD GetValueOfNumberNode(CAsmSyntaxTree *pTree);
	BOOL IsOprandNodeByte(CAsmSyntaxTree *pTree);
	BOOL IsOprandNodeWord(CAsmSyntaxTree *pTree);
	BOOL IsOprandNodeDword(CAsmSyntaxTree *pTree);
	BYTE GetCodeOfRegisterNode(CAsmSyntaxTree *pTree);
	BYTE GetCodeOfRegisterString(const char *pszReg1);
	BYTE GetCodeOfRegisterScale(const char *pszScale);
	BOOL GetAddressOfLabelRef(CAsmSyntaxTree *pTree, int *pnAddress);
	void DetermingLabelRefDistance(CAsmSyntaxTree *pTree);
	void AddNotFoundLebelRefJumpStatement(CAsmSyntaxTree *pTree);
	int GetBinCodeEmitPos(void);
	void SetBinCodeEmitPos(int nPos);
	void PrintHexData(void *pData, int nLength);
	void GetBinCodeAt(int nPos, void *pData, int nLength);
	void EmitBinCodeAt(int nPos, void *pData, int nLength);
	void EmitBinCode(void *pData, int nLength);
	void EmitBinCode_Byte(BYTE byData);
	void EmitBinCode_Word(WORD wData);
	void EmitBinCode_Dword(DWORD dwData);
	void EmitBinCode_Byte_NotPrint(BYTE byData);
	void EmitStatementStart(void);
	void EmitStatementEnd(void);
	void Error(const char *pszFormat, ...);
	FILE *m_pBinFile;
	DWORD m_dwOriginalAddress;
	BOOL m_bError;
	DWORD m_dwInstrStartAddress;
	BOOL m_bDisplaySyntaxTree;
	BOOL m_bFirstOrgStatement;
};

CAsmCodeGenerator *CAsmCodeGenerator_GetInstance(void);