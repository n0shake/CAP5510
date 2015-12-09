#include <stdio.h>
#include "BitFuncs.h"
#include "DNACommonFuncs.h"

char DNAByteTable[4][4][4][4];
char DNAByteTableInverse[256][4];

void ComputeDNAByteTable() {
	char i0, i1, i2, i3;
	char compactCode;
	for (i0 = 0; i0 < 4; i0++) {
		for (i1 = 0; i1 < 4; i1++) {
			for (i2 = 0; i2 < 4; i2++) {
				for (i3 = 0; i3 < 4; i3++) {
					compactCode = 4*4*4*i0 + 4*4*i1 + 4*i2 + i3;
					DNAByteTable[i0][i1][i2][i3] = (char)compactCode;
				}
			}
		}
	}
}

void ComputeDNAByteTableInverse() {
	int i;
	char ch;
	for (i = 0; i < 256; i++) {
		ch = i;
		if ( !BitRead(ch,0) && !BitRead(ch,1) ) {
			DNAByteTableInverse[i][0] = 0;
		}
		if ( !BitRead(ch,0) && BitRead(ch,1) ) {
			DNAByteTableInverse[i][0] = 1;
		}
		if ( BitRead(ch,0) && !BitRead(ch,1) ) {
			DNAByteTableInverse[i][0] = 2;
		}
		if ( BitRead(ch,0) && BitRead(ch,1) ) {
			DNAByteTableInverse[i][0] = 3;
		}

		if ( !BitRead(ch,2) && !BitRead(ch,3) ) {
			DNAByteTableInverse[i][1] = 0;
		}
		if ( !BitRead(ch,2) && BitRead(ch,3) ) {
			DNAByteTableInverse[i][1] = 1;
		}
		if ( BitRead(ch,2) && !BitRead(ch,3) ) {
			DNAByteTableInverse[i][1] = 2;
		}
		if ( BitRead(ch,2) && BitRead(ch,3) ) {
			DNAByteTableInverse[i][1] = 3;
		}
		
		if ( !BitRead(ch,4) && !BitRead(ch,5) ) {
			DNAByteTableInverse[i][2] = 0;
		}
		if ( !BitRead(ch,4) && BitRead(ch,5) ) {
			DNAByteTableInverse[i][2] = 1;
		}
		if ( BitRead(ch,4) && !BitRead(ch,5) ) {
			DNAByteTableInverse[i][2] = 2;
		}
		if ( BitRead(ch,4) && BitRead(ch,5) ) {
			DNAByteTableInverse[i][2] = 3;
		}

		if ( !BitRead(ch,6) && !BitRead(ch,7) ) {
			DNAByteTableInverse[i][3] = 0;
		}
		if ( !BitRead(ch,6) && BitRead(ch,7) ) {
			DNAByteTableInverse[i][3] = 1;
		}
		if ( BitRead(ch,6) && !BitRead(ch,7) ) {
			DNAByteTableInverse[i][3] = 2;
		}
		if ( BitRead(ch,6) && BitRead(ch,7) ) {
			DNAByteTableInverse[i][3] = 3;
		}
	}
}
void EncodeDNAString(const char * str, const int strLen, char * strEncoded) {
	int nUnproceedLetters;
	const char * strE = str + strLen - 4;
	for ( ;str <= strE; str += 4) {		
		*strEncoded = DNAByteTable[str[0]][str[1]][str[2]][str[3]];
		strEncoded++;	
	}
	nUnproceedLetters = strE - str + 4;
	if (nUnproceedLetters == 1) {
		*strEncoded = DNAByteTable[str[0]][0][0][0];
	} else if (nUnproceedLetters == 2) {
		*strEncoded = DNAByteTable[str[0]][str[1]][0][0];
	} else if (nUnproceedLetters == 3) {
		*strEncoded = DNAByteTable[str[0]][str[1]][str[2]][0];
	}	
}
void DecodeDNAString(const char * strEncoded, const int strEncodedLen, char * str) {
	int i;	
	for (i = 0; i < strEncodedLen; i++, str += 4) {
		memcpy(str, DNAByteTableInverse[(unsigned char)strEncoded[i]], 4);
	}
}
char MapDNALetterToNumber(char letter) {
	if ( letter == 'A' || letter == 'a' ) return 0;
	if ( letter == 'C' || letter == 'c' ) return 1;
	if ( letter == 'G' || letter == 'g' ) return 2;
	if ( letter == 'T' || letter == 't') return 3;
	return 0;
}
char MapNumberToDNALetter(char number) {
	if ( number == 0 ) return 'A';
	if ( number == 1 ) return 'C';
	if ( number == 2 ) return 'G';
	if ( number == 3 ) return 'T';
	return 'A';
}
void MapDNAStringFromLetterToNumber(char * str, const  int len) {
	const char * ptrE = str + len;
	for (; str < ptrE; str++) {
		*str = MapDNALetterToNumber(*str);
	}
}
void MapDNAStringFromNumberToLetter(char * str, const  int len) {
	const char * ptrE = str + len;
	for (; str < ptrE; str++) {
		*str = MapNumberToDNALetter(*str);
	}
}
void PrintDNAString(const char * str, const  int len, 
					const char strMode, const char printMode, FILE * f) {
	const char * ptrE = str + len;
	if (strMode == 1) {
		if (printMode) {
			for (; str < ptrE; str++) {
				fprintf(f, "%d", *str);
			}
		} else {
			for (; str < ptrE; str++) {
				fprintf(f, "%c", MapNumberToDNALetter(*str));
			}
		}
	} else {
		if (printMode) {
			for (; str < ptrE; str++) {
				fprintf(f, "%d", MapDNALetterToNumber(*str));
			}
		} else {
			for (; str < ptrE; str++) {
				fprintf(f, "%c", *str);
			}
		}
	}
}
