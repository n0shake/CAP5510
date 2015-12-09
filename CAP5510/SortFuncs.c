#include <stdlib.h>
#include <stdio.h>
#include "BitFuncs.h"
#include "SortFuncs.h"

/*
char * BinarySearch(char * array, int nKey, int keyLen, const char * key) {	
	int cmp;
	//int i;
	//const char * bArray = array;
	while (nKey > 0) {
		nKey /= 2;
		//fprintf(stdout, "\nnKey=%d \nminPos=%3d midPos=%3d | ", nKey, (array - bArray)/keyLen, (array - bArray)/keyLen + nKey);
		//for (i = 0; i < keyLen; i++) {
		//	fprintf(stdout, "%d ", (unsigned char)*(array + nKey * keyLen + i));
		//}
		//fprintf(stdout, " | ");
		//for (i = 0; i < keyLen; i++) {
		//	fprintf(stdout, "%d ", (unsigned char)key[i]);
		//}
		cmp = memcmp(array + nKey * keyLen, key, keyLen);
		//mid < str
		if (cmp < 0) {			
			array += nKey * keyLen;
			array += keyLen;			
			//nKey--;
		} else if (cmp > 0){
			nKey--;
		} else {
			return array;
		}
	}
	return NULL;
}
*/


char * BinarySearch(char * array, int nKey, int keyLen, const char * key) {	
	int cmp;
	int a, b, mid;
	a = 0; b = nKey - 1;
/*
	if (memcmp(array, key, keyLen) > 0) {
		return NULL;
	}
	if (memcmp(array + b * keyLen, key, keyLen) < 0) {
		return NULL;
	}
*/
	while (a <= b) {
		mid = (a + b)/2;
		cmp = memcmp((char *)array + mid * keyLen, (char *)key, keyLen);
		//mid < str
		if (cmp < 0) {
			a = mid + 1;
		} else if (cmp > 0){
			b = mid - 1;
		} else {
			return array + mid * keyLen;
		}
	}
	return NULL;
}

char * BinarySearchEnhanced(char * array, int nKey, int arrLen, int cmpKeyLen, const char * key) {	
	int cmp;
	int a, b, mid;
	a = 0; b = nKey - 1;
	array += (arrLen - cmpKeyLen);
	while (a <= b) {
		mid = (a + b)/2;
		cmp = memcmp(array + mid * arrLen, key, cmpKeyLen);
		//mid < str
		if (cmp < 0) {
			a = mid + 1;
		} else if (cmp > 0){
			b = mid - 1;
		} else {
			return (array + mid * arrLen - (arrLen - cmpKeyLen));
		}
	}
	return NULL;
}

unsigned int RemoveSortedDuplicate(char * array, unsigned int nKey, int keyLen) {
	char * pArray, * rArray, * eArray;
	if (nKey == 0) {
		return 0;
	}
	eArray = array + keyLen * nKey;	
	for (rArray = array + keyLen; rArray < eArray; rArray += keyLen) {
		if (memcmp(rArray, rArray - keyLen, keyLen) == 0) {			
			break;
		} 
	}
	pArray = rArray - keyLen;
	rArray += keyLen;
	for (; rArray < eArray; rArray += keyLen) {
		if (memcmp(pArray, rArray, keyLen)) {
			pArray += keyLen;
			memcpy(pArray, rArray, keyLen);
		} 
	}
	return (pArray - array)/keyLen + 1;
}

void QuickSortRecursive(unsigned char * array, unsigned int nKey, int keyLen, int copyKeyLen) {
	unsigned char * lArray, * rArray, *bArray;	
	unsigned int nSmallerKey;
	//int i, j;
	if (nKey <= 2) {
		if (nKey == 2) {
			bArray = array + keyLen - copyKeyLen;
			rArray = bArray + keyLen;
			if (memcmp(bArray, rArray, copyKeyLen) > 0) {
				SwapMemory(bArray, rArray, copyKeyLen);
			}
		}
		return;
	}
	bArray = array + keyLen - copyKeyLen;
	lArray = bArray + keyLen ;
	rArray = bArray + keyLen * (nKey - 1);
	while (1) {
		while (memcmp(lArray, bArray, copyKeyLen) < 0) {
			lArray += keyLen;
			if (lArray > rArray) {
				break;
			}
		}
		while (memcmp(rArray, bArray, copyKeyLen) >= 0) {
			rArray -= keyLen;
			if (rArray <= bArray) {
				break;
			}
		}		
		if (lArray < rArray) {
			SwapMemory(lArray, rArray, copyKeyLen);
			lArray += keyLen;
			rArray -= keyLen;
		} else {
			break;
		}
	}

	SwapMemory(bArray, rArray, copyKeyLen);	
	nSmallerKey = (rArray - bArray)/keyLen;

	if (nSmallerKey > 1) {
		QuickSortRecursive(array, nSmallerKey, keyLen, copyKeyLen);
	}	
	if (nSmallerKey + 1 < nKey) {
		QuickSortRecursive(array + keyLen * (nSmallerKey + 1), nKey - nSmallerKey - 1, keyLen, copyKeyLen);
	}
}

