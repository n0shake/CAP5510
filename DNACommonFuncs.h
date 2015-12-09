#ifndef __DNACommonFuncs_h__
#define __DNACommonFuncs_h__

#include <stdio.h>

void ComputeDNAByteTable();
void ComputeDNAByteTableInverse();
void EncodeDNAString(const char * str, const int strLen, char * strEncoded);
void DecodeDNAString(const char * strEncoded, const int strEncodedLen, char * str);
char MapDNALetterToNumber(char letter);
char MapNumberToDNALetter(char number);
void MapDNAStringFromLetterToNumber(char * str, const  int len);
void MapDNAStringFromNumberToLetter(char * str, const  int len);
void PrintDNAString(const char * str, const  int len, 
					const char strMode, const char printMode, FILE * f);


#endif