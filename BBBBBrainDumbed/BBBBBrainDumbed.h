#pragma once
#include <stdint.h>
#include <bit>
#include <bitset>
#include <vector>
#include <stdexcept>

#ifdef __clang__
#define rotr _rotr
#define rotl _rotl
#endif // __clang__


using namespace std;

class Memory
{
public:
	bitset<0x8000> ROM;
	bitset<0x4000> RAM;
	bitset<0x2000> NVRAM;
	bitset<0x100> VREG;
	bitset<0x5> AREG;
	bitset<0x5> controllerInput0;
	bitset<0x5> controllerInput1;
	Memory();
	~Memory();
	void bakeRom(vector<bool> input);
	uint16_t mapAddress(uint16_t input);
	bool read(uint16_t address);
	uint8_t read4(uint16_t address);
	uint8_t read7(uint16_t address);
	uint16_t read16(uint16_t address);
	void write(uint16_t address, bool value);
	void write(uint16_t address, vector<bool> value);
	void write4(uint16_t address, uint8_t value);
	void write16(uint16_t address, uint16_t value);

private:

};

Memory::Memory()
{
}

Memory::~Memory()
{
}

void Memory::bakeRom(vector<bool> input)
{
	if (input.size() > 0x8000)
	{
		throw out_of_range("Input is too large.");
	}
	for (size_t i = 0; i < input.size(); i++)
	{
		ROM[i] = input[i];
	}
}

uint16_t Memory::mapAddress(uint16_t input)
{
	uint16_t output = input;
	if (input >= 0xe100 && input <= 0xefff)	//VREG
	{
		//1100 0000 0001 1111
		//1101 1111 1111 1111
		output = input & 0xe0ff;
	}
	else if (input >= 0xf008 && input <= 0xf7ff)	//AREG
	{
		//1110 0000 0000 0111
		//1110 1111 1111 1111
		output = input & 0xf007;
	}
	else if (input >= 0xf810 && input <= 0xffff)	//Peripheral
	{
		//1111 0000 0000 1111
		//1111 0111 1111 1111
		output = input & 0xf80f;
	}
	return output;
}

bool Memory::read(uint16_t address)
{
	uint16_t i = mapAddress(address);
	if (i <= 0x7fff)
	{
		return ROM[i];
	}
	else if (i <= 0xbfff)
	{
		return RAM[i - 0x8000];
	}
	else if (i <= 0xdfff)
	{
		return NVRAM[i - 0xc000];
	}
	else if (i <= 0xefff)
	{
		return VREG[i - 0xe000];
	}
	else if (i <= 0xf004)
	{
		return AREG[i - 0xf000];
	}
	else if (i <= 0xf7ff)
	{
		return true;
	}
	else if (i <= 0xf800)
	{
		return controllerInput0[i - 0xf800];
	}
	else if (i <= 0xf807)
	{
		return true;
	}
	else if (i <= 0xf80c)
	{
		return controllerInput1[i - 0xf808];
	}
	else
	{
		return true;
	}
}

uint8_t Memory::read4(uint16_t address)
{
	uint8_t out = 0;
	for (uint8_t i = 0; i < 4; i++)
	{
		out |= read(address + i) << i;
	}
	return out;
}

uint8_t Memory::read7(uint16_t address)
{
	uint8_t out = 0;
	for (uint8_t i = 0; i < 7; i++)
	{
		out |= read(address + i) << i;
	}
	return out;
}

uint16_t Memory::read16(uint16_t address)
{
	uint16_t out = 0;
	for (uint8_t i = 0; i < 16; i++)
	{
		out |= read(address + i) << i;
	}
	return out;
}

