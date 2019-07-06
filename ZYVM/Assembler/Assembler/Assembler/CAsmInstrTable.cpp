//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "CAsmInstrTable.hpp"

CAsmInstrTable g_AsmInstrTable;

CAsmInstrTable *CAsmInstrTable_GetInstance(void)
{
	return &g_AsmInstrTable;
}

CAsmInstrTable::CAsmInstrTable(void)
{

}

CAsmInstrTable::~CAsmInstrTable(void)
{

}

void CAsmInstrTable::AddInstrItem1(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
								   enmAsmOprandType eOprand1Type, enmAsmOprandAttr eOprand1Attr, enmAsmOprandType eOprand2Type, enmAsmOprandAttr eOprand2Attr, BYTE byCode, BYTE bySubCode)
{
	CAsmInstrItem AsmInstrItem;

	AsmInstrItem.m_eType = eType;
	AsmInstrItem.m_eAttr = eAttr;
	AsmInstrItem.m_eOprand1Type = eOprand1Type;
	AsmInstrItem.m_eOprand1Attr = eOprand1Attr;
	AsmInstrItem.m_eOprand2Type = eOprand2Type;
	AsmInstrItem.m_eOprand2Attr = eOprand2Attr;
	AsmInstrItem.m_byCode = byCode;
	AsmInstrItem.m_bySubCode = bySubCode;

	std::map<std::string,CAsmInstrItemList *>::iterator it;

	it = m_mapInstrTable.find(pszInstr);

	if ( it == m_mapInstrTable.end() )
	{
		m_mapInstrTable[pszInstr] = new CAsmInstrItemList();

		it = m_mapInstrTable.find(pszInstr);
	}

	CAsmInstrItemList *pInstrItemList = it->second;

	pInstrItemList->m_vecInstrItemList.push_back(AsmInstrItem);
}

void CAsmInstrTable::AddInstrItem2(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
								   enmAsmOprandType eOprand1Type, enmAsmOprandAttr eOprand1Attr, BYTE byCode, BYTE bySubCode)
{
	AddInstrItem1(pszInstr, eType, eAttr, eOprand1Type, eOprand1Attr, enmAsmOprandType_Nil, (enmAsmOprandAttr)0, byCode, bySubCode);
}

void CAsmInstrTable::AddInstrItem3(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
								   BYTE byCode, BYTE bySubCode)
{
	AddInstrItem1(pszInstr, eType, eAttr, enmAsmOprandType_Nil, (enmAsmOprandAttr)0, enmAsmOprandType_Nil, (enmAsmOprandAttr)0, byCode, bySubCode);
}

