#pragma once

#include <string>
#include <list>
#include <stdexcept>

using namespace std;

enum class $TokenType {
	Default,
	Label,
	ExposedDelimiter,
	NonexposedDelimiter,
	LeftParenthesis,
	RightParenthesis,
	Operator,
	QuotedText,
};

class Token {
public:
	wstring token = L"";
	$TokenType type = $TokenType::Default;
	wstring filename = L"";
	basic_string<wchar_t>::size_type line = 0;
	basic_string<wchar_t>::size_type digit = 0;
};

class TokenizerError : public runtime_error {
public:
	Token token;
	TokenizerError(string message, Token _token) :runtime_error(message) {
		token = _token;
	}
};

class Tokenizer
{
public:
	Tokenizer();
	~Tokenizer();
	static list<Token>* tokenize(wstring input, wstring filename);
private:

};

Tokenizer::Tokenizer()
{
}

Tokenizer::~Tokenizer()
{
}

list<Token>* Tokenizer::tokenize(wstring input, wstring filename) {
	size_t parenthesisDepth = 0;
	list<Token>* output = new list<Token>();
	basic_string<wchar_t>::size_type i = 0;
	basic_string<wchar_t>::size_type line = 1;
	basic_string<wchar_t>::size_type digit = 1;
	while (true)
	{
		Token tmp;
		if (i >= input.length() || input[i] == L'\0')	//end of file
		{
			break;
		}
		if (input[i] == L',')
		{
			tmp.token.push_back(input[i]);
			if (parenthesisDepth != 0)
			{
				tmp.type = $TokenType::NonexposedDelimiter;
			}
			else
			{
				tmp.type = $TokenType::ExposedDelimiter;
			}
			tmp.filename = filename;
			tmp.digit = digit;
			tmp.line = line;
			i++;
			digit++;
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L'(')
		{
			parenthesisDepth++;
			tmp.token.push_back(input[i]);
			tmp.type = $TokenType::LeftParenthesis;
			tmp.filename = filename;
			tmp.digit = digit;
			tmp.line = line;
			i++;
			digit++;
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L')')
		{
			parenthesisDepth--;
			tmp.token.push_back(input[i]);
			tmp.type = $TokenType::RightParenthesis;
			tmp.filename = filename;
			tmp.digit = digit;
			tmp.line = line;
			if (parenthesisDepth < 0)
			{
				throw TokenizerError("Parenthesis depth underrun", tmp);
			}
			i++;
			digit++;
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L'+' || input[i] == L'-' || input[i] == L'*' || input[i] == L'/' || input[i] == L'%' || input[i] == L'~')	//label and operands
		{
			tmp.token.push_back(input[i]);
			tmp.type = $TokenType::Operator;
			tmp.filename = filename;
			tmp.digit = digit;
			tmp.line = line;
			i++;
			digit++;
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L'<')
		{
			if ((i + 1) < input.length() && (input[i + 1] == L'<' || input[i + 1] == L'='))
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'>')
		{
			if ((i + 2) < input.length() && input[i + 1] == L'>' && input[i + 2] == L'>')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.token.push_back(input[i + 2]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 3;
				digit += 3;
				output->push_back(tmp);
				continue;
			}
			else if ((i + 1) < input.length() && (input[i + 1] == L'>' || input[i + 1] == L'='))
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'|')
		{
			if ((i + 1) < input.length() && input[i + 1] == L'|')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'&')
		{
			if ((i + 1) < input.length() && input[i + 1] == L'&')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'^')
		{
			if ((i + 1) < input.length() && input[i + 1] == L'^')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'!')
		{
			if ((i + 1) < input.length() && input[i + 1] == L'=')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L'=')
		{
			if ((i + 1) < input.length() && input[i + 1] == L'=')
			{
				tmp.token.push_back(input[i]);
				tmp.token.push_back(input[i + 1]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i += 2;
				digit += 2;
				output->push_back(tmp);
				continue;
			}
			else
			{
				tmp.token.push_back(input[i]);
				tmp.type = $TokenType::Operator;
				tmp.filename = filename;
				tmp.digit = digit;
				tmp.line = line;
				i++;
				digit++;
				output->push_back(tmp);
				continue;
			}
		}
		if (input[i] == L';')	//comment
		{
			i++;
			digit++;
			while (input[i] != L'\n' && input[i] != L'\r')
			{
				i++;
				digit++;
			}
			continue;
		}
		if (input[i] == L'\'')	//single quote
		{
			tmp.type = $TokenType::QuotedText;
			tmp.filename = filename;
			tmp.line = line;
			tmp.digit = digit;
			i++;
			digit++;
			while (true)
			{
				if (i >= input.length())
				{
					throw TokenizerError("Unexpected end of file", tmp);
					break;
				}
				if (input[i] == L'\\')
				{
					i++;
					digit++;
					if (i >= input.length())
					{
						throw TokenizerError("Unexpected end of file", tmp);
						break;
					}
					if (input[i] == L'a')
					{
						tmp.token.push_back(L'\a');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'b')
					{
						tmp.token.push_back(L'\b');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'f')
					{
						tmp.token.push_back(L'\f');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'n')
					{
						tmp.token.push_back(L'\n');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'r')
					{
						tmp.token.push_back(L'\r');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L't')
					{
						tmp.token.push_back(L'\t');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'v')
					{
						tmp.token.push_back(L'\v');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\\')
					{
						tmp.token.push_back(L'\\');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\'')
					{
						tmp.token.push_back(L'\'');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\"')
					{
						tmp.token.push_back(L'\"');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\?')
					{
						tmp.token.push_back(L'\?');
						i++;
						digit++;
						continue;
					}
					if (input[i] >= L'0' && input[i] <= L'7')
					{
						wstring tmpString = L"";
						while (input[i] >= L'0' && input[i] <= L'7')
						{
							tmpString.push_back(input[i]);
							i++;
							digit++;
						}
						basic_string<wchar_t>::size_type j = tmpString.length();
						while (j > 0)
						{
							basic_string<wchar_t>::size_type k = 0;
							uint64_t l = 0;
							while (k < 16 && j > 0)
							{
								l = l | (stoull(&tmpString[j - 1], nullptr, 8)) << (3 * k);
								//l = l | ((uint64_t)(tmpString[j - 1] - L'0') << 3 * k);
								tmpString.pop_back();
								j--;
								k++;
							}
							uint32_t m = 1;
							if (*(char*)&m)	//little-endian 0x1032547698ba----
							{
								for (basic_string<wchar_t>::size_type n = 0; (k * 3) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&l)[n]);
								}
							}
							else //big-endian 0x----ba9876543210
							{
								uint32_t o = 0;
								o |= l << 56;
								o |= (l & 0x00000000'0000ff00) << 40;
								o |= (l & 0x00000000'00ff0000) << 24;
								o |= (l & 0x00000000'ff000000) << 8;
								o |= (l & 0x000000ff'00000000) >> 8;
								o |= (l & 0x0000ff00'00000000) >> 24;
								o |= (l & 0x00ff0000'00000000) >> 40;
								o |= l >> 56;
								for (basic_string<wchar_t>::size_type n = 0; (k * 3) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&o)[n]);
								}
							}
						}
					}
					if (input[i] == L'x' || input[i] == L'X')
					{
						i++;
						digit++;
						wstring tmpString = L"";
						while ((input[i] >= L'0' && input[i] <= L'9') || (input[i] >= L'a' && input[i] <= L'f') || (input[i] >= L'A' && input[i] <= L'F'))
						{
							tmpString.push_back(input[i]);
							i++;
							digit++;
						}
						basic_string<wchar_t>::size_type j = tmpString.length();
						while (j > 0)
						{
							basic_string<wchar_t>::size_type k = 0;
							uint64_t l = 0;
							while (k < 16 && j > 0)
							{
								//l = l | (_wtoll(&tmpString[j - 1]) << (4 * k));
								l = l | (stoull(&tmpString[j - 1], nullptr, 16)) << (4 * k);
								//l = l | ((uint64_t)(tmpString[j - 1] - L'0') << 3 * k);
								tmpString.pop_back();
								j--;
								k++;
							}
							uint32_t m = 1;
							if (*(char*)&m)	//little-endian 0x1032547698ba----
							{
								for (basic_string<wchar_t>::size_type n = 0; (k * 4) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&l)[n]);
								}
							}
							else //big-endian 0x----ba9876543210
							{
								uint32_t o = 0;
								o |= l << 56;
								o |= (l & 0x00000000'0000ff00) << 40;
								o |= (l & 0x00000000'00ff0000) << 24;
								o |= (l & 0x00000000'ff000000) << 8;
								o |= (l & 0x000000ff'00000000) >> 8;
								o |= (l & 0x0000ff00'00000000) >> 24;
								o |= (l & 0x00ff0000'00000000) >> 40;
								o |= l >> 56;
								for (basic_string<wchar_t>::size_type n = 0; (k * 4) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&o)[n]);
								}
							}
						}
					}
				}
				if (input[i] == L'\'')
				{
					i++;
					digit++;
					break;
				}
				tmp.token.push_back(input[i]);
				i++;
				digit++;
			}
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L'\"')	//double quote
		{
			tmp.type = $TokenType::QuotedText;
			tmp.filename = filename;
			tmp.line = line;
			tmp.digit = digit;
			i++;
			digit++;
			while (true)
			{
				if (i >= input.length())
				{
					throw TokenizerError("Unexpected end of file", tmp);
					break;
				}
				if (input[i] == L'\\')
				{
					i++;
					digit++;
					if (i >= input.length())
					{
						throw TokenizerError("Unexpected end of file", tmp);
						break;
					}
					if (input[i] == L'a')
					{
						tmp.token.push_back(L'\a');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'b')
					{
						tmp.token.push_back(L'\b');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'f')
					{
						tmp.token.push_back(L'\f');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'n')
					{
						tmp.token.push_back(L'\n');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'r')
					{
						tmp.token.push_back(L'\r');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L't')
					{
						tmp.token.push_back(L'\t');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'v')
					{
						tmp.token.push_back(L'\v');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\\')
					{
						tmp.token.push_back(L'\\');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\'')
					{
						tmp.token.push_back(L'\'');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\"')
					{
						tmp.token.push_back(L'\"');
						i++;
						digit++;
						continue;
					}
					if (input[i] == L'\?')
					{
						tmp.token.push_back(L'\?');
						i++;
						digit++;
						continue;
					}
					if (input[i] >= L'0' && input[i] <= L'7')
					{
						wstring tmpString = L"";
						while (input[i] >= L'0' && input[i] <= L'7')
						{
							tmpString.push_back(input[i]);
							i++;
							digit++;
						}
						basic_string<wchar_t>::size_type j = tmpString.length();
						while (j > 0)
						{
							basic_string<wchar_t>::size_type k = 0;
							uint64_t l = 0;
							while (k < 16 && j > 0)
							{
								l = l | (stoull(&tmpString[j - 1], nullptr, 8)) << (3 * k);
								//l = l | ((uint64_t)(tmpString[j - 1] - L'0') << 3 * k);
								tmpString.pop_back();
								j--;
								k++;
							}
							uint32_t m = 1;
							if (*(char*)&m)	//little-endian 0x1032547698ba----
							{
								for (basic_string<wchar_t>::size_type n = 0; (k * 3) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&l)[n]);
								}
							}
							else //big-endian 0x----ba9876543210
							{
								uint32_t o = 0;
								o |= l << 56;
								o |= (l & 0x00000000'0000ff00) << 40;
								o |= (l & 0x00000000'00ff0000) << 24;
								o |= (l & 0x00000000'ff000000) << 8;
								o |= (l & 0x000000ff'00000000) >> 8;
								o |= (l & 0x0000ff00'00000000) >> 24;
								o |= (l & 0x00ff0000'00000000) >> 40;
								o |= l >> 56;
								for (basic_string<wchar_t>::size_type n = 0; (k * 3) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&o)[n]);
								}
							}
						}
					}
					if (input[i] == L'x' || input[i] == L'X')
					{
						i++;
						digit++;
						wstring tmpString = L"";
						while ((input[i] >= L'0' && input[i] <= L'9') || (input[i] >= L'a' && input[i] <= L'f') || (input[i] >= L'A' && input[i] <= L'F'))
						{
							tmpString.push_back(input[i]);
							i++;
							digit++;
						}
						basic_string<wchar_t>::size_type j = tmpString.length();
						while (j > 0)
						{
							basic_string<wchar_t>::size_type k = 0;
							uint64_t l = 0;
							while (k < 16 && j > 0)
							{
								//l = l | (_wtoll(&tmpString[j - 1]) << (4 * k));
								l = l | (stoull(&tmpString[j - 1], nullptr, 16)) << (4 * k);
								//l = l | ((uint64_t)(tmpString[j - 1] - L'0') << 3 * k);
								tmpString.pop_back();
								j--;
								k++;
							}
							uint32_t m = 1;
							if (*(char*)&m)	//little-endian 0x1032547698ba----
							{
								for (basic_string<wchar_t>::size_type n = 0; (k * 4) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&l)[n]);
								}
							}
							else //big-endian 0x----ba9876543210
							{
								uint32_t o = 0;
								o |= l << 56;
								o |= (l & 0x00000000'0000ff00) << 40;
								o |= (l & 0x00000000'00ff0000) << 24;
								o |= (l & 0x00000000'ff000000) << 8;
								o |= (l & 0x000000ff'00000000) >> 8;
								o |= (l & 0x0000ff00'00000000) >> 24;
								o |= (l & 0x00ff0000'00000000) >> 40;
								o |= l >> 56;
								for (basic_string<wchar_t>::size_type n = 0; (k * 4) > (sizeof(wchar_t) * 8 * n); n++)
								{
									tmp.token.push_back(((wchar_t*)&o)[n]);
								}
							}
						}
					}
				}
				if (input[i] == L'\"')
				{
					i++;
					digit++;
					break;
				}
				tmp.token.push_back(input[i]);
				i++;
				digit++;
			}
			output->push_back(tmp);
			continue;
		}
		if (input[i] == L' ' || input[i] == L'\t')	//space and tab
		{
			i++;
			digit++;
			continue;
		}
		if (input[i] == L'\r')	//return
		{
			if ((i + 1) < input.length() && input[i + 1] == L'\n')
			{
				i += 2;
				digit = 1;
				line++;
				continue;
			}
			else
			{
				i++;
				digit = 1;
				line++;
				continue;
			}
		}
		if (input[i] == L'\n')	//linefeed
		{
			i++;
			digit = 1;
			line++;
			continue;
		}
		if (true)	//others
		{
			tmp.filename = filename;
			tmp.line = line;
			tmp.digit = digit;
			while (i < input.length() && input[i] != L' ' && input[i] != L'\r' && input[i] != L'\n' && input[i] != L'\0' && input[i] != L'\t' && input[i] != L'+' && input[i] != L'-' && input[i] != L'*' && input[i] != L'/' && input[i] != L'%' && input[i] != L'|' && input[i] != L'&' && input[i] != L'^' && input[i] != L'~' && input[i] != L'<' && input[i] != L'>' && input[i] != L'!' && input[i] != L'=' && input[i] != L',' && input[i] != L'(' && input[i] != L')')	//not separator nor operand
			{
				if (input[i] == L':')
				{
					tmp.type = $TokenType::Label;
					tmp.token.push_back(input[i]);
					i++;
					digit++;
					break;
				}
				tmp.token.push_back(input[i]);
				i++;
				digit++;
			}
			output->push_back(tmp);
			continue;
		}
	}
	return output;
}