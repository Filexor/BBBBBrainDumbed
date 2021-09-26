#include <string>
#include <fstream>
#include <iostream>

#include <Windows.h>

#include "Parser.h"
#include "BBBBBrainDumbed.h"

using namespace std;

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	wstring exepath, filepath;
	basic_ifstream<wchar_t> ifs;
	if (argc >= 2)
	{
		ifs.open(argv[1]);
		filepath = argv[1];
		if (ifs.fail())
		{
			return 2;
		}

	}
	else
	{
		return 1;
	}
	istreambuf_iterator<wchar_t> ifsbegin(ifs), ifsend;
	wstring finput(ifsbegin, ifsend);
	ifs.close();
	list<Token>* tokens = Tokenizer::tokenize(finput, filepath);
	vector<bool> ROM;
	Parser parser(tokens);
	try
	{
		ROM = parser.parse();
	}
	catch (const ParserError& e)
	{
		wcout << L"Parser error at token:" << e.token.token << L" filename:" << e.token.filename << L" line:" << e.token.line << L" digit:" << e.token.digit << endl << e.what() << endl;
		return 3;
	}
	catch (const runtime_error& e)
	{
		wcout << L"Parser error\n" << e.what() << endl;
		return 4;
	}
	BBBBBrainDumbed b;
	b.memory.bakeRom(ROM);
	LARGE_INTEGER qpc0, qpc1, qpf;
	QueryPerformanceFrequency(&qpf);
	QueryPerformanceCounter(&qpc0);
	for (size_t i = 0; i < 6000; i++)
	{
		b.P = 0;
		b.execute(117314, false);
	}
	QueryPerformanceCounter(&qpc1);
	wcout << L"P=" << b.P << endl;
	wcout << (double)(qpc1.QuadPart - qpc0.QuadPart) / qpf.QuadPart << endl;
	return 0;
}