CAsmInstrItemList *CAsmInstrTable::FindInstrItemListOfInstr(const char *pszInstr)
{
	std::map<std::string,CAsmInstrItemList *>::iterator it;

	it = this->m_mapInstrTable.find(pszInstr);

	if ( it != m_mapInstrTable.end() )
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CAsmInstrTable::AddInstrItem_NoneOprand_General(const char *pszInstr, BYTE byCode)
{
	AddInstrItem3(pszInstr,enmAsmInstrType_NoneOprand,(enmAsmInstrAttr)0,byCode,0);
}

void CAsmInstrTable::AddInstrItem_SingleOprand_General(const char *pszInstr, BYTE byCode)
{
	AddInstrItem2(pszInstr,enmAsmInstrType_SingleOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Byte,byCode,0);

	AddInstrItem2(pszInstr,enmAsmInstrType_SingleOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Word,byCode|0x01,0);

	AddInstrItem2(pszInstr,enmAsmInstrType_SingleOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Dword,byCode|0x02,0);
}

void CAsmInstrTable::AddInstrItem_BinaryOprand_General(const char *pszInstr, BYTE byCode)
{
	AddInstrItem1(pszInstr,enmAsmInstrType_BinaryOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Byte,enmAsmOprandType_General,enmAsmOprandAttr_Byte,byCode,0);

	AddInstrItem1(pszInstr,enmAsmInstrType_BinaryOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Word,enmAsmOprandType_General,enmAsmOprandAttr_Word,byCode|0x01,0);

	AddInstrItem1(pszInstr,enmAsmInstrType_BinaryOprand,(enmAsmInstrAttr)0,
		enmAsmOprandType_General,enmAsmOprandAttr_Dword,enmAsmOprandType_General,enmAsmOprandAttr_Dword,byCode|0x02,0);
}

void CAsmInstrTable::BuildInstrTable(void)
{
	//无操作数指令

	AddInstrItem_NoneOprand_General("nop",0x00);

	AddInstrItem_NoneOprand_General("cbw",0x01);

	AddInstrItem_NoneOprand_General("cwd",0x02);
	
	AddInstrItem_NoneOprand_General("cli",0x03);

	AddInstrItem_NoneOprand_General("sti",0x04);

	AddInstrItem_NoneOprand_General("pushf",0x05);

	AddInstrItem_NoneOprand_General("popf",0x06);

	AddInstrItem_NoneOprand_General("pushfd",0x07);

	AddInstrItem_NoneOprand_General("popfd",0x08);

	AddInstrItem_NoneOprand_General("hlt",0x09);

	//单操作数指令

	AddInstrItem_SingleOprand_General("push", 0x10);

	AddInstrItem_SingleOprand_General("pop", 0x14);

	AddInstrItem_SingleOprand_General("inc", 0x18);

	AddInstrItem_SingleOprand_General("dec", 0x1C);

	AddInstrItem_SingleOprand_General("neg", 0x20);

	AddInstrItem_SingleOprand_General("not", 0x24);

	AddInstrItem_SingleOprand_General("mul", 0x28);

	AddInstrItem_SingleOprand_General("imul", 0x2C);

	AddInstrItem_SingleOprand_General("div", 0x30);

	AddInstrItem_SingleOprand_General("idiv", 0x34);

	//双操作数指令

	AddInstrItem_BinaryOprand_General("mov", 0x50);

	AddInstrItem_BinaryOprand_General("add", 0x54);

	AddInstrItem_BinaryOprand_General("sub", 0x58);

	AddInstrItem_BinaryOprand_General("and", 0x5C);

	AddInstrItem_BinaryOprand_General("or", 0x60);

	AddInstrItem_BinaryOprand_General("xor", 0x64);

	AddInstrItem_BinaryOprand_General("shl", 0x68);

	AddInstrItem_BinaryOprand_General("shr", 0x6C);

	AddInstrItem_BinaryOprand_General("sar", 0x70);

	AddInstrItem_BinaryOprand_General("xchg", 0x74);

	AddInstrItem_BinaryOprand_General("lea", 0x78);

	AddInstrItem_BinaryOprand_General("cmp", 0x7C);

	AddInstrItem_BinaryOprand_General("in", 0x80);

	AddInstrItem_BinaryOprand_General("out", 0x84);

	//转移指令

	AddInstrItem_SingleOprand_General("jmp", 0xA0);

	AddInstrItem_SingleOprand_General("je", 0xA4);

	AddInstrItem_SingleOprand_General("jne", 0xA8);

	AddInstrItem_SingleOprand_General("jb", 0xAC);

	AddInstrItem_SingleOprand_General("jnb", 0xB0);

	AddInstrItem_SingleOprand_General("ja", 0xB4);

	AddInstrItem_SingleOprand_General("jna", 0xB8);

	AddInstrItem_SingleOprand_General("jl", 0xBC);

	AddInstrItem_SingleOprand_General("jnl", 0xC0);

	AddInstrItem_SingleOprand_General("jg", 0xC4);

	AddInstrItem_SingleOprand_General("jng", 0xC8);

	AddInstrItem_SingleOprand_General("call", 0xCC);

	AddInstrItem_SingleOprand_General("int", 0xD0);

	AddInstrItem_NoneOprand_General("ret",0xD4);

	AddInstrItem_NoneOprand_General("iret",0xD5);
}