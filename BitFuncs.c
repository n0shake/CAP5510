#include <stdio.h>
#include "BitFuncs.h"

void BitWrite(char * c, char pos, char val) {
	char mask = 1;
	//mask 000010000
	mask <<= (7 - pos);
	if (val) {		
		*c |= mask;
	} else {
		//mask 111101111
		*c &= (~mask);		
	}
}

//01234567
char BitRead(char c, char pos) {	
	c >>= (7 - pos);
	return (char)(c & 1);
}

void BitPrint(char c, FILE * f) {
	char i;	
	for (i = 0; i < 8; i++) {
		fprintf(f, "%d", BitRead(c, i));
	}
}

// return 0 if nBit in c1 and c2 are the same, return 1 otherwise
int BitCompare(char c1, char c2, char pos, char nBit) {
	//fprintf(stdout, "\nc1 ");
	//BitPrint(c1, stdout);
	//fprintf(stdout, "\nc2 ");
	//BitPrint(c2, stdout);
	c1 <<= pos;
	c2 <<= pos;
	//fprintf(stdout, "\nc1 ");
	//BitPrint(c1, stdout);
	//fprintf(stdout, "\nc2 ");
	//BitPrint(c2, stdout);
	c1 >>= (8 - nBit);
	c2 >>= (8 - nBit);
	//fprintf(stdout, "\nc1 ");
	//BitPrint(c1, stdout);
	//fprintf(stdout, "\nc2 ");
	//BitPrint(c2, stdout);
	return !(c1 == c2);
}

void BitCopyZeroPadding(char * c1, char c2, char pos, char nBit) {
	//BitPrint(~(255 << (8 - pos)), stdout);
	//BitPrint((255 << (8 - pos - nBit)), stdout);
	*c1 = (~(255 << (8 - pos))) & (255 << (8 - pos - nBit));
	*c1 &= c2;	
}

void BitShift2RightZeroPadding(char * c, char nBit) {
	*c >>= nBit;
	*c &= ~(255 << (8 - nBit));
}

void BitArrayWrite(char * array, unsigned int pos, char val) {
	unsigned int posByte = pos/8;
	BitWrite(array + posByte, pos - 8 * posByte, val);
}

char BitArrayRead(const char * array, unsigned int pos) {
	unsigned int posByte = pos/8;
	return BitRead(array[posByte], pos - 8 * posByte);
}

void SwapMemory(char * mem1, char * mem2, unsigned int len) {
	char * eMem1 = mem1 + len;
	if (mem1 == mem2) {
		return;
	}
	for (; mem1 < eMem1; mem1++, mem2++) {
		//a ^= b;
		//b ^= a;
		//a ^= b;
		*mem1 ^= *mem2;
		*mem2 ^= *mem1;
		*mem1 ^= *mem2;
	}
}

void SwapMemoryWithoutChecking(char * mem1, char * mem2, unsigned int len) {
	char * eMem1 = mem1 + len;
	for (; mem1 < eMem1; mem1++, mem2++) {
		//a ^= b;
		//b ^= a;
		//a ^= b;
		*mem1 ^= *mem2;
		*mem2 ^= *mem1;
		*mem1 ^= *mem2;
	}
}