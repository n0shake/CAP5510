#ifndef __BitFuncs_h__
#define __BitFuncs_h__

#include <stdio.h>

void BitPrint(char c, FILE * f);
void BitWrite(char * c, char pos, char val);
char BitRead(char c, char pos);
int BitCompare(char c1, char c2, char pos, char nBit);
void BitCopyZeroPadding(char * c1, char c2, char pos, char nBit);
void BitShift2RightZeroPadding(char * c, char nBit);
void BitArrayWrite(char * c, unsigned int pos, char val);
char BitArrayRead(const char * c, unsigned int pos);
void SwapMemory(char * mem1, char * mem2, unsigned int len);
void SwapMemoryWithoutChecking(char * mem1, char * mem2, unsigned int len);

#endif