void QuickSort(unsigned char * array, unsigned int nKey, int keyLen) {
	QuickSortRecursive(array, nKey, keyLen, keyLen);
}

void QuickSortByte(unsigned char * array, int nKey) {
	unsigned char * lArray, * rArray;	
	//int i, j;
	if (nKey <= 1) {
		return;
	}
	if (nKey == 2) {
		if (*array > *(array + 1)) {
			*array ^= *(array + 1);
			*(array + 1) ^= *array;
			*array ^= *(array + 1);
		}
		return;
	}
	lArray = array + 1 ;
	rArray = array + nKey - 1;

	while (1) {
		while (*lArray < *array) {
			lArray++;
			if (lArray > rArray) {
				break;
			}
		}
		while (*rArray >= *array) {
			rArray--;
			if (rArray <= array) {
				break;
			}
		}		
		if (lArray < rArray) {
			*lArray ^= *rArray;
			*rArray ^= *lArray;
			*lArray ^= *rArray;
			lArray++;
			rArray--;
		} else {
			break;
		}
	}

	if (rArray > array) {
		*array ^= *rArray;
		*rArray ^= *array;
		*array ^= *rArray;
		QuickSortByte(array, rArray - array);		
	} 
	QuickSortByte(rArray + 1, (array + nKey) - (rArray + 1));
}

void HeapSortByteSiftDown(unsigned char * array, int root, int bottom)
{
	int maxChild;
	int leftChild = 2 * root + 1;
	unsigned char * maxChildPtr, * rootPtr;
	while ( leftChild < bottom ) {
		if (array[leftChild] > array[leftChild + 1]) {
			maxChild = leftChild;
		} else {
			maxChild = leftChild + 1;
		} 
		rootPtr = array + root;
		maxChildPtr = array + maxChild;
		if ( *rootPtr < *maxChildPtr ) {
			*rootPtr ^= *maxChildPtr;
			*maxChildPtr ^= *rootPtr;
			*rootPtr ^= *maxChildPtr;
			root = maxChild;
			leftChild = 2 * root + 1;
		} else {
			break;
		}
	}
	if (leftChild == bottom) {
		rootPtr = array + root;
		maxChildPtr = array + leftChild;
		if ( *rootPtr < *maxChildPtr ) {
			*rootPtr ^= *maxChildPtr;
			*maxChildPtr ^= *rootPtr;
			*rootPtr ^= *maxChildPtr;
		}
	}
}

void HeapSortByte(unsigned char * array, int nKey)
{
	int i;	
	int nKeyMinusOne = nKey - 1;
	unsigned char * tempPtr, * nextArray = array + 1;
	for (i = (nKey/2) - 1; i >= 0; i--) {
		HeapSortByteSiftDown(array, i, nKeyMinusOne);
	}
/*	
	for (i = nKey - 1, tempPtr = array + i; i >= 1; i--, tempPtr--) {		
		*array ^= *tempPtr;
		*tempPtr ^= *array;
		*array ^= *tempPtr;
		HeapSortByteSiftDown(array, 0, i - 1);
	}
*/
	for (tempPtr = array + nKeyMinusOne; tempPtr > array; tempPtr--) {		
		*array ^= *tempPtr;
		*tempPtr ^= *array;
		*array ^= *tempPtr;
		HeapSortByteSiftDown(array, 0, tempPtr - nextArray);
	}
}
 

void HeapSortSiftDown(unsigned char * array, int root, int bottom, int keyLen)
{
	int maxChild;
	int leftChild = 2 * root + 1;
	unsigned char * maxChildPtr, * rootPtr;
	while ( leftChild < bottom ) {
		//if (array[leftChild] > array[leftChild + 1]) {
		if (memcmp(array + keyLen*leftChild, array + keyLen*leftChild + keyLen, keyLen) > 0) {
			maxChild = leftChild;
		} else {
			maxChild = leftChild + 1;
		} 
		rootPtr = array + keyLen * root;
		maxChildPtr = array + keyLen * maxChild;
		if ( memcmp(rootPtr, maxChildPtr, keyLen) < 0 ) {
			SwapMemory(rootPtr, maxChildPtr, keyLen);
			root = maxChild;
			leftChild = 2 * root + 1;
		} else {
			break;
		}
	}
	if (leftChild == bottom) {
		rootPtr = array + keyLen * root;
		maxChildPtr = array + keyLen * leftChild;
		if ( memcmp(rootPtr, maxChildPtr, keyLen) < 0 ) {
			SwapMemory(rootPtr, maxChildPtr, keyLen);		
		}
	}
}