void Memory::write(uint16_t address, bool value)
{
	uint16_t i = mapAddress(address);
	if (i <= 0x7fff)
	{
		ROM[i] = value;
	}
	else if (i <= 0xbfff)
	{
		RAM[i - 0x8000] = value;
	}
	else if (i <= 0xc016)
	{
		VREG[i - 0xc000] = value;
	}
	else if (i <= 0xdfff)
	{
		return;
	}
	else if (i <= 0xe004)
	{
		AREG[i - 0xe000] = value;
	}
	else if (i <= 0xefff)
	{
		return;
	}
	else if (i <= 0xf004)
	{
		controllerInput0[i - 0xf000] = value;
	}
	else if (i <= 0xf007)
	{
		return;
	}
	else if (i <= 0xf00c)
	{
		controllerInput1[i - 0xf008] = value;
	}
	else
	{
		return;
	}
}

void Memory::write(uint16_t address, vector<bool> value) {
	for (uint8_t i = 0; i < value.size(); i++)
	{
		write(address + i, value[i]);
	}
}

void Memory::write4(uint16_t address, uint8_t value) {
	for (uint8_t i = 0; i < 4; i++)
	{
		write(address + i, (bool)((value >> i) & 1));
	}
}

void Memory::write16(uint16_t address, uint16_t value) {
	for (uint8_t i = 0; i < 16; i++)
	{
		write(address + i, (bool)((value >> i) & 1));
	}
}

class BBBBBrainDumbed
{
public:
	uint32_t T3 = 0;
	uint16_t A = 0, B = 0, D = 0, E = 0, F = 0, G = 0, K = 0, P = 0, T1 = 0, T2 = 0, V = 0, H = 0, L = 0;
	uint16_t *OP1 = &A, *OP2 = &A;
	uint8_t I = 0, J = 0, inst = 0;
	bool C = false, M = false, IRQ = false;
	Memory memory;
	BBBBBrainDumbed();
	~BBBBBrainDumbed();
	size_t execute(size_t count, bool isInit);
	void checkIRQ();
private:

};

BBBBBrainDumbed::BBBBBrainDumbed()
{
}

BBBBBrainDumbed::~BBBBBrainDumbed()
{
}

size_t BBBBBrainDumbed::execute(size_t count, bool isInit)
{
	size_t tick = 0;
	size_t inst_count = 0;
	if (isInit)
	{
		tick++;
		isInit = false;
	}
	while (count > tick)
	{
		inst = memory.read7(P);
		P += 7;
		switch (inst)
		{
		case 0:
			OP1 = &A;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 1:
			OP1 = &B;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 2:
			OP1 = &D;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 3:
			OP1 = &E;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 4:
			OP1 = &F;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 5:
			OP1 = &G;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 6:
			OP1 = &K;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 7:
			OP1 = &P;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 8:
			OP2 = &A;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 9:
			OP2 = &B;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 10:
			OP2 = &D;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 11:
			OP2 = &E;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 12:
			OP2 = &F;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 13:
			OP2 = &G;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 14:
			OP2 = &K;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 15:
			OP2 = &P;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 16:	//mov.1
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfffe) | (T2 & 0x1);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 17:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfffe) | (~T2 & 0x1);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 18:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 | (T2 & 0x1);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 19:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 & (T2 | 0xfffe);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 20:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfffe) | ((T1 ^ T2) & 0x1);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 21:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 << (T2 & 0xf);
			*OP1 = rotl(T1, I);
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 22:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 >> (T2 & 0xf);
			*OP1 = rotl(T1, I);
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 23:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (uint16_t)(((int16_t)T1) >> (T2 & 0xf));
			*OP1 = rotl(T1, I);
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 24:
			T1 = rotr(*OP2, I);
			*OP1 = rotl(*OP1, T1 & 0xf);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 25:
			T1 = rotr(*OP2, I);
			*OP1 = rotr(*OP1, T1 & 0xf);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 26:	//adc.1
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T2 = (T1 & 0x1) + (T2 & 0x1) + C;
			T1 = (T1 & 0xfffe) | (T2 & 0x1);
			C = (T2 >> 1) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 27:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T2 = (T1 & 0x1) - (T2 & 0x1) - C;
			T1 = (T1 & 0xfffe) | (T2 & 0x1);
			C = (T2 >> 1) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 28:
			T1 = rotr(*OP2, I);
			T2 = (T1 & 0xf) + 1;
			T1 = (T1 & 0xfff0) | (T2 & 0xf);
			C = (T2 >> 4) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 29:
			T1 = rotr(*OP2, I);
			T3 = T1 + 1;
			T1 = T3 & 0xffff;
			C = (T3 >> 16) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 34;
			inst_count++;
			checkIRQ();
			break;
		case 30:
			T1 = rotr(*OP2, I);
			T2 = (T1 & 0xf) - 1;
			T1 = (T1 & 0xfff0) | (T2 & 0xf);
			C = (T2 >> 4) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 31:
			T1 = rotr(*OP2, I);
			T3 = T1 - 1;
			T1 = T3 & 0xffff;
			C = (T3 >> 16) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 34;
			inst_count++;
			checkIRQ();
			break;
		case 32:	//mov.4
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfff0) | (T2 & 0xf);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 33:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfff0) | (~T2 & 0xf);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 34:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 | (T2 & 0xf);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 35:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = T1 & (T2 | 0xfff0);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 36:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfff0) | ((T1 ^ T2) & 0xf);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 37:
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 38:
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 39:
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 40:
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 41:
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 42:	//adc.4
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T2 = (T1 & 0xf) + (T2 & 0xf) + C;
			T1 = (T1 & 0xfff0) | (T2 & 0xf);
			C = (T2 >> 4) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 43:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T2 = (T1 & 0xf) - (T2 & 0xf) - C;
			T1 = (T1 & 0xfff0) | (T2 & 0xf);
			C = (T2 >> 4) & 0x1;
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 32;
			inst_count++;
			checkIRQ();
			break;
		case 44:	//mul.4
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T3 = (uint32_t)((T1 & 0xf) * (T2 & 0xf));
			H = T3 >> 16;
			L = T3 & 0xffff;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 45:	//muls.4
			T1 = (((int8_t)rotr(*OP1, I)) << 4) >> 4;
			T2 = (((int8_t)rotr(*OP2, I)) << 4) >> 4;
			T3 = (int32_t)(T1 * T2);
			H = T3 >> 16;
			L = T3 & 0xffff;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 46:	//div.4
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			L = (T1 & 0xf) / (T2 & 0xf);
			H = (T1 & 0xf) % (T2 & 0xf);
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 47:	//divs.4
			T1 = (((int8_t)rotr(*OP1, I)) << 4) >> 4;
			T2 = (((int8_t)rotr(*OP2, I)) << 4) >> 4;
			if (T2 == 0)
			{
				L = 0;
				H = 0;
			}
			else
			{
				L = (int16_t)T1 / (int16_t)T2;
				H = (int16_t)T1 % (int16_t)T2;
			}
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 48:	//mov.16
			*OP1 = *OP2;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 49:
			*OP1 = ~(*OP2);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 50:
			*OP1 = (*OP1) | (*OP2);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 51:
			*OP1 = (*OP1) & (*OP2);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 52:
			*OP1 = (*OP1) ^ (*OP2);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 53:	//mfh
			*OP1 = rotl(H, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 54:
			*OP1 = rotl(L, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 55:
			*OP1 = 0;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 56:
			T1 = rotr(*OP2, I);
			T1 = -T1;
			*OP1 = rotl(T1, I);
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 57:	//nop
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 58:	//adc.16
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T3 = T1 + T2 + C;
			T1 = T3 & 0xffff;
			C = (T3 >> 16) & 0x1;
			*OP1 = rotl(T1, I);
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 59:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T3 = T1 - T2 - C;
			T1 = T3 & 0xffff;
			C = (T3 >> 16) & 0x1;
			*OP1 = rotl(T1, I);
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 60:
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			T3 = (uint32_t)(T1) * (uint32_t)(T2);
			H = T3 >> 16;
			L = T3 & 0xffff;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 61:
			T1 = (int16_t)rotr(*OP1, I);
			T2 = (int16_t)rotr(*OP2, I);
			T3 = (int32_t)(T1) * (int32_t)(T2);
			H = T3 >> 16;
			L = T3 & 0xffff;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 62:	//div.16
			T1 = rotr(*OP1, I);
			T2 = rotr(*OP2, I);
			if (T2 == 0)
			{
				L = 0;
				H = 0;
			}
			else
			{
				L = T1 / T2;
				H = T1 % T2;
			}
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 63:
			T1 = (int16_t)rotr(*OP1, I);
			T2 = (int16_t)rotr(*OP2, I);
			L = T1 / T2;
			H = T1 % T2;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 64:	//ldi.4 0
		case 65:
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
		case 76:
		case 77:
		case 78:
		case 79:	//ldi.4 15
			T1 = rotr(*OP1, I);
			T1 = (T1 & 0xfff0) | (inst & 0xf);
			*OP1 = rotl(T1, I);
			I = (I + 4) & 0xf;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 80:	//ldr.1
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfffe) | (memory.read(T2) & 0x1);
			*OP1 = rotl(T1, J);
			J = (J+ 1) & 0xf;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 81:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfffe) | (memory.read(T2) & 0x1);
			T2++;
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			J = (J+ 1) & 0xf;
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 82:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T2--;
			T1 = (T1 & 0xfffe) | (memory.read(T2) & 0x1);
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			J = (J+ 1) & 0xf;
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 83:	//str.1
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write(T2, T1 & 0x1);
			J = (J+ 1) & 0xf;
			tick += 31;
			inst_count++;
			checkIRQ();
			break;
		case 84:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write(T2, T1 & 0x1);
			T2++;
			*OP2 = rotl(T2, I);
			J = (J+ 1) & 0xf;
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 85:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T2--;
			memory.write(T2, T1 & 0x1);
			*OP2 = rotl(T2, I);
			J = (J+ 1) & 0xf;
			tick += 35;
			inst_count++;
			checkIRQ();
			break;
		case 86:	//cli
			I = 0;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 87:
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 88:
			I = (I + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 89:
			*OP1 = I & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 90:
			I = *OP2 & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 91:
			J = 0;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 92:
			J = (J + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 93:
			J = (J + 4) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 94:
			T1 = J & 0xf;
			*OP1 = rotl(T1, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 95:
			J = rotr(*OP2, I) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 96:	//ldr.4
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfff0) | memory.read4(T2);
			*OP1 = rotl(T1, J);
			J = (J + 4) & 0xf;
			tick += 46;
			inst_count++;
			checkIRQ();
			break;
		case 97:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T1 = (T1 & 0xfff0) | memory.read4(T2);
			T2 += 4;
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			J = (J + 4) & 0xf;
			tick += 49;
			inst_count++;
			checkIRQ();
			break;
		case 98:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T2 -= 4;;
			T1 = (T1 & 0xfff0) | memory.read4(T2);
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			J = (J + 4) & 0xf;
			tick += 50;
			inst_count++;
			checkIRQ();
			break;
		case 99:	//str.4
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write4(T2, T1 & 0xf);
			J = (J + 4) & 0xf;
			tick += 46;
			inst_count++;
			checkIRQ();
			break;
		case 100:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write4(T2, T1 & 0xf);
			T2 += 4;
			*OP2 = rotl(T2, I);
			J = (J + 4) & 0xf;
			tick += 49;
			inst_count++;
			checkIRQ();
			break;
		case 101:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T2 -= 4;
			memory.write4(T2, T1 & 0xf);
			*OP2 = rotl(T2, I);
			J = (J + 4) & 0xf;
			tick += 50;
			inst_count++;
			checkIRQ();
			break;
		case 102:	//clc
			C = false;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 103:
			C = true;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 104:
			T1 = rotr(*OP1, I);
			T1 = (T1 & 0xfffe) | (C & 0x1);
			*OP1 = rotl(T1, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 105:
			M = false;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 106:
			M = true;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 107:
			T1 = rotr(*OP1, I);
			T1 = (T1 & 0xfffe) | (M & 0x1);
			*OP1 = rotl(T1, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 108:
			*OP1 = rotl(V, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 109:
			V = rotr(*OP1, I);
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 110:
			T1 = *OP1;
			T1 = ((T1 & 0x5555) << 1) | ((T1 & 0xAAAA) >> 1);
			T1 = ((T1 & 0x3333) << 2) | ((T1 & 0xCCCC) >> 2);
			T1 = ((T1 & 0x0F0F) << 4) | ((T1 & 0xF0F0) >> 4);
			T1 = ((T1 & 0x00FF) << 8) | ((T1 & 0xFF00) >> 8);
			*OP1 = T1;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 111:
			T1 = *OP1;
			*OP1 = *OP2;
			*OP2 = T1;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 112:	//ldr.16
			T2 = rotr(*OP2, I);
			T1 = memory.read16(T2);
			*OP1 = rotl(T1, J);
			tick += 106;
			inst_count++;
			checkIRQ();
			break;
		case 113:
			T2 = rotr(*OP2, I);
			T1 = memory.read16(T2);
			T2 += 16;
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			tick += 109;
			inst_count++;
			checkIRQ();
			break;
		case 114:
			T2 = rotr(*OP2, I);
			T2 -= 16;
			T1 = memory.read16(T2);
			*OP1 = rotl(T1, J);
			*OP2 = rotl(T2, I);
			tick += 110;
			inst_count++;
			checkIRQ();
			break;
		case 115:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write16(T2, T1);
			tick += 106;
			inst_count++;
			checkIRQ();
			break;
		case 116:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			memory.write16(T2, T1);
			T2 += 16;
			*OP2 = rotl(T2, I);
			tick += 109;
			inst_count++;
			checkIRQ();
			break;
		case 117:
			T1 = rotr(*OP1, J);
			T2 = rotr(*OP2, I);
			T2 -= 16;
			memory.write16(T2, T1);
			*OP2 = rotl(T2, I);
			tick += 110;
			inst_count++;
			checkIRQ();
			break;
		case 118:	//bcc
			if (C == false)
			{
				P = rotr(*OP2, I);
			}
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 119:
			if (C == false)
			{
				T1 = rotr(*OP2, I);
				*OP2 = rotl(P, I);
				P = T1;
			}
			tick += 30;
			inst_count++;
			checkIRQ();
			break;
		case 120:
			if (*OP1 == 0)
			{
				P = rotr(*OP2, I);
			}
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 121:
			if (*OP1 == 0)
			{
				T1 = rotr(*OP2, I);
				*OP2 = rotl(P, I);
				P = T1;
			}
			tick += 30;
			inst_count++;
			checkIRQ();
			break;
		case 122:	//bn
			if ((rotr(*OP1, I) & 0x8000) != 0)
			{
				P = rotr(*OP2, I);
			}
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		case 123:
			if ((rotr(*OP1, I) & 0x8000) != 0)
			{
				T1 = rotr(*OP2, I);
				*OP2 = rotl(P, I);
				P = T1;
			}
			tick += 30;
			inst_count++;
			checkIRQ();
			break;
		case 124:	//wait.4
			T1 = rotr(*OP1, I) & 0xf;
			tick += (30 + T1);
			inst_count++;
			checkIRQ();
			break;
		case 125:	//wait.4e
			T1 = rotr(*OP1, I) & 0xf;
			tick += (30 + T1 + 16);
			inst_count++;
			checkIRQ();
			break;
		case 126:
		case 127:
			T1 = rotr(*OP1, I);
			T1 = (T1 & 0xfffe) | (inst & 0x1);
			*OP1 = rotl(T1, I);
			I = (I + 1) & 0xf;
			tick += 29;
			inst_count++;
			checkIRQ();
			break;
		default:
			break;
		}
	}
	return tick - count;
}

void BBBBBrainDumbed::checkIRQ()
{
	if (!M && IRQ)
	{
		T1 = P;
		P = V;
		V = T1;
	}
}