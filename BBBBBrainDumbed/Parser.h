#pragma once
#include <vector>
#include <list>
#include <map>
#include <stdexcept>
#include <fstream>
#include <deque>
#include <bitset>

#include "Instructions.h"
#include "Tokenizer.h"

using namespace std;

class Macro
{
public:
	vector<Token> args;
	list<Token> body;
	Instructions insts;
};

class ParserError : public runtime_error
{
public:
	Token token;
	ParserError(string message, Token _token) :runtime_error(message) {
		token = _token;
	}
};

class Parser
{
public:
	Instructions insts;
	list<Token>* input;
	list<Token>::iterator i;
	vector<wstring> fileHierarchy;
	map<wstring, Macro> macros;
	vector<wstring> macroHierarchy;
	Parser(list<Token>* _input, wstring _filename);
	~Parser();
	bool hasNumber(Token input);
	bool isParsable(Token input);
	int64_t toNumber(bool allowUnknown);
	list<Token>::iterator peekToken();
	list<Token>::iterator getToken();
	bool isUnary(list<Token>::iterator begin);
	int64_t parse_unary(list<Token>::iterator begin, bool allowUnknown);
	int64_t parse_main(list<Token>::iterator begin, int64_t lhs, int64_t precedence, bool allowUnknown);
	int64_t parse_init(bool allowUnknown);
	bool checkDependencyCycleAndAssign(vector<wstring>* Hierarchy, wstring name);
	vector<bool> parse();
private:

};

Parser::Parser(list<Token>* _input, wstring _filename)
{
	input = _input;
	wchar_t* fullpath = _wfullpath(NULL, _filename.c_str(), _MAX_PATH);
	if (!fullpath)
	{
		throw runtime_error("invalid path");
	}
	fileHierarchy.push_back(wstring(fullpath));
}

Parser::~Parser()
{
}

bool Parser::hasNumber(Token input)
{
	/*
		followings has number: binary(start with 0b), quaternary(start with 0q), octal(start with 0o or 0), decimal(no prefix or start with 0d), hexadecimal(start with 0x), quoted text(surrounded by ' or "), identifier(enything else without end with :), label(enything else with end with :)
		followings does not have number: mnemonic, directive, operator
	*/
	auto i = insts.inst.find(input.token);
	if (i != insts.inst.end() && (i->second.itype == InstructionType::mnemonic || i->second.itype == InstructionType::directive || i->second.itype == InstructionType::$operator))
	{
		return false;
	}
	return true;
}

bool Parser::isParsable(Token input)
{
	return hasNumber(input) || input.type == $TokenType::LeftParenthesis || input.type == $TokenType::Operator;
}

int64_t Parser::toNumber(bool allowUnknown)
{
	auto j = i->token;
	auto k = insts.inst.find(j);
	if (k != insts.inst.end())
	{
		if (k->second.itype == InstructionType::knownnumber)
		{
			return k->second.value;
		}
		else if (k->second.itype == InstructionType::unknownnumber && allowUnknown)
		{
			return 0;
		}
		else
		{
			throw ParserError("unresolved value", *i);
		}
	}
	else if (j[0] == L'0')
	{
		if (j[1] == L'b')
		{
			j.erase(0, 2);
			return stoll(j, nullptr, 2);
		}
		else if (j[1] == L'q')
		{
			j.erase(0, 2);
			return stoll(j, nullptr, 4);
		}
		else if (j[1] == L'o')
		{
			j.erase(0, 2);
			return stoll(j, nullptr, 8);
		}
		else if (j[1] == L'd')
		{
			j.erase(0, 2);
			return stoll(j, nullptr, 10);
		}
		else if (j[1] == L'x')
		{
			j.erase(0, 2);
			return stoll(j, nullptr, 16);
		}
		else
		{
			return stoll(j, nullptr, 0);
		}
	}
	else if (j[0] >= L'1' && j[0] <= L'9')
	{
		return stoll(j, nullptr, 10);
	}
	else if ((j[0] == L'\"' && j.back() == L'\"') || (j[0] == L'\'' && j.back() == L'\''))
	{
		j.erase(0, 1);
		j.erase(j.size() - 1, 1);
		j.resize(sizeof(wchar_t) * 4, 0);
		return (int64_t)*j.c_str();
	}
	else
	{
		//throw ParserError("not a number", *i);
		insts.inst.insert(make_pair(j, Instruction(InstructionType::unknownnumber)));
	}
	return 0;
}

list<Token>::iterator Parser::peekToken()
{
	auto j = ++i;
	--i;
	return j;
}

list<Token>::iterator Parser::getToken()
{
	return ++i;
}

bool Parser::isUnary(list<Token>::iterator begin)
{
	bool output = false;
	if (i == begin)
	{
		return true;
	}
	--i;
	auto j = insts.inst.find(i->token);
	if (j != insts.inst.end() && j->second.itype == InstructionType::$operator)
	{
		output = true;
	}
	if (i->token == L")")
	{
		output = true;
	}
	++i;
	return output;
}

int64_t Parser::parse_unary(list<Token>::iterator begin, bool allowUnknown)
{
	int64_t value = 0;
	if (i->token == L"(")
	{
		getToken();
		value = parse_init(allowUnknown);
		getToken();
		if (i->token != L")")
		{
			throw runtime_error("Right parenthesis missing");
		}
	}
	else if (i->token == L"-" && isUnary(begin) && !allowUnknown)	//unary minus if previous token does not exist or is operator or right parenthesis
	{
		getToken();
		value -= parse_unary(begin, allowUnknown);
	}
	else if (i->token == L"+" && isUnary(begin) && !allowUnknown)
	{
		getToken();
		value += parse_unary(begin, allowUnknown);
	}
	else if (i->token == L"~" && isUnary(begin) && !allowUnknown)
	{
		getToken();
		value = ~parse_unary(begin, allowUnknown);
	}
	else if (i->token == L"!" && isUnary(begin) && !allowUnknown)
	{
		getToken();
		value = !parse_unary(begin, allowUnknown);
	}
	else if (hasNumber(*i))
	{
		value = toNumber(allowUnknown);
	}
	else if (allowUnknown)
	{
		getToken();
	}
	else
	{

	}
	return value;
}

int64_t Parser::parse_main(list<Token>::iterator begin, int64_t lhs, int64_t precedence, bool allowUnknown)
{
	list<Token>::iterator j = peekToken();
	auto k = insts.inst.find((j)->token);
	while ((j) != input->end() && (j->token != L")") && k != insts.inst.end() && k->second.itype == InstructionType::$operator && k->second.value >= precedence)
	{
		Token op = *j;
		getToken();
		getToken();
		int64_t rhs = parse_unary(begin, allowUnknown);
		j = peekToken();
		while ((j != input->end()) && (j->token != L")") && ((insts.inst.find(op.token)->second.value < insts.inst.find((j)->token)->second.value) || (insts.inst.find((j)->token)->second.atype == Associativity::right_associative && (insts.inst.find(op.token)->second.value == insts.inst.find((j)->token)->second.value))))
		{
			rhs = parse_main(begin, rhs, insts.inst.find(op.token)->second.value + 1, allowUnknown);
			j = peekToken();
		}
		if (!allowUnknown)
		{
			if (op.token == L"+")
			{
				lhs += rhs;
			}
			else if (op.token == L"-")
			{
				lhs -= rhs;
			}
			else if (op.token == L"*")
			{
				lhs *= rhs;
			}
			else if (op.token == L"/")
			{
				lhs /= rhs;
			}
			else if (op.token == L"%")
			{
				lhs %= rhs;
			}
			else if (op.token == L"|")
			{
				lhs |= rhs;
			}
			else if (op.token == L"&")
			{
				lhs &= rhs;
			}
			else if (op.token == L"^")
			{
				lhs ^= rhs;
			}
			else if (op.token == L"<<")
			{
				lhs = lhs << rhs;
			}
			else if (op.token == L">>")
			{
				lhs = ((uint64_t)lhs) >> rhs;
			}
			else if (op.token == L">>>")
			{
				lhs = ((int64_t)lhs) >> rhs;
			}
			else if (op.token == L"||")
			{
				lhs = (lhs != 0) || (rhs != 0);
			}
			else if (op.token == L"&&")
			{
				lhs = (lhs != 0) && (rhs != 0);
			}
			else if (op.token == L"^^")
			{
				lhs = (lhs != 0) != (rhs != 0);
			}
			else if (op.token == L"<")
			{
				lhs = lhs < rhs;
			}
			else if (op.token == L">")
			{
				lhs = lhs > rhs;
			}
			else if (op.token == L"<=")
			{
				lhs = lhs <= rhs;
			}
			else if (op.token == L">=")
			{
				lhs = lhs >= rhs;
			}
			else if (op.token == L"!=")
			{
				lhs = lhs != rhs;;
			}
			else if (op.token == L"==")
			{
				lhs = lhs == rhs;
			}
		}
	}
	return lhs;
}

int64_t Parser::parse_init(bool allowUnknown)
{
	return parse_main(i, parse_unary(i, allowUnknown), 0, allowUnknown);
}

bool Parser::checkDependencyCycleAndAssign(vector<wstring>* Hierarchy, wstring name)
{
	for (size_t i = 0; i < Hierarchy->size(); i++)
	{
		if ((*Hierarchy)[i].compare(name))
		{
			return false;
		}
	}
	Hierarchy->push_back(name);
	return true;
}

vector<bool> Parser::parse()
{
	vector<bool> output;
	vector<pair<size_t, list<Token>::iterator>> TBR;	//to be resolved. <binary position, directive>
	; i = input->begin();
	while (i != input->end())
	{
		for (basic_string<wchar_t>::size_type j = 0; j < (*i).token.length(); j++)
		{
			(*i).token[j] = towlower((*i).token[j]);
		}
		i++;
	}
	/*
	processing order: convert to binary (leave unresolved reference empty) -> resolve reference -> overwrite resolved reference -> end

	What to do if we meet like this:
		binclude "filename" filesize	;<- size need to know filesize
		filesize:	;<- filesize need to know size of binclude which defined by filesize (self reference)
	Dependency of label is all of previously appeared size-defining identifier
	*/
	i = input->begin();
	while (i != input->end())
	{
		auto j = insts.inst.find((*i).token);
		if (j == insts.inst.end())
		{
			if ((*i).type == $TokenType::Label)	//label
			{
				wstring l = (*i).token;
				l.pop_back();
				insts.inst.insert_or_assign(l, Instruction(InstructionType::knownnumber, output.size()));
			}
			else	//identifier
			{
				throw ParserError("identifier must be come with mnemonic or directive", *i);
			}
		}
		else
		{
			if (j->second.itype == InstructionType::mnemonic)
			{
				for (size_t k = 0; k < j->second.opcode.size(); k++)
				{
					output.push_back(j->second.opcode.test(k));
				}
			}
			else if (j->second.itype == InstructionType::mnemonic_expect_number)
			{
				TBR.push_back(make_pair(output.size(), i));
				i++;
				if (!isParsable(*i))
				{
					throw ParserError("parsable token expacted", *i);
				}
				parse_init(true);
				for (size_t k = 0; k < 7 ; k++)
				{
					output.push_back(false);
				}
			}
			else if (j->second.itype == InstructionType::mnemonic_expect_registername)
			{
				i++;
				auto k = insts.inst.find((*i).token);
				if (k->second.itype != InstructionType::registername)
				{
					throw ParserError("register name expacted", *i);
				}
				uint8_t l = (uint8_t)(j->second.opcode.to_ullong() | k->second.opcode.to_ullong());
				for (size_t m = 0; m < j->second.opcode.size(); m++)
				{
					output.push_back((l >> m) & 0x1);
				}
			}
			else if (j->second.itype == InstructionType::directive)
			{
				if (j->first == L"binclude")	//format: binclude filename [offset] [size]
				{
					i++;
					if (i->type != $TokenType::QuotedText)
					{
						throw ParserError("file name must be quoted", *i);
					}
					wstring filepath = (*i).token;
					int64_t size = 0, offset = 0;
					i++;
					if (isParsable(*i))
					{
						offset = parse_init(false);
						i++;
						if (isParsable(*i))
						{
							size = parse_init(false);
						}
						else
						{
							i--;
						}
					}
					else
					{
						i--;
					}
					basic_ifstream<char> ifs;
					ifs.open(filepath, ios_base::binary | ios_base::in);
					if (ifs.fail())
					{
						throw ParserError("failed to open file", *i);
					}
					istreambuf_iterator<char> ifsbegin(ifs), ifsend;
					string finput(ifsbegin, ifsend);
					ifs.close();
					deque<bool> binput;
					for (size_t k = 0; k < finput.size(); k++)
					{
						binput.push_back(finput[k] & 1);
						binput.push_back((finput[k] >> 1) & 1);
						binput.push_back((finput[k] >> 2) & 1);
						binput.push_back((finput[k] >> 3) & 1);
						binput.push_back((finput[k] >> 4) & 1);
						binput.push_back((finput[k] >> 5) & 1);
						binput.push_back((finput[k] >> 6) & 1);
						binput.push_back((finput[k] >> 7) & 1);
					}
					binput.resize(offset + size);
					for (size_t k = offset; k < size; k++)
					{
						output.push_back(binput[k]);
					}
				}
				else if (j->first == L"include")	//format: include filename
				{
					i++;
					if (i->type != $TokenType::QuotedText)
					{
						throw ParserError("file name must be quoted", *i);
					}
					wstring filepath = (*i).token;
					wchar_t* fullpath = _wfullpath(NULL, filepath.c_str(), _MAX_PATH);
					if (!fullpath)
					{
						throw ParserError("invalid path", *i);
					}
					if (!checkDependencyCycleAndAssign(&fileHierarchy, wstring(fullpath)))
					{
						throw ParserError("file dependency cycle detected", *i);
					}
					basic_ifstream<wchar_t> ifs;
					ifs.open(filepath, ios_base::binary | ios_base::in);
					if (ifs.fail())
					{
						throw ParserError("failed to open file", *i);
					}
					istreambuf_iterator<wchar_t> ifsbegin(ifs), ifsend;
					wstring finput(ifsbegin, ifsend);
					ifs.close();
					list<Token>* token = Tokenizer::tokenize(finput, filepath);
					i++;
					auto k = --(token->end());
					Token eof = *k;
					eof.token = L" endoffile";
					eof.type = $TokenType::Genetated;
					input->insert(i, eof);
					input->insert(i, token->begin(), token->end());

				}
				else if (j->first == L"define")
				{
					auto k = ++i;
					if (i == input->end())
					{
						throw runtime_error("unexpected end of file");
					}
					i++;
					int64_t l = parse_init(false);
					if (insts.inst.find((*k).token) == insts.inst.end() || insts.inst.find((*k).token)->second.itype == InstructionType::knownnumber || insts.inst.find((*k).token)->second.itype == InstructionType::unknownnumber)
					{
						insts.inst.insert_or_assign(k->token, Instruction(InstructionType::knownnumber, l));
					}
					else
					{
						throw ParserError("keyword cannot be used", *k);
					}
				}
				else if (j->first == L"macro")	//format: macro identifier [(argument ...)] endmacro
				{
					Macro macro = Macro();
					i++;
					auto l = i;
					if (!checkDependencyCycleAndAssign(&macroHierarchy, i->token))
					{
						throw ParserError("macro dependency cycle detected", *i);
					}
					insts.inst.insert_or_assign(i->token, Instruction(InstructionType::macro));
					if (macroHierarchy.size() <= 1)
					{
						macro.insts = insts;
					}
					else
					{
						auto k = macroHierarchy.end();
						k--;
						k--;
						macro.insts = macros.find(*k)->second.insts;
					}
					i++;
					if (i->type == $TokenType::LeftParenthesis)
					{
						i++;
						while (i->type != $TokenType::RightParenthesis)
						{
							if (macro.insts.inst.find(i->token) != macro.insts.inst.end())
							{
								throw ParserError("identfier of the argument is already taken", *i);
							}
							macro.args.push_back(*i);
							i++;
						}
						i++;
					}
					while (i->token != L"endmacro")
					{
						macro.body.push_back(*i);
						i++;
					}
					macros.insert_or_assign(l->token, macro);
				}
				else if (j->first == L"ed")	//format: ed size(0<n<=64) data (...) enddata
				{
					TBR.push_back(make_pair(output.size(), i));
					i++;
					int64_t size;
					if (!isParsable(*i))
					{
						throw ParserError("parsable token expacted", *i);
					}
					size = parse_init(false);
					if (size > 64 || size <= 0)
					{
						throw ParserError("data size too small or too large\nsize must be in 0 < size <= 64", *i);
					}
					while (i->token != L"enddata")
					{
						parse_init(true);
						for (size_t k = 0; k < size; k++)
						{
							output.push_back(false);
						}
					}
				}
				else if (j->first == L"ldi.16")	//accepts label as value. format: ldi value
				{
					TBR.push_back(make_pair(output.size(), i));
					i++;
					if (!isParsable(*i))
					{
						throw ParserError("parsable token expacted", *i);
					}
					parse_init(true);
					for (size_t k = 0; k < 7 * 4; k++)
					{
						output.push_back(false);
					}
				}
			}
			else if (j->second.itype == InstructionType::macro)	//format: identifier [ ['('] argument [')'] ...]
			{
				auto k_ = macros.find(i->token);
				if (k_ == macros.end())
				{
					throw ParserError("the macro does not found", *i);
				}
				auto k = k_->second;
				i++;
				size_t l = 0;
				while (l < k.args.size())
				{
					list<Token> replaceList;
					size_t parenthesisDepth = 0;
					if (i->type == $TokenType::LeftParenthesis)
					{
						i++;
						parenthesisDepth++;
						while (i->type != $TokenType::RightParenthesis && parenthesisDepth <= 1)
						{
							if (i->type != $TokenType::LeftParenthesis)
							{
								parenthesisDepth++;
							}
							else if (i->type != $TokenType::RightParenthesis)
							{
								parenthesisDepth--;
							}
							replaceList.push_back(*i);
							i++;
						}
						i++;
					}
					auto m = k.body.begin();
					while (m != k.body.end())
					{
						m = find_if(m, k.body.end(), [&](Token o) {return o.token == k.args[l].token; });
						m = k.body.erase(m);
						k.body.insert(m, replaceList.begin(), replaceList.end());
						for (size_t p = 0; p < replaceList.size(); p++)
						{
							m++;
						}
					}
					i++;
					l++;
				}
				i = input->insert(i, k.body.begin(), k.body.end());
			}
			else if (j->second.itype == InstructionType::endoffile)
			{
				fileHierarchy.pop_back();
			}
		}
		i++;
	}
	for (size_t j = 0; j < TBR.size(); j++)
	{
		if (TBR[j].second->token == L"ed")
		{
			auto k = TBR[j].second;
			k++;
			i = k;
			int64_t size = parse_init(false);
			while (i->token != L"enddata")
			{
				int64_t l = parse_init(false);
				for (size_t k = 0; k < size; k++)
				{
					output.push_back(l & 1);
					l >>= 1;
				}
			}
		}
		if (TBR[j].second->token == L"ldi.16")
		{
			auto k = TBR[j].second;
			k++;
			i = k;
			int64_t l = parse_init(false);
			l &= 0xffff;
			uint8_t m0, m1, m2, m3;
			m0 = (l) & 0xf | 0x40;
			m1 = (l >> 4) & 0xf | 0x40;
			m2 = (l >> 8) & 0xf | 0x40;
			m3 = (l >> 12) & 0xf | 0x40;
			for (size_t n = 0; n < 7; n++)
			{
				output[TBR[j].first + n] = (m0 >> n) & 1;
				output[TBR[j].first + n + 7] = (m1 >> n) & 1;
				output[TBR[j].first + n + 7 * 2] = (m2 >> n) & 1;
				output[TBR[j].first + n + 7 * 3] = (m3 >> n) & 1;
			}
		}
		else if (TBR[j].second->token == L"ldi.4")
		{
			auto k = TBR[j].second;
			k++;
			i = k;
			int64_t l = parse_init(false);
			l &= 0xf;
			bitset<7> n = l;
			uint8_t m0;
			m0 = (uint8_t)(n | insts.inst.find(TBR[j].second->token)->second.opcode).to_ullong();
			for (size_t n = 0; n < 7; n++)
			{
				output[TBR[j].first + n] = (m0 >> n) & 1;
			}
		}
		else if (TBR[j].second->token == L"ldi.1")
		{
			auto k = TBR[j].second;
			k++;
			i = k;
			int64_t l = parse_init(false);
			l &= 0x1;
			bitset<7> n = l;
			uint8_t m0;
			m0 = (uint8_t)(n | insts.inst.find(TBR[j].second->token)->second.opcode).to_ullong();
			for (size_t n = 0; n < 7; n++)
			{
				output[TBR[j].first + n] = (m0 >> n) & 1;
			}
		}
	}
	return output;
}