void HeapSort(unsigned char * array, int nKey, int keyLen)
{
	int i;
	unsigned char * tempPtr;
	for (i = (nKey / 2) - 1; i >= 0; i--) {
		HeapSortSiftDown(array, i, nKey - 1, keyLen);
	} 	
	for (i = nKey - 2, tempPtr = array + keyLen*(i + 1); i >= 0; i--, tempPtr -= keyLen) {		
		SwapMemory(array, tempPtr, keyLen);
		HeapSortSiftDown(array, 0, i, keyLen);
	}
}

/*
void RadixSortRecursive(unsigned char * array, unsigned int nKey, int keyLen, int copyKeyLen, 
						unsigned char * appearKey,
						unsigned int * countKeyTable,
						unsigned char ** posKeyTable,
						unsigned char ** ePosKeyTable) {	
	
	int i;
	int j;
	unsigned char chA, chB;
	int nAppearKey;
	unsigned char * rArray, * bArray, * eArray; 
	
	unsigned int nTestKey = 0;

	if (nKey <= 1) {
		return;
	}
	if (nKey == 2) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		eArray = bArray + keyLen;
		if (memcmp(bArray, eArray, copyKeyLen) > 0) {
			SwapMemory(bArray, eArray, copyKeyLen);
		}
		return;
	}
	if (nKey == 3) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		rArray = bArray + keyLen;
		eArray = rArray + keyLen;
		if (memcmp(bArray, rArray, copyKeyLen) > 0) {
			SwapMemory(bArray, rArray, copyKeyLen);
		}
		if (memcmp(rArray, eArray, copyKeyLen) > 0) {
			SwapMemory(rArray, eArray, copyKeyLen);
			if (memcmp(bArray, rArray, copyKeyLen) > 0) {
				SwapMemory(bArray, rArray, copyKeyLen);
			}
		}
		return;
	}

	memset(countKeyTable, 0, 256 * sizeof(unsigned int));
	bArray = (unsigned char *)(array) + keyLen - copyKeyLen;
	eArray = bArray +  keyLen * nKey;
	for (rArray = bArray; rArray < eArray; rArray += keyLen) {
		countKeyTable[*rArray]++;
	}

	nAppearKey = 0;
	for (i = 0; i < 256; i++ ) {
		if (countKeyTable[i] > 0) {
			appearKey[nAppearKey] = i;
			nAppearKey++;
		}
	}

	if (nAppearKey > 1) {

		posKeyTable[appearKey[0]] = bArray;
		for (i = 1; i < nAppearKey; i++) {
			posKeyTable[appearKey[i]] = posKeyTable[appearKey[i - 1]] + keyLen * countKeyTable[appearKey[i - 1]];
			ePosKeyTable[appearKey[i - 1]] = posKeyTable[appearKey[i]];
		}
		ePosKeyTable[appearKey[i - 1]] = eArray;

		i = 0;
		chA = appearKey[i];
		while (1) {		
			while (*posKeyTable[chA] == chA) {
				posKeyTable[chA] += keyLen;
			}
			if (posKeyTable[chA] < ePosKeyTable[chA]) {
				chB = *posKeyTable[chA];
				while (*posKeyTable[chB] == chB) {
					posKeyTable[chB] += keyLen;
				}
				SwapMemory(posKeyTable[chA], posKeyTable[chB], copyKeyLen);
				posKeyTable[chB] += keyLen;
			} else {
				i++;
				while (i < nAppearKey) {
					if (posKeyTable[appearKey[i]] < ePosKeyTable[appearKey[i]]) {
						break;
					}
					i++;
				}
				if (i < nAppearKey) {				
					chA = appearKey[i];
				} else {
					break;
				}
			}
		}
	} 	

	if (copyKeyLen <= 1) {
		return;
	}

	rArray = (unsigned char *)array;
	for (i = 0; i < nAppearKey; i++) {
		RadixSortRecursive( rArray, countKeyTable[appearKey[i]], keyLen, copyKeyLen - 1,
	  						appearKey + 256, countKeyTable + 256, posKeyTable, ePosKeyTable);
		rArray += keyLen * countKeyTable[appearKey[i]];
	}
}
*/

