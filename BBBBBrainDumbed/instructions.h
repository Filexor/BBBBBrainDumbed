#pragma once
#include <bitset>
#include <map>
#include <string>

using namespace std;

enum class InstructionType
{
	mnemonic,
	mnemonic_expect_number,
	mnemonic_expect_registername,
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
	Instruction(uint8_t _opcode, InstructionType _itype);
	Instruction(InstructionType _itype);
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

Instruction::Instruction(uint8_t _opcode, InstructionType _itype)
{
	opcode = _opcode;
	itype = _itype;
	value = 0;
	atype = Associativity::left_associative;
}

Instruction::Instruction(InstructionType _itype)
{
	opcode = 0;
	itype = _itype;
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
	map<wstring, Instruction> inst;
	Instructions();
	~Instructions();

private:

};

Instructions::Instructions()
{
	inst.insert(make_pair(L"op1 A", Instruction(0)));
	inst.insert(make_pair(L"op1 B", Instruction(1)));
	inst.insert(make_pair(L"op1 D", Instruction(2)));
	inst.insert(make_pair(L"op1 E", Instruction(3)));
	inst.insert(make_pair(L"op1 F", Instruction(4)));
	inst.insert(make_pair(L"op1 G", Instruction(5)));
	inst.insert(make_pair(L"op1 K", Instruction(6)));
	inst.insert(make_pair(L"op1 P", Instruction(7)));
	inst.insert(make_pair(L"op2 A", Instruction(8)));
	inst.insert(make_pair(L"op2 B", Instruction(9)));
	inst.insert(make_pair(L"op2 D", Instruction(10)));
	inst.insert(make_pair(L"op2 E", Instruction(11)));
	inst.insert(make_pair(L"op2 F", Instruction(12)));
	inst.insert(make_pair(L"op2 G", Instruction(13)));
	inst.insert(make_pair(L"op2 K", Instruction(14)));
	inst.insert(make_pair(L"op2 P", Instruction(15)));
	inst.insert(make_pair(L"mov.1", Instruction(16)));
	inst.insert(make_pair(L"not.1", Instruction(17)));
	inst.insert(make_pair(L"or.1", Instruction(18)));
	inst.insert(make_pair(L"and.1", Instruction(19)));
	inst.insert(make_pair(L"xor.1", Instruction(20)));
	inst.insert(make_pair(L"shl", Instruction(21)));
	inst.insert(make_pair(L"shr", Instruction(22)));
	inst.insert(make_pair(L"asr", Instruction(23)));
	inst.insert(make_pair(L"rol", Instruction(24)));
	inst.insert(make_pair(L"ror", Instruction(25)));
	inst.insert(make_pair(L"adc.1", Instruction(26)));
	inst.insert(make_pair(L"sbb.1", Instruction(27)));
	inst.insert(make_pair(L"inc.4", Instruction(28)));
	inst.insert(make_pair(L"inc.16", Instruction(29)));
	inst.insert(make_pair(L"dec.4", Instruction(30)));
	inst.insert(make_pair(L"dec.16", Instruction(31)));
	inst.insert(make_pair(L"mov.4", Instruction(32)));
	inst.insert(make_pair(L"not.4", Instruction(33)));
	inst.insert(make_pair(L"or.4", Instruction(34)));
	inst.insert(make_pair(L"and.4", Instruction(35)));
	inst.insert(make_pair(L"xor.4", Instruction(36)));
	//inst.insert(make_pair(L"", Instruction(37)));
	//inst.insert(make_pair(L"", Instruction(38)));
	//inst.insert(make_pair(L"", Instruction(39)));
	//inst.insert(make_pair(L"", Instruction(40)));
	//inst.insert(make_pair(L"", Instruction(41)));
	inst.insert(make_pair(L"adc.4", Instruction(42)));
	inst.insert(make_pair(L"sbb.4", Instruction(43)));
	inst.insert(make_pair(L"mul.4", Instruction(44)));
	inst.insert(make_pair(L"muls.4", Instruction(45)));
	inst.insert(make_pair(L"div.4", Instruction(46)));
	inst.insert(make_pair(L"divs.4", Instruction(47)));
	inst.insert(make_pair(L"mov.16", Instruction(48)));
	inst.insert(make_pair(L"not.16", Instruction(49)));
	inst.insert(make_pair(L"or.16", Instruction(50)));
	inst.insert(make_pair(L"and.16", Instruction(51)));
	inst.insert(make_pair(L"xor.16", Instruction(52)));
	inst.insert(make_pair(L"mfh", Instruction(53)));
	inst.insert(make_pair(L"mfl", Instruction(54)));
	inst.insert(make_pair(L"clr", Instruction(55)));
	inst.insert(make_pair(L"neg", Instruction(56)));
	inst.insert(make_pair(L"nop", Instruction(57)));
	inst.insert(make_pair(L"adc.16", Instruction(58)));
	inst.insert(make_pair(L"sbb.16", Instruction(59)));
	inst.insert(make_pair(L"mul.16", Instruction(60)));
	inst.insert(make_pair(L"muls.16", Instruction(61)));
	inst.insert(make_pair(L"div.16", Instruction(62)));
	inst.insert(make_pair(L"divs.16", Instruction(63)));
	inst.insert(make_pair(L"ldi.4 0", Instruction(64)));
	inst.insert(make_pair(L"ldi.4 1", Instruction(65)));
	inst.insert(make_pair(L"ldi.4 2", Instruction(66)));
	inst.insert(make_pair(L"ldi.4 3", Instruction(67)));
	inst.insert(make_pair(L"ldi.4 4", Instruction(68)));
	inst.insert(make_pair(L"ldi.4 5", Instruction(69)));
	inst.insert(make_pair(L"ldi.4 6", Instruction(70)));
	inst.insert(make_pair(L"ldi.4 7", Instruction(71)));
	inst.insert(make_pair(L"ldi.4 8", Instruction(72)));
	inst.insert(make_pair(L"ldi.4 9", Instruction(73)));
	inst.insert(make_pair(L"ldi.4 10", Instruction(74)));
	inst.insert(make_pair(L"ldi.4 11", Instruction(75)));
	inst.insert(make_pair(L"ldi.4 12", Instruction(76)));
	inst.insert(make_pair(L"ldi.4 13", Instruction(77)));
	inst.insert(make_pair(L"ldi.4 14", Instruction(78)));
	inst.insert(make_pair(L"ldi.4 15", Instruction(79)));
	inst.insert(make_pair(L"ldr.1", Instruction(80)));
	inst.insert(make_pair(L"ldri.1", Instruction(81)));
	inst.insert(make_pair(L"ldrd.1", Instruction(82)));
	inst.insert(make_pair(L"str.1", Instruction(83)));
	inst.insert(make_pair(L"stri.1", Instruction(84)));
	inst.insert(make_pair(L"strd.1", Instruction(85)));
	inst.insert(make_pair(L"cli", Instruction(86)));
	inst.insert(make_pair(L"inci", Instruction(87)));
	inst.insert(make_pair(L"add4i", Instruction(88)));
	inst.insert(make_pair(L"mfi", Instruction(89)));
	inst.insert(make_pair(L"mti", Instruction(90)));
	inst.insert(make_pair(L"clj", Instruction(91)));
	inst.insert(make_pair(L"incj", Instruction(92)));
	inst.insert(make_pair(L"add4j", Instruction(93)));
	inst.insert(make_pair(L"mfj", Instruction(94)));
	inst.insert(make_pair(L"mtj", Instruction(95)));
	inst.insert(make_pair(L"ldr.4", Instruction(96)));
	inst.insert(make_pair(L"ldri.4", Instruction(97)));
	inst.insert(make_pair(L"ldrd.4", Instruction(98)));
	inst.insert(make_pair(L"str.4", Instruction(99)));
	inst.insert(make_pair(L"stri.4", Instruction(100)));
	inst.insert(make_pair(L"strd.4", Instruction(101)));
	inst.insert(make_pair(L"clc", Instruction(102)));
	inst.insert(make_pair(L"sec", Instruction(103)));
	inst.insert(make_pair(L"mfc", Instruction(104)));
	inst.insert(make_pair(L"clm", Instruction(105)));
	inst.insert(make_pair(L"sem", Instruction(106)));
	inst.insert(make_pair(L"mfm", Instruction(107)));
	inst.insert(make_pair(L"mfv", Instruction(108)));
	inst.insert(make_pair(L"mtv", Instruction(109)));
	inst.insert(make_pair(L"flp", Instruction(110)));
	inst.insert(make_pair(L"swp", Instruction(111)));
	inst.insert(make_pair(L"ldr.16", Instruction(112)));
	inst.insert(make_pair(L"ldri.16", Instruction(113)));
	inst.insert(make_pair(L"ldrd.16", Instruction(114)));
	inst.insert(make_pair(L"str.16", Instruction(115)));
	inst.insert(make_pair(L"stri.16", Instruction(116)));
	inst.insert(make_pair(L"strd.16", Instruction(117)));
	inst.insert(make_pair(L"bcc", Instruction(118)));
	inst.insert(make_pair(L"bccr", Instruction(119)));
	inst.insert(make_pair(L"bz", Instruction(120)));
	inst.insert(make_pair(L"bzr", Instruction(121)));
	inst.insert(make_pair(L"bn", Instruction(122)));
	inst.insert(make_pair(L"bnr", Instruction(123)));
	//inst.insert(make_pair(L"", Instruction(124)));
	//inst.insert(make_pair(L"", Instruction(125)));
	inst.insert(make_pair(L"ldi.1 0", Instruction(126)));
	inst.insert(make_pair(L"ldi.1 1", Instruction(127)));
	
	inst.insert(make_pair(L"op1", Instruction(0, InstructionType::mnemonic_expect_registername)));
	inst.insert(make_pair(L"op2", Instruction(8, InstructionType::mnemonic_expect_registername)));

	inst.insert(make_pair(L"ldi.4", Instruction(64, InstructionType::mnemonic_expect_number)));
	inst.insert(make_pair(L"ldi.1", Instruction(126, InstructionType::mnemonic_expect_number)));

	inst.insert(make_pair(L"a", Instruction(0, InstructionType::registername)));
	inst.insert(make_pair(L"b", Instruction(1, InstructionType::registername)));
	inst.insert(make_pair(L"d", Instruction(2, InstructionType::registername)));
	inst.insert(make_pair(L"e", Instruction(3, InstructionType::registername)));
	inst.insert(make_pair(L"f", Instruction(4, InstructionType::registername)));
	inst.insert(make_pair(L"g", Instruction(5, InstructionType::registername)));
	inst.insert(make_pair(L"k", Instruction(6, InstructionType::registername)));
	inst.insert(make_pair(L"p", Instruction(7, InstructionType::registername)));

	inst.insert(make_pair(L"binclude", Instruction(InstructionType::directive)));
	inst.insert(make_pair(L"define", Instruction(InstructionType::directive)));
	inst.insert(make_pair(L"equ", Instruction(InstructionType::directive)));
	inst.insert(make_pair(L"ldi.16", Instruction(InstructionType::directive)));

	inst.insert(make_pair(L"+", Instruction(InstructionType::$operator, 11)));	//add, pos(13)
	inst.insert(make_pair(L"-", Instruction(InstructionType::$operator, 11)));	//sub, neg(13)
	inst.insert(make_pair(L"*", Instruction(InstructionType::$operator, 12)));	//mul
	inst.insert(make_pair(L"/", Instruction(InstructionType::$operator, 12)));	//div
	inst.insert(make_pair(L"%", Instruction(InstructionType::$operator, 12)));	//mod
	//inst.insert(make_pair(L"**", Instruction(InstructionType::$operator, 14, associativity::right_associative)));	//pow
	inst.insert(make_pair(L"|", Instruction(InstructionType::$operator, 5)));	//bitwise or
	inst.insert(make_pair(L"&", Instruction(InstructionType::$operator, 7)));	//bitwise and
	inst.insert(make_pair(L"^", Instruction(InstructionType::$operator, 6)));	//bitwise xor
	inst.insert(make_pair(L"~", Instruction(InstructionType::$operator, 15, Associativity::right_associative)));	//bitwise not
	inst.insert(make_pair(L"<<", Instruction(InstructionType::$operator, 10)));	//shift left
	inst.insert(make_pair(L">>", Instruction(InstructionType::$operator, 10)));	//logical shift right
	inst.insert(make_pair(L">>>", Instruction(InstructionType::$operator, 10)));	//arithmetic shift right
	inst.insert(make_pair(L"||", Instruction(InstructionType::$operator, 2)));	//bool or
	inst.insert(make_pair(L"&&", Instruction(InstructionType::$operator, 4)));	//bool and
	inst.insert(make_pair(L"^^", Instruction(InstructionType::$operator, 3)));	//bool xor
	inst.insert(make_pair(L"!", Instruction(InstructionType::$operator, 15, Associativity::right_associative)));	//bool not
	inst.insert(make_pair(L"<", Instruction(InstructionType::$operator, 9)));	//bool less than
	inst.insert(make_pair(L">", Instruction(InstructionType::$operator, 9)));	//bool greater than
	inst.insert(make_pair(L"<=", Instruction(InstructionType::$operator, 9)));	//bool less or equal
	inst.insert(make_pair(L">=", Instruction(InstructionType::$operator, 9)));	//bool greater or equal
	inst.insert(make_pair(L"==", Instruction(InstructionType::$operator, 8)));	//bool equal
	inst.insert(make_pair(L"!=", Instruction(InstructionType::$operator, 8)));	//bool not equal
	inst.insert(make_pair(L",", Instruction(InstructionType::$operator, 1)));	//bool not equal
}

Instructions::~Instructions()
{
}