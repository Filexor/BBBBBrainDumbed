#pragma once
#include <stdint.h>
#include <bit>

class BBBBBrainDumbed
{
public:
	uint16_t A = 0, B = 0, D = 0, E = 0, F = 0, G = 0, K = 0, P = 0, T1 = 0, T2 = 0, V = 0, H = 0, L = 0;
	uint16_t *OP1 = &A, *OP2 = &A;
	uint8_t I = 0, J = 0, inst = 0;
	bool C = false, M = false;
	BBBBBrainDumbed();
	~BBBBBrainDumbed();
	size_t execute(size_t count);
private:

};

BBBBBrainDumbed::BBBBBrainDumbed()
{
}

BBBBBrainDumbed::~BBBBBrainDumbed()
{
}

size_t BBBBBrainDumbed::execute(size_t count)
{
	return size_t();
}