/*
void RadixSortRecursive(unsigned char * array, unsigned int nKey, int keyLen, int copyKeyLen, 
						unsigned char * appearKey,
						unsigned int * countKeyTable,
						unsigned char ** posKeyTable,
						unsigned char ** ePosKeyTable) {	
	
	int i;
	int j;
	unsigned char chA, chB;
	int nAppearKey;
	//unsigned char appearKey[256];
	//unsigned int countKeyTable[256];
	//unsigned char * posKeyTable[256];
	//unsigned char * ePosKeyTable[256];	
	unsigned char * rArray, * bArray, * eArray; 
	
	if (nKey <= 1) {
		return;
	}
	if (nKey == 2) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		eArray = bArray + keyLen;
		if (memcmp(bArray, eArray, copyKeyLen) > 0) {
			SwapMemory(bArray, eArray, copyKeyLen);
		}
		return;
	}	
	if (nKey == 3) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		rArray = bArray + keyLen;
		eArray = rArray + keyLen;
		if (memcmp(bArray, rArray, copyKeyLen) > 0) {
			SwapMemory(bArray, rArray, copyKeyLen);
		}
		if (memcmp(rArray, eArray, copyKeyLen) > 0) {
			SwapMemory(rArray, eArray, copyKeyLen);
			if (memcmp(bArray, rArray, copyKeyLen) > 0) {
				SwapMemory(bArray, rArray, copyKeyLen);
			}
		}
		return;
	}

	bArray = (unsigned char *)(array) + keyLen - copyKeyLen;
	eArray = bArray +  keyLen * nKey;
	
	if (nKey >= 16) {		
		memset(countKeyTable, 0, 256 * sizeof(unsigned int));
		nAppearKey = 0;
		for (rArray = bArray; rArray < eArray; rArray += keyLen) {
			if (countKeyTable[*rArray] == 0) {
				appearKey[nAppearKey] = *rArray;
				nAppearKey++;
			}
			countKeyTable[*rArray]++;
		}
		if (nAppearKey >= 256) {
			for (i = 0; i < 256; i++) {
				appearKey[i] = i;
			}
		} else {			
			//QuickSort(appearKey, nAppearKey, 1, 1);
			//QuickSortByte(appearKey, nAppearKey);
			//fprintf(stdout, "\nbefore heapsort \n");
			//for (i = 0; i < nAppearKey; i++) {
			//	fprintf(stdout, "%d ", appearKey[i]);
			//}
			HeapSortByte(appearKey, nAppearKey);
		}
	} else {
		i = 0;
		for (rArray = bArray; rArray < eArray; rArray += keyLen) {
			appearKey[i] = *rArray;
			i++;
		}
		//QuickSortByte(appearKey, nKey);
		HeapSortByte(appearKey, nKey);
		nAppearKey = 0;
		j = 0;
		for (i = 1; i < nKey; i++) {
			if (appearKey[j] != appearKey[i]) {
				countKeyTable[appearKey[j]] = i - j;
				j = i;				
				nAppearKey++;
				appearKey[nAppearKey] = appearKey[i];
			}
		}
		countKeyTable[appearKey[j]] = nKey - j;
		nAppearKey++;
	}
	//fprintf(stdout, "\n");
	//for (i = 0; i < nAppearKey; i++) {
	//	fprintf(stdout, "%d ", appearKey[i]);
	//}
	//system("PAUSE");
	
	if (nAppearKey > 1) {

		posKeyTable[appearKey[0]] = bArray;
		for (i = 1; i < nAppearKey; i++) {
			posKeyTable[appearKey[i]] = posKeyTable[appearKey[i - 1]] + keyLen * countKeyTable[appearKey[i - 1]];
			ePosKeyTable[appearKey[i - 1]] = posKeyTable[appearKey[i]];
		}
		ePosKeyTable[appearKey[i - 1]] = eArray;

		i = 0;
		chA = appearKey[i];
		while (1) {		
			while (*posKeyTable[chA] == chA) {
				posKeyTable[chA] += keyLen;
			}
			if (posKeyTable[chA] < ePosKeyTable[chA]) {
				chB = *posKeyTable[chA];
				while (*posKeyTable[chB] == chB) {
					posKeyTable[chB] += keyLen;
				}
				SwapMemory(posKeyTable[chA], posKeyTable[chB], copyKeyLen);
				posKeyTable[chB] += keyLen;
			} else {
				i++;
				while (i < nAppearKey) {
					if (posKeyTable[appearKey[i]] < ePosKeyTable[appearKey[i]]) {
						break;
					}
					i++;
				}
				if (i < nAppearKey) {				
					chA = appearKey[i];
				} else {
					break;
				}
			}
		}
	} 	

	if (copyKeyLen <= 1) {
		return;
	}

	rArray = (unsigned char *)array;
	for (i = 0; i < nAppearKey; i++) {
		RadixSortRecursive( rArray, countKeyTable[appearKey[i]], keyLen, copyKeyLen - 1,
							appearKey + 256, countKeyTable + 256, posKeyTable, ePosKeyTable);
		rArray += keyLen * countKeyTable[appearKey[i]];
	}
}
*/


