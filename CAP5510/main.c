#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "InputDefs.h"
#include "DNACommonFuncs.h"
#include "BitFuncs.h"
#include "SortFuncs.h"
#include "UtilityFuncs.h"
#include "PMSPrune.h"


//global input parameters
int motifLen = 11;
int hammingDist = 3;
CInputStringSet inputStrs;
CCompactMotif foundMotifs[CONST_MAX_NUM_FOUND_MOTIFS_ALLOWED];
int numFoundMotifs = 0;

int VerifyMotifs( int moLen, int hmDist, const CInputStringSet * inptStrs,
				  const CCompactMotif * foMotifs, int nFoMotifs) {
	int i;
	int nCorrectMotifs = 0;
	char aMotif[CONST_MAX_MOTIF_STRING_LENGTH + 4];
	for (i = 0; i < nFoMotifs; i++) {
		DecodeDNAString((char *)&foMotifs[i], moLen/4 + 1, aMotif);
		//fprintf(stdout, "\nTest: ");
		//PrintDNAString(aMotif, moLen, 1, 0, stdout);
		if (IsMotifInputStrSet(aMotif, moLen, hmDist, inptStrs, 0, inptStrs->m_num - 1)) {
		//if (IsMotifInputStrSetWithBindingSite(aMotif, moLen, hmDist, inptStrs, 0, inptStrs->m_num - 1)) {
			nCorrectMotifs++;
		} else {

		}
	}
	return nCorrectMotifs;
}


void PrintMotitsToFile(const char * fName, int moLen, CCompactMotif * foMotifs, int nMotifs) {
	int i;
	char aMotif[CONST_MAX_MOTIF_STRING_LENGTH];
	FILE * f;
	f = fopen(fName, "w+");
	if (f == NULL) {
		fprintf(stdout, "\nUnable to create output file %s", fName);
		return;
	}
	//fprintf(stdout, "#Motifs=%d\n", nMotifs);
	for (i = 0; i < nMotifs; i++) {
		DecodeDNAString(foMotifs + i, moLen/4 + 1, aMotif);		
		PrintDNAString(aMotif, moLen, 1, 0, f);
		fprintf(f, "\n");
	}
	//fprintf(stdout, "#Motifs=%d", nMotifs);
	fclose(f);	
}

void main(int argc, char *argv[]) {

	int t1, t2;
	int i, j;	
	char iFileName[500];
	char oFileName[500];

	ComputeDNAByteTable();
	ComputeDNAByteTableInverse();

	motifLen = 13;
	hammingDist = 4;
	sprintf(iFileName, "TestingInput\\input_l%d_d%d_0.txt", motifLen, hammingDist);
	sprintf(oFileName, "output_l%d_d%d_0.txt", motifLen, hammingDist);
		
//	if (argc < 5) {
//		fprintf(stdout, "\nWrong parameters.\nCommand line: \nPMSPrune.exe [input file] [output file] [motif length] [d]");
//		return;
//	} else {
//		strcpy(iFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/input_example.txt");
//		strcpy(oFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/Abhishek.txt");
//		motifLen = atoi(argv[3]);
//		hammingDist = atoi(argv[4]);		
//	}
    
    strcpy(iFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/input_example.txt");
    strcpy(oFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/SampleAbhishek.txt");
    motifLen = 9; //should be dynamically asking for motif length. View original source for reference
    hammingDist = 4; // should be dynamically asking for hamming distance. View original code for reference

	ReadInputFile(iFileName, &inputStrs); //passes file name and gets a pointer to sorted input string

	fprintf(stdout, "\n\nInput Strings: %d strings l=%d d=%d", inputStrs.m_num, motifLen, hammingDist);
	for (i = 0; i < inputStrs.m_num; i++) {	
		fprintf(stdout, "\n\n%d\n", i + 1);
		PrintInputString(&inputStrs.m_str[i], 0, stdout);
	}

	t1 = time(NULL);
	numFoundMotifs = PMSPrune(motifLen, hammingDist, &inputStrs, foundMotifs, CONST_MAX_NUM_FOUND_MOTIFS_ALLOWED);
	t2 = time(NULL);
	fprintf(stdout, "\nTime=%d seconds", t2 - t1);

	PrintMotitsToFile(oFileName, motifLen, foundMotifs, numFoundMotifs);

	return;
}




