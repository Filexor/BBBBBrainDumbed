#pragma once
#include <bitset>
#include <map>
#include <string>

using namespace std;

enum class InstructionType
{
	mnemonic,
	registername,
	directive,
	$operator,
	unknownnumber,
	knownnumber
};

enum class Associativity
{
	left_associative,
	right_associative,
};

class Instruction
{
public:
	bitset<7> opcode;
	InstructionType itype;
	int64_t value;
	Associativity atype;
	Instruction(uint8_t _opcode);
	Instruction(InstructionType _itype, int64_t _value);
	Instruction(InstructionType _itype, int64_t _value, Associativity _atype);
	~Instruction();

private:

};

Instruction::Instruction(uint8_t _opcode)
{
	opcode = _opcode;
	itype = InstructionType::mnemonic;
	value = 0;
	atype = Associativity::left_associative;
}

Instruction::Instruction(InstructionType _itype, int64_t _value)
{
	opcode = 0;
	itype = _itype;
	value = _value;
	atype = Associativity::left_associative;
}

Instruction::Instruction(InstructionType _itype, int64_t _value, Associativity _atype)
{
	opcode = 0;
	itype = _itype;
	value = _value;
	atype = _atype;
}

Instruction::~Instruction()
{
}

class Instructions
{
public:
	map<wstring, Instruction> insts;
	Instructions();
	~Instructions();

private:

};

Instructions::Instructions()
{
	insts.insert(make_pair(L"op1 A", Instruction(0)));
	insts.insert(make_pair(L"op1 B", Instruction(1)));
	insts.insert(make_pair(L"op1 D", Instruction(2)));
	insts.insert(make_pair(L"op1 E", Instruction(3)));
	insts.insert(make_pair(L"op1 F", Instruction(4)));
	insts.insert(make_pair(L"op1 G", Instruction(5)));
	insts.insert(make_pair(L"op1 K", Instruction(6)));
	insts.insert(make_pair(L"op1 P", Instruction(7)));
	insts.insert(make_pair(L"op2 A", Instruction(8)));
	insts.insert(make_pair(L"op2 B", Instruction(9)));
	insts.insert(make_pair(L"op2 D", Instruction(10)));
	insts.insert(make_pair(L"op2 E", Instruction(11)));
	insts.insert(make_pair(L"op2 F", Instruction(12)));
	insts.insert(make_pair(L"op2 G", Instruction(13)));
	insts.insert(make_pair(L"op2 K", Instruction(14)));
	insts.insert(make_pair(L"op2 P", Instruction(15)));
	insts.insert(make_pair(L"mov.1", Instruction(16)));
	insts.insert(make_pair(L"not.1", Instruction(17)));
	insts.insert(make_pair(L"or.1", Instruction(18)));
	insts.insert(make_pair(L"and.1", Instruction(19)));
	insts.insert(make_pair(L"xor.1", Instruction(20)));
	insts.insert(make_pair(L"shl.1", Instruction(21)));
	insts.insert(make_pair(L"shr.1", Instruction(22)));
	insts.insert(make_pair(L"asr.1", Instruction(23)));
	insts.insert(make_pair(L"rol.1", Instruction(24)));
	insts.insert(make_pair(L"ror.1", Instruction(25)));
	insts.insert(make_pair(L"adc.1", Instruction(26)));
	insts.insert(make_pair(L"sbb.1", Instruction(27)));
	insts.insert(make_pair(L"inc.4", Instruction(28)));
	insts.insert(make_pair(L"inc.16", Instruction(29)));
	insts.insert(make_pair(L"dec.4", Instruction(30)));
	insts.insert(make_pair(L"dec.16", Instruction(31)));
	insts.insert(make_pair(L"mov.4", Instruction(32)));
	insts.insert(make_pair(L"not.4", Instruction(33)));
	insts.insert(make_pair(L"or.4", Instruction(34)));
	insts.insert(make_pair(L"and.4", Instruction(35)));
	insts.insert(make_pair(L"xor.4", Instruction(36)));
	insts.insert(make_pair(L"shl.4", Instruction(37)));
	insts.insert(make_pair(L"shr.4", Instruction(38)));
	insts.insert(make_pair(L"asr.4", Instruction(39)));
	insts.insert(make_pair(L"rol.4", Instruction(40)));
	insts.insert(make_pair(L"ror.4", Instruction(41)));
	insts.insert(make_pair(L"adc.4", Instruction(42)));
	insts.insert(make_pair(L"sbb.4", Instruction(43)));
	insts.insert(make_pair(L"mul.4", Instruction(44)));
	insts.insert(make_pair(L"muls.4", Instruction(45)));
	insts.insert(make_pair(L"div.4", Instruction(46)));
	insts.insert(make_pair(L"divs.4", Instruction(47)));
	insts.insert(make_pair(L"mov.16", Instruction(48)));
	insts.insert(make_pair(L"not.16", Instruction(49)));
	insts.insert(make_pair(L"or.16", Instruction(50)));
	insts.insert(make_pair(L"and.16", Instruction(51)));
	insts.insert(make_pair(L"xor.16", Instruction(52)));
	insts.insert(make_pair(L"mfh", Instruction(53)));
	insts.insert(make_pair(L"mfl", Instruction(54)));
	insts.insert(make_pair(L"clr", Instruction(55)));
	insts.insert(make_pair(L"neg", Instruction(56)));
	insts.insert(make_pair(L"nop", Instruction(57)));
	insts.insert(make_pair(L"adc.16", Instruction(58)));
	insts.insert(make_pair(L"sbb.16", Instruction(59)));
	insts.insert(make_pair(L"mul.16", Instruction(60)));
	insts.insert(make_pair(L"muls.16", Instruction(61)));
	insts.insert(make_pair(L"div.16", Instruction(62)));
	insts.insert(make_pair(L"divs.16", Instruction(63)));
	insts.insert(make_pair(L"ldi.4 0", Instruction(64)));
	insts.insert(make_pair(L"ldi.4 1", Instruction(65)));
	insts.insert(make_pair(L"ldi.4 2", Instruction(66)));
	insts.insert(make_pair(L"ldi.4 3", Instruction(67)));
	insts.insert(make_pair(L"ldi.4 4", Instruction(68)));
	insts.insert(make_pair(L"ldi.4 5", Instruction(69)));
	insts.insert(make_pair(L"ldi.4 6", Instruction(70)));
	insts.insert(make_pair(L"ldi.4 7", Instruction(71)));
	insts.insert(make_pair(L"ldi.4 8", Instruction(72)));
	insts.insert(make_pair(L"ldi.4 9", Instruction(73)));
	insts.insert(make_pair(L"ldi.4 10", Instruction(74)));
	insts.insert(make_pair(L"ldi.4 11", Instruction(75)));
	insts.insert(make_pair(L"ldi.4 12", Instruction(76)));
	insts.insert(make_pair(L"ldi.4 13", Instruction(77)));
	insts.insert(make_pair(L"ldi.4 14", Instruction(78)));
	insts.insert(make_pair(L"ldi.4 15", Instruction(79)));
	insts.insert(make_pair(L"ldr.1", Instruction(80)));
	insts.insert(make_pair(L"ldri.1", Instruction(81)));
	insts.insert(make_pair(L"ldrd.1", Instruction(82)));
	insts.insert(make_pair(L"str.1", Instruction(83)));
	insts.insert(make_pair(L"stri.1", Instruction(84)));
	insts.insert(make_pair(L"strd.1", Instruction(85)));
	insts.insert(make_pair(L"cli", Instruction(86)));
	insts.insert(make_pair(L"inci", Instruction(87)));
	insts.insert(make_pair(L"add4i", Instruction(88)));
	insts.insert(make_pair(L"mfi", Instruction(89)));
	insts.insert(make_pair(L"mti", Instruction(90)));
	insts.insert(make_pair(L"clj", Instruction(91)));
	insts.insert(make_pair(L"incj", Instruction(92)));
	insts.insert(make_pair(L"add4j", Instruction(93)));
	insts.insert(make_pair(L"mfj", Instruction(94)));
	insts.insert(make_pair(L"mtj", Instruction(95)));
	insts.insert(make_pair(L"ldr.4", Instruction(96)));
	insts.insert(make_pair(L"ldri.4", Instruction(97)));
	insts.insert(make_pair(L"ldrd.4", Instruction(98)));
	insts.insert(make_pair(L"str.4", Instruction(99)));
	insts.insert(make_pair(L"stri.4", Instruction(100)));
	insts.insert(make_pair(L"strd.4", Instruction(101)));
	insts.insert(make_pair(L"clc", Instruction(102)));
	insts.insert(make_pair(L"sec", Instruction(103)));
	insts.insert(make_pair(L"mfc", Instruction(104)));
	insts.insert(make_pair(L"clm", Instruction(105)));
	insts.insert(make_pair(L"sem", Instruction(106)));
	insts.insert(make_pair(L"mfm", Instruction(107)));
	insts.insert(make_pair(L"mfv", Instruction(108)));
	insts.insert(make_pair(L"mtv", Instruction(109)));
	insts.insert(make_pair(L"flp", Instruction(110)));
	insts.insert(make_pair(L"swp", Instruction(111)));
	insts.insert(make_pair(L"ldr.16", Instruction(112)));
	insts.insert(make_pair(L"ldri.16", Instruction(113)));
	insts.insert(make_pair(L"ldrd.16", Instruction(114)));
	insts.insert(make_pair(L"str.16", Instruction(115)));
	insts.insert(make_pair(L"stri.16", Instruction(116)));
	insts.insert(make_pair(L"strd.16", Instruction(117)));
	insts.insert(make_pair(L"bcc", Instruction(118)));
	insts.insert(make_pair(L"bccr", Instruction(119)));
	insts.insert(make_pair(L"bz", Instruction(120)));
	insts.insert(make_pair(L"bzr", Instruction(121)));
	insts.insert(make_pair(L"bn", Instruction(122)));
	insts.insert(make_pair(L"bnr", Instruction(123)));
	//insts.insert(make_pair(L"", Instruction(124)));
	//insts.insert(make_pair(L"", Instruction(125)));
	insts.insert(make_pair(L"ldi.1 0", Instruction(126)));
	insts.insert(make_pair(L"ldi.1 1", Instruction(127)));
}

Instructions::~Instructions()
{
}