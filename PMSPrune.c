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
//#include "PMS5.h"
#include "PMSPrune.h"

typedef int CPosMat[CONST_MAX_NUM_STRINGS][CONST_MAX_INPUT_STRING_LENGTH];
typedef int CPosLen[CONST_MAX_NUM_STRINGS];

CPosMat posMatPMSPrune;
CPosLen posLenPMSPrune;
CCompactMotif * curFMotifsPMSPrune;
CCompactMotif * endFMotifsPMSPrune;

int numNodesPMSPrune = 0;

//pre-computed variables
//its value is in {-1, 0, 1}
char DiffHammingDistTablePMSPrune[4][4][4];

void ComputeHammingDistTable() {
	int i,j,k;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				if (i == k && j == k) {
					DiffHammingDistTablePMSPrune[i][j][k] = 0;
				}
				if (i == k && j != k) {
					DiffHammingDistTablePMSPrune[i][j][k] = -1;
				}
				if (i != k && j == k) {
					DiffHammingDistTablePMSPrune[i][j][k] = 1;
				}
				if (i != k && j != k) {
					DiffHammingDistTablePMSPrune[i][j][k] = 0;
				}
			}
		}
	}
}

void PMSPruneRecursive(int mLen, int hammingDist, const CInputStringSet * inputStrs,
					   const char * rootStr, char * currentStr, 
					   int pos, int depth, CHammingDistMat * distMat) {


	char isWrong = 0;

	int i, j, maxPos;
	int minDist;
	int dist = 0;

	CHammingDistMat * currentMat, * parrentMat;
	char * ptrRowMat, *ptrRowMatMax;
	char * ptrInputStr;
	int * posRowInput;
	char * ptrHamingDistCache = DiffHammingDistTablePMSPrune[currentStr[pos]][rootStr[pos]];
	
	numNodesPMSPrune++;

	if (curFMotifsPMSPrune >= endFMotifsPMSPrune) {
		return;
	}

	if (depth == 0) {
		for ( i = 1; i < inputStrs->m_num; i++ ) {			
			minDist = 255;
			//maxPos = inputStrs->m_str[i].m_length - mLen;
			maxPos = posLenPMSPrune[i];
			for ( j = 0; j < maxPos; j++ ) {
				distMat[depth][i][j] = HammingDistStrInputStrAtPos(rootStr, mLen, &inputStrs->m_str[i], j);
				distMat[depth][i][j] = HammingDistStrInputStrAtPos(rootStr, mLen, &inputStrs->m_str[i], posMatPMSPrune[i][j]);
				if ( minDist > distMat[depth][i][j] ) {
					minDist = distMat[depth][i][j];
				}
			}
			distMat[depth][i][maxPos] = minDist;
			if (dist < minDist) {
				dist = minDist;
			}
		}		
	} else {
		if (depth == hammingDist) {

			parrentMat = distMat + depth - 1;
			for ( i = 1; i < inputStrs->m_num; i++ ) {
				ptrRowMat = (*parrentMat)[i]; 
				//ptrRowMatMax = ptrRowMat + inputStrs->m_str[i].m_length - mLen;
				ptrRowMatMax = ptrRowMat + posLenPMSPrune[i];
				if (*ptrRowMatMax < hammingDist) {
					continue;
				}
				ptrInputStr = inputStrs->m_str[i].m_data + pos;
				posRowInput = (int *)posMatPMSPrune[i];
				for ( ; ptrRowMat < ptrRowMatMax; ptrRowMat++ ) {					
					if ( (*ptrRowMat) + ptrHamingDistCache[*(ptrInputStr + (*posRowInput))] <= hammingDist) {
					//if ( (*ptrRowMat) + ptrHamingDistCache[*(ptrInputStr)] <= hammingDist) {
						break;
					}
					//ptrInputStr++;
					posRowInput++;
				}
				//prune
				if (ptrRowMat >= ptrRowMatMax) {
					return;
				}
			}
			//motif found
			//fprintf(stdout, "\ndepth=%d ", depth);
			//PrintDNAString(currentStr, mLen, 1, 0, stdout);
			if (curFMotifsPMSPrune < endFMotifsPMSPrune) {
				EncodeDNAString(currentStr, mLen, (char *)curFMotifsPMSPrune);
				curFMotifsPMSPrune++;
			}
			return;
		} else {			
			currentMat = distMat + depth;
			parrentMat = currentMat - 1;
			//memcpy(currentMat, parrentMat, sizeof(CHammingDistMat));

			for ( i = 1; i < inputStrs->m_num; i++ ) {
				memcpy((*currentMat)[i], (*parrentMat)[i], posLenPMSPrune[i]);
				minDist = 255;
				ptrRowMat = (*currentMat)[i]; 
				//ptrRowMatMax = ptrRowMat + inputStrs->m_str[i].m_length - mLen;
				ptrRowMatMax = ptrRowMat + posLenPMSPrune[i];
				ptrInputStr = inputStrs->m_str[i].m_data + pos;
				posRowInput = (int *)posMatPMSPrune[i];
				for ( ; ptrRowMat < ptrRowMatMax; ptrRowMat++ ) {
					//*ptrRowMat += ptrHamingDistCache[*(ptrInputStr)];
					*ptrRowMat += ptrHamingDistCache[*(ptrInputStr + (*posRowInput))];
					if ( minDist > *ptrRowMat ) {
						minDist = *ptrRowMat;
					}
					//ptrInputStr++;
					posRowInput++;
				}
				*ptrRowMatMax = minDist;
				//prune
				if (minDist + depth > 2 * hammingDist) {
					//numPrunes++;
					return;
				}
				if (dist < minDist) {
					dist = minDist;
				}
			}
		}
	}

	//motif found
	if (dist <= hammingDist) {
		//fprintf(stdout, "\ndepth=%d ", depth);
		//PrintDNAString(currentStr, mLen, 1, 0, stdout);
		if (curFMotifsPMSPrune < endFMotifsPMSPrune) {
			EncodeDNAString(currentStr, mLen, (char *)curFMotifsPMSPrune);
			curFMotifsPMSPrune++;
		}
	}
	//explore children
	if (depth < hammingDist) {
		for (i = pos + 1; i < mLen; i++) {		
			for (j = 0; j < 4; j++) {
				if ( !( (char)j == rootStr[i]) ) {
					currentStr[i] = j;					
					PMSPruneRecursive(mLen, hammingDist, inputStrs, 
									  rootStr, currentStr, i, depth + 1, distMat);					
				}
			}
			currentStr[i] = rootStr[i];			
		}
	}
}

int PMSPrune(int motifLen, int hammingDist, const CInputStringSet * inputStrs,
			 CCompactMotif * foundMotifs, int maxNumMotifsAllowed) {

	int i, j, k;
	char aMotif[CONST_MAX_MOTIF_STRING_LENGTH];
	char * rootStr;
	char currentStr[CONST_MAX_MOTIF_STRING_LENGTH];
	int numFoundMotifs = 0;
	CHammingDistMat * distMat = malloc(CONST_MAX_HAMMING_DIST * sizeof(CHammingDistMat));

	ComputeHammingDistTable();

	curFMotifsPMSPrune = foundMotifs;
	endFMotifsPMSPrune = foundMotifs + maxNumMotifsAllowed;
	numNodesPMSPrune = 0;

	for (i = 0; i <= inputStrs->m_str[0].m_length - motifLen; i++) {
		rootStr = inputStrs->m_str[0].m_data + i;
		memcpy(currentStr, rootStr, motifLen);
		for (j = 1; j < inputStrs->m_num; j++) {
			posLenPMSPrune[j] = 0;
			for (k = 0; k <= inputStrs->m_str[j].m_length - motifLen; k++) {
				if (HammingDistStrInputStrAtPos(rootStr, motifLen, &inputStrs->m_str[j], k) <= 2 * hammingDist) {
					posMatPMSPrune[j][posLenPMSPrune[j]] = k;
					posLenPMSPrune[j]++;
				}
			}
		}
		PMSPruneRecursive(motifLen, hammingDist, inputStrs, rootStr, currentStr, -1, 0, distMat);
		if (i % 100 == 0) {
			numFoundMotifs = curFMotifsPMSPrune - foundMotifs;
			fprintf(stdout, "\n%d #Duplicate Motifs=%d", i, numFoundMotifs);
		}
	}
	free(distMat);

	//qsort(foundMotifs, curFMotifsPMSPrune - foundMotifs, sizeof(CCompactMotif), CompareTwoCompactMotifs);
	RadixSort((unsigned char *)foundMotifs, curFMotifsPMSPrune - foundMotifs, sizeof(CCompactMotif));
	numFoundMotifs = RemoveSortedDuplicate(foundMotifs, curFMotifsPMSPrune - foundMotifs, sizeof(CCompactMotif));
	fprintf(stdout, "\n#Unduplicate Motifs=%d", numFoundMotifs);

	return numFoundMotifs;
}


int PMSPrune4( int motifLen, int hammingDist, CInputStringSet * inputStrs,
			  CCompactMotif * foundMotifs, int maxNumMotifsAllowed) {

	int i;
	CCompactMotif * fMotifs;
	int nFoMotif = 0;
	int maxNFoMotifs = 2 * 1024 * 1024;
	char aMotif[CONST_MAX_MOTIF_STRING_LENGTH];
	int numFoundMotifs = 0; 

	int numStr = inputStrs->m_num;
	int k = 20;
	if (k > inputStrs->m_num) {
		k = inputStrs->m_num;
	} else {
		inputStrs->m_num = k;
	}

	fMotifs = (CCompactMotif *)malloc(maxNFoMotifs * sizeof(CCompactMotif));
	nFoMotif = PMSPrune(motifLen, hammingDist, inputStrs, fMotifs, maxNFoMotifs);	
	if (nFoMotif >= 512 * 1024) {
		nFoMotif >= 512 * 1024;
	}
	inputStrs->m_num = numStr;
	if (k < inputStrs->m_num) {
		numFoundMotifs = 0;
		for (i = 0; i < nFoMotif; i++) {
			DecodeDNAString((char *)&fMotifs[i], motifLen/4 + 1, aMotif);
			if (IsMotifInputStrSet(aMotif, motifLen, hammingDist, inputStrs, k, inputStrs->m_num - 1)) {
			//if (IsMotifInputStrSet(aMotif, motifLen, hammingDist, inputStrs, 1, inputStrs->m_num - 1)) {
				if (numFoundMotifs < maxNumMotifsAllowed) {
					memcpy(foundMotifs + numFoundMotifs, fMotifs + i, sizeof(CCompactMotif));
					numFoundMotifs++;
				}
			}
		}
	} else {
		numFoundMotifs = nFoMotif;
		if (numFoundMotifs >= maxNumMotifsAllowed) {
			numFoundMotifs = maxNumMotifsAllowed;
		}
		memcpy(foundMotifs, fMotifs, numFoundMotifs * sizeof(CCompactMotif));
	}
	fprintf(stdout, "\n#Unduplicate Motifs = %d", numFoundMotifs);

	free(fMotifs);
	return numFoundMotifs;

}