void RadixSortRecursive(unsigned char * array, unsigned int nKey, int keyLen, int copyKeyLen, 
						unsigned char * appearKey,
						unsigned int * countKeyTable,
						unsigned char ** posKeyTable,
						unsigned char ** ePosKeyTable) {	
	
	short int i;
	//int j;
	unsigned char chA, chB;
	short int nAppearKey;
	unsigned char * rArray, * bArray, * eArray; 
	unsigned char * nextAppearKey  = appearKey + 256;
	unsigned int * nextCountKeyTable = countKeyTable + 256;
	
	//if (nKey <= 1) {
	//	return;
	//}
	if (nKey == 2) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		eArray = bArray + keyLen;
		if (memcmp(bArray, eArray, copyKeyLen) > 0) {
			//SwapMemory(bArray, eArray, copyKeyLen);
			SwapMemoryWithoutChecking(bArray, eArray, copyKeyLen);
		}
		return;
	}	
	if (nKey == 3) {
		bArray = (unsigned char *)array + keyLen - copyKeyLen;
		rArray = bArray + keyLen;
		eArray = rArray + keyLen;
		if (memcmp(bArray, rArray, copyKeyLen) > 0) {
			//SwapMemory(bArray, rArray, copyKeyLen);
			SwapMemoryWithoutChecking(bArray, rArray, copyKeyLen);
		}
		if (memcmp(rArray, eArray, copyKeyLen) > 0) {
			//SwapMemory(rArray, eArray, copyKeyLen);
			SwapMemoryWithoutChecking(rArray, eArray, copyKeyLen);
			if (memcmp(bArray, rArray, copyKeyLen) > 0) {
				//SwapMemory(bArray, rArray, copyKeyLen);
				SwapMemoryWithoutChecking(bArray, rArray, copyKeyLen);
			}
		}
		return;
	}


	bArray = (unsigned char *)(array) + keyLen - copyKeyLen;
	eArray = bArray +  keyLen * nKey;
	
	if (nKey >= 32) { 
		//memset(countKeyTable, 0, 256 * sizeof(unsigned int));
		for (rArray = bArray; rArray < eArray; rArray += keyLen) {
			countKeyTable[*rArray]++;
		}
		nAppearKey = 0;
		for (i = 0; i < 256; i++ ) {
			if (countKeyTable[i] > 0) {
				appearKey[nAppearKey] = i;
				nAppearKey++;
			}
		}
	} else {
		//memset(countKeyTable, 0, 256 * sizeof(unsigned int));
		nAppearKey = 0;
		for (rArray = bArray; rArray < eArray; rArray += keyLen) {
			if (countKeyTable[*rArray] == 0) {
				appearKey[nAppearKey] = *rArray;
				nAppearKey++;
			}
			countKeyTable[*rArray]++;
		} 
		//QuickSortByte(appearKey, nAppearKey);
		HeapSortByte(appearKey, nAppearKey);		
	} 

	if (nAppearKey > 2) {

		posKeyTable[appearKey[0]] = bArray;
		for (i = 1; i < nAppearKey; i++) {
			posKeyTable[appearKey[i]] = posKeyTable[appearKey[i - 1]] + keyLen * countKeyTable[appearKey[i - 1]];
			ePosKeyTable[appearKey[i - 1]] = posKeyTable[appearKey[i]];
		}
		ePosKeyTable[appearKey[i - 1]] = eArray;

		i = 0;
		chA = appearKey[i];
		while (1) {		
			while (*posKeyTable[chA] == chA) {
				posKeyTable[chA] += keyLen;
			}
			if (posKeyTable[chA] < ePosKeyTable[chA]) {
				chB = *posKeyTable[chA];
				while (*posKeyTable[chB] == chB) {
					posKeyTable[chB] += keyLen;
				}
				//SwapMemory(posKeyTable[chA], posKeyTable[chB], copyKeyLen);
				SwapMemoryWithoutChecking(posKeyTable[chA], posKeyTable[chB], copyKeyLen);
				posKeyTable[chB] += keyLen;
			} else {
				i++;
				while (i < nAppearKey) {
					if (posKeyTable[appearKey[i]] < ePosKeyTable[appearKey[i]]) {
						break;
					}
					i++;
				}
				if (i < nAppearKey) {				
					chA = appearKey[i];
				} else {
					break;
				}
			}
		}
	} else if (nAppearKey == 2) {		
		eArray -= keyLen;
		chA = appearKey[0];
		chB = appearKey[1];
		while (1) {
			while (*bArray == chA) {
				bArray += keyLen;
			}
			while (*eArray == chB) {
				eArray -= keyLen;
			}
			if (bArray < eArray) { 
				SwapMemoryWithoutChecking(bArray, eArray, copyKeyLen);
				bArray += keyLen;
				eArray -= keyLen;
			} else {
				break;
			}
		}
	}

	if (copyKeyLen <= 1 || nAppearKey == nKey) {
		for (i = 0; i < nAppearKey; i++) {
			countKeyTable[appearKey[i]] = 0;
		}
		return;
	}

	copyKeyLen--;	
	rArray = (unsigned char *)array;
	for (i = 0; i < nAppearKey; i++) {
		chA = appearKey[i];
		if (countKeyTable[chA] > 0) {
			RadixSortRecursive( rArray, countKeyTable[chA], keyLen, copyKeyLen,
							nextAppearKey, nextCountKeyTable, posKeyTable, ePosKeyTable);
		}
		rArray += keyLen * countKeyTable[chA];
		countKeyTable[chA] = 0;
	}

}


void RadixSort(unsigned char * array, unsigned int nKey, int keyLen) {	
	unsigned char * appearKey = malloc(256 * sizeof(unsigned char) * keyLen);
	unsigned int * countKeyTable = malloc(256 * sizeof(unsigned int) * keyLen);	
	unsigned char * posKeyTable[256];
	unsigned char * ePosKeyTable[256];
	int i, j;
	if (nKey > 1) {
		memset(countKeyTable, 0, 256 * sizeof(unsigned int) * keyLen);
		RadixSortRecursive( array, nKey, keyLen, keyLen, 
							appearKey, countKeyTable, posKeyTable, ePosKeyTable);
/*
		fprintf(stdout, "\n\nsort result "); 
		for (i = 0; i < nKey; i++) {
			if (i % 100 == 0 && i > 0) {
				system("PAUSE");
			}
			fprintf(stdout, "\n%4d | ", i); 
			for (j = 0; j < keyLen; j++) {
				fprintf(stdout, "%d ", array[i * keyLen + j]);
			}
		}
		system("PAUSE");
*/
	}
	free(appearKey);
	free(countKeyTable);
}


void RadixSortEnhanced(unsigned char * array, unsigned int nKey, int keyLen,
					   unsigned char * appearKey, unsigned int * countKeyTable,
					   unsigned char ** posKeyTable, unsigned char ** ePosKeyTable) {	
	
	if (nKey > 1) {
		memset(countKeyTable, 0, 256 * sizeof(unsigned int) * keyLen);
		RadixSortRecursive( array, nKey, keyLen, keyLen, 
							appearKey, countKeyTable, posKeyTable, ePosKeyTable);
	}
}



/*
void RadixSortV2Recursive(unsigned char ** permutePtr, unsigned char ** permutePtrTemp,
						  unsigned int nKey, int keyLen, int sortOrdinalByte,
						  unsigned char * appearKey, 
						  unsigned char *** appearKeyPtr, unsigned char *** appearKeyPtrE, 
						  unsigned int * countKeyTable) {

	int i;
	int nAppearKey = 0;
	unsigned char ch;
	unsigned char ** perPtr, ** perPtrE, ** perPtrTemp;
	if (nKey <= 1 || sortOrdinalByte >= keyLen) {
		return;
	}
	perPtrE = permutePtr + nKey;
	for (perPtr = permutePtr; perPtr < perPtrE; perPtr++) {
		ch = (unsigned char)*((*perPtr) + sortOrdinalByte);
		if (countKeyTable[ch] == 0) {
			appearKey[nAppearKey] = ch;
			nAppearKey++;
		}
		countKeyTable[ch]++;
	}

	if (nAppearKey >= 256) {
		for (i = 0; i < 256; i++) {
			appearKey[i] = i;
		}
	} else {			
		HeapSortByte(appearKey, nAppearKey);
	}

	appearKeyPtr[appearKey[0]] = permutePtrTemp;
	for (i = 1; i < nAppearKey; i++) {
		appearKeyPtr[appearKey[i]] = appearKeyPtr[appearKey[i - 1]] + countKeyTable[appearKey[i - 1]];
	}

	for (perPtr = permutePtr; perPtr < perPtrE; perPtr++, perPtrTemp++) {
		ch = (unsigned char)*((*perPtr) + sortOrdinalByte);
		*(appearKeyPtr[ch]) = *perPtr;
		appearKeyPtr[ch]++;
	}
	memcpy(permutePtr, permutePtrTemp, nKey * sizeof(unsigned char *));

	for (i = 0; i < nAppearKey; i++) {
		RadixSortV2Recursive(permutePtr, permutePtrTemp, countKeyTable[appearKey[i]],
							 keyLen, sortOrdinalByte + 1, 
							 appearKey + 256, appearKeyPtr, appearKeyPtrE, countKeyTable + 256);
		permutePtr += countKeyTable[appearKey[i]];
		permutePtrTemp += countKeyTable[appearKey[i]];
		countKeyTable[appearKey[i]] = 0;
	}
}
*/


void RadixSortV2Recursive(unsigned char ** permutePtr, unsigned char ** permutePtrTemp1,
						  unsigned int nKey, int keyLen, int sortOrdinalByte,
						  unsigned char * appearKey, 
						  unsigned char *** appearKeyPtr, unsigned char *** appearKeyPtrE, 
						  unsigned int * countKeyTable) {

	int i;
	int nAppearKey = 0;
	unsigned char chA, chB;
	unsigned char ** perPtr, ** perPtrE;
	unsigned char * temp;

	if (nKey <= 1) {
		return;
	}
	if (nKey == 2) {
		if (memcmp(permutePtr[0] + sortOrdinalByte, permutePtr[1] + sortOrdinalByte, keyLen - sortOrdinalByte) > 0) {
			temp = permutePtr[0];
			permutePtr[0] = permutePtr[1];
			permutePtr[1] = temp;
			//SwapMemoryWithoutChecking(permutePtr, permutePtr + 1, sizeof(unsigned char *));
		}
		return;
	}	
	if (nKey == 3) {
		if (memcmp(permutePtr[0] + sortOrdinalByte, permutePtr[1] + sortOrdinalByte, keyLen - sortOrdinalByte) > 0) {
			temp = permutePtr[0];
			permutePtr[0] = permutePtr[1];
			permutePtr[1] = temp;
			//SwapMemoryWithoutChecking(permutePtr, permutePtr + 1, sizeof(unsigned char *));
		}
		if (memcmp(permutePtr[1] + sortOrdinalByte, permutePtr[2] + sortOrdinalByte, keyLen - sortOrdinalByte) > 0) {
			temp = permutePtr[1];
			permutePtr[1] = permutePtr[2];
			permutePtr[2] = temp;
			//SwapMemoryWithoutChecking(permutePtr + 1, permutePtr + 2, sizeof(unsigned char *));
			if (memcmp(permutePtr[0] + sortOrdinalByte, permutePtr[1] + sortOrdinalByte, keyLen - sortOrdinalByte) > 0) {
				temp = permutePtr[0];
				permutePtr[0] = permutePtr[1];
				permutePtr[1] = temp;
				//SwapMemoryWithoutChecking(permutePtr, permutePtr + 1, sizeof(unsigned char *));
			}
		}
		return;
	}

	perPtrE = permutePtr + nKey;
	for (perPtr = permutePtr; perPtr < perPtrE; perPtr++) {
		chA = (unsigned char)*((*perPtr) + sortOrdinalByte);
		if (countKeyTable[chA] == 0) {
			appearKey[nAppearKey] = chA;
			nAppearKey++;
		}
		countKeyTable[chA]++;
	}

	if (nAppearKey >= 256) {
		for (i = 0; i < 256; i++) {
			appearKey[i] = i;
		}
	} else {			
		HeapSortByte(appearKey, nAppearKey);
	}

/*
	fprintf(stdout, "\nnKey=%d orByte=%d nAppearKey=%d | ", nKey, sortOrdinalByte, nAppearKey);
	for (i = 0; i < nAppearKey; i++) {
		fprintf(stdout, "[%d]=%d ", (unsigned char)appearKey[i], countKeyTable[i]);
	}
*/
	if (nAppearKey > 1) {

		appearKeyPtr[appearKey[0]] = permutePtr;
		for (i = 1; i < nAppearKey; i++) {
			appearKeyPtr[appearKey[i]] = appearKeyPtr[appearKey[i - 1]] + countKeyTable[appearKey[i - 1]];
			appearKeyPtrE[appearKey[i - 1]] = appearKeyPtr[appearKey[i]];
		}
		appearKeyPtrE[appearKey[i - 1]] = perPtrE;
		
		i = 0;
		chA = appearKey[i];
		while (1) {
/*			
			while (*((*(appearKeyPtr[chA])) + sortOrdinalByte) == chA) {
				appearKeyPtr[chA]++;
				if (appearKeyPtr[chA] >= appearKeyPtrE[chA]) {
					break;
				}
			}
*/
			while (appearKeyPtr[chA] < appearKeyPtrE[chA]) {				
				if (*((*(appearKeyPtr[chA])) + sortOrdinalByte) != chA) {
					break;
				}
				appearKeyPtr[chA]++;
			}

			if (appearKeyPtr[chA] < appearKeyPtrE[chA]) {
				chB = *((*(appearKeyPtr[chA])) + sortOrdinalByte);
				while (*((*(appearKeyPtr[chB])) + sortOrdinalByte) == chB) {
					appearKeyPtr[chB]++;
				}
				//temp = *(appearKeyPtr[chA]);
				//*(appearKeyPtr[chA]) = *(appearKeyPtr[chB]);
				//*(appearKeyPtr[chB]) = temp;
				//SwapMemoryWithoutChecking(appearKeyPtr[chA], appearKeyPtr[chB], sizeof(unsigned char *));
				if (nKey < 5000) {
					temp = *(appearKeyPtr[chA]);
					*(appearKeyPtr[chA]) = *(appearKeyPtr[chB]);
					*(appearKeyPtr[chB]) = temp;
					//SwapMemoryWithoutChecking(appearKeyPtr[chA], appearKeyPtr[chB], sizeof(unsigned char *));
				} else {
					SwapMemoryWithoutChecking(*(appearKeyPtr[chA]) + sortOrdinalByte, *(appearKeyPtr[chB]) + sortOrdinalByte, keyLen - sortOrdinalByte);
				}
				appearKeyPtr[chB]++;
			} else {
				i++;
				while (i < nAppearKey) {
					if (appearKeyPtr[appearKey[i]] < appearKeyPtrE[appearKey[i]]) {
						break;
					}
					i++;
				}
				if (i < nAppearKey) {				
					chA = appearKey[i];
				} else {
					break;
				}
			}
		}
	} 

	if (sortOrdinalByte >= keyLen - 1) {
		for (i = 0; i < nAppearKey; i++) {
			countKeyTable[appearKey[i]] = 0;
		}
		return;
	}

	for (i = 0; i < nAppearKey; i++) {
		RadixSortV2Recursive(permutePtr, permutePtrTemp1, countKeyTable[appearKey[i]],
							 keyLen, sortOrdinalByte + 1, 
							 appearKey + 256, appearKeyPtr, appearKeyPtrE, countKeyTable + 256);
		permutePtr += countKeyTable[appearKey[i]];
		//permutePtrTemp1 += countKeyTable[appearKey[i]];
		countKeyTable[appearKey[i]] = 0;
	}
}


void RadixSortV2(unsigned char * inputArray, unsigned char * sortedArray, 
				 unsigned int nKey, int keyLen) {

	unsigned char * appearKey = malloc(256 * sizeof(unsigned char) * keyLen);
	unsigned char *** appearKeyPtr = malloc(256 * sizeof(unsigned char **));
	unsigned char *** appearKeyPtrE = malloc(256 * sizeof(unsigned char **));
	unsigned int * countKeyTable = malloc(256 * sizeof(unsigned int) * keyLen);	
	unsigned char ** permutePtr = malloc(nKey * sizeof(unsigned char *));
	unsigned char ** permutePtrTemp = malloc(nKey * sizeof(unsigned char *));

	unsigned char ** bkPtr, ** bkPtrE;
	unsigned char * ptr;
	//int i, j;
	if (nKey >= 1) {
		memset(countKeyTable, 0, 256 * sizeof(unsigned int) * keyLen);
		bkPtrE = permutePtr + nKey;
		ptr = inputArray;
		for (bkPtr = permutePtr ; bkPtr < bkPtrE; bkPtr++, ptr += keyLen) {
			*bkPtr = ptr; 
		}
/*
		for (i = 0; i < nKey; i++) {
			fprintf(stdout, "\n%3d | ", i);
			for (j = 0; j < keyLen; j++) {
				fprintf(stdout, "%d ", (unsigned char)permutePtr[i][j]);
			}
		}
*/
		RadixSortV2Recursive(permutePtr, permutePtrTemp, nKey, keyLen, 0, 
							 appearKey, appearKeyPtr, appearKeyPtrE, countKeyTable);
		ptr = sortedArray;
		for (bkPtr = permutePtr ; bkPtr < bkPtrE; bkPtr++, ptr += keyLen) {
			memcpy(ptr, *bkPtr, keyLen);
		}
	}

	free(appearKey);
	free(appearKeyPtr);
	free(appearKeyPtrE);
	free(countKeyTable);
	free(permutePtr);
	free(permutePtrTemp);
}

void RadixSortV2Enhance(unsigned char * inputArray, unsigned char * sortedArray, 
						unsigned int nKey, int keyLen,
						unsigned char * appearKey, 
						unsigned char *** appearKeyPtr, unsigned char *** appearKeyPtrE,
						unsigned int * countKeyTable,
						unsigned char ** permutePtr, unsigned char ** permutePtrTemp) {

	unsigned char ** bkPtr, ** bkPtrE;
	unsigned char * ptr;
	//int i, j;
	if (nKey >= 1) {
		memset(countKeyTable, 0, 256 * sizeof(unsigned int) * keyLen);
		bkPtrE = permutePtr + nKey;
		ptr = inputArray;
		for (bkPtr = permutePtr ; bkPtr < bkPtrE; bkPtr++, ptr += keyLen) {
			*bkPtr = ptr; 
		}
/*
		for (i = 0; i < nKey; i++) {
			fprintf(stdout, "\n%3d | ", i);
			for (j = 0; j < keyLen; j++) {
				fprintf(stdout, "%d ", (unsigned char)permutePtr[i][j]);
			}
		}
*/
		RadixSortV2Recursive(permutePtr, permutePtrTemp, nKey, keyLen, 0, 
							 appearKey, appearKeyPtr, appearKeyPtrE, countKeyTable);
		ptr = sortedArray;
		for (bkPtr = permutePtr ; bkPtr < bkPtrE; bkPtr++, ptr += keyLen) {
			memcpy(ptr, *bkPtr, keyLen);
		}
	}


}

int IsSortedArray(unsigned char * array, unsigned int nKey, int keyLen) {
	int i;
	char * eArray = array + nKey * keyLen - keyLen;
	if (nKey <= 1) {
		return 1;
	}
	for (; array < eArray; array += keyLen) {
		if (memcmp(array, array + keyLen, keyLen) > 0) {
			fprintf(stdout, "\n");
			for (i = 0; i < keyLen; i++) {
				fprintf(stdout, "%d ", (unsigned char)*(array + i));
			}
			fprintf(stdout, "\n");
			for (i = 0; i < keyLen; i++) {
				fprintf(stdout, "%d ", (unsigned char)*(array + i + keyLen));
			}
			return 0;
		}
	}
	return 1;
}