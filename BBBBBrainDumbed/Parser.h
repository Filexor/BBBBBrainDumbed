#pragma once
#include <vector>
#include <list>
#include <stdexcept>
#include <fstream>
#include <deque>

#include "Instructions.h"
#include "Tokenizer.h"

using namespace std;

class ParserError : public runtime_error {
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
	Parser(list<Token>* _input);
	~Parser();
	bool hasNumber(Token input);
	bool isParsable(Token input);
	int64_t toNumber();
	list<Token>::iterator peekToken();
	list<Token>::iterator getToken();
	bool isUnary(list<Token>::iterator begin);
	int64_t parse_unary(list<Token>::iterator begin, bool allowUnknown);
	int64_t parse_main(list<Token>::iterator begin, int64_t lhs, int64_t precedence, bool allowUnknown);
	int64_t parse_init(bool allowUnknown);
	vector<bool> parse();
private:

};

Parser::Parser(list<Token>* _input)
{
	input = _input;
}

Parser::~Parser()
{
}

inline bool Parser::hasNumber(Token input)
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

inline bool Parser::isParsable(Token input)
{
	return hasNumber(input) || input.type == $TokenType::LeftParenthesis || input.type == $TokenType::Operator;
}

inline int64_t Parser::toNumber()
{
	auto j = i->token;
	auto k = insts.inst.find(j);
	if (k != insts.inst.end())
	{
		if (k->second.itype == InstructionType::knownnumber)
		{
			return k->second.value;
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
		throw ParserError("not a number", *i);
	}
	return 0;
}

inline list<Token>::iterator Parser::peekToken()
{
	auto j = ++i;
	--i;
	return j;
}

inline list<Token>::iterator Parser::getToken()
{
	return ++i;
}

inline bool Parser::isUnary(list<Token>::iterator begin)
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

inline int64_t Parser::parse_unary(list<Token>::iterator begin, bool allowUnknown)
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
	else if (hasNumber(*i) && !allowUnknown)
	{
		value = toNumber();
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

inline int64_t Parser::parse_main(list<Token>::iterator begin, int64_t lhs, int64_t precedence, bool allowUnknown)
{
	list<Token>::iterator j = peekToken();
	while ((j) != input->end() && (j->token != L")") && insts.inst.find((j)->token)->second.itype == InstructionType::$operator && insts.inst.find((j)->token)->second.value >= precedence)
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

inline int64_t Parser::parse_init(bool allowUnknown)
{
	return parse_main(i, parse_unary(i, allowUnknown), 0, allowUnknown);
}

inline vector<bool> Parser::parse()
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
					size_t size = 0, offset = 0;
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
						insts.inst.insert_or_assign((*k).token, Instruction(InstructionType::knownnumber, l));
					}
					else
					{
						throw ParserError("keyword cannot be used", *k);
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
		}
		i++;
	}
	for (size_t j = 0; j < TBR.size(); j++)
	{
		if (TBR[j].second->token == L"ldi.16")
		{
			auto k = TBR[j].second;
			k++;
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
	}
	return output;
}
