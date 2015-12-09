#ifndef __UtilityFuncs_h__
#define __UtilityFuncs_h__

#include "stdio.h"
#include "InputDefs.h"

void PrintInputString(const CInputString * inputStr, const char isNumber, FILE * f);
void ReadInputFile(const char * fName, CInputStringSet * inputStrs);
void PrintInputStrSet2File(FILE * f, const CInputStringSet * inputStrs, int startSeq, int endSeq);

int CompareTwoCompactMotifs(const CCompactMotif * m1, const CCompactMotif * m2);
unsigned int RemoveDupCompMotifsInOrderArray(CCompactMotif * motifs, 
											 const unsigned int numMotifs);

int CompareTwoMotifs(const CMotif * m1, const CMotif * m2);
unsigned int RemoveDupMotifsInOrderArray(CMotif * motifs, const unsigned int numMotifs);

int HammingDistStrStr(const char * str1, const char * str2, const  int len);
int HammingDistStrInputStrAtPos(const char * str,  int strLen, const CInputString * inputStr, 
								const  int pos);
int HammingDistStrInputStr(const char * str,  int strLen, const CInputString * inputStr);

char IsMotifInputStr(const char * motif, const int mLen, const int hd,
					 const CInputString * inputStr);
char IsMotifInputStrSet(const char * motif, const  int mLen, const  int hd,
					    const CInputStringSet * inputStrSet, const int loStr, const int hiStr);
char IsMotifInputStrSetWithBindingSite(const char * motif, const  int mLen, const  int hd,
					    const CInputStringSet * inputStrSet, const int loStr, const int hiStr);
int MaxHammingDistFrom1StrToNStrs(const char * str, const char ** strs, 
								  const int numStrs, const int strLen);


#endif