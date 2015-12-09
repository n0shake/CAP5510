#ifndef __SortFuncs_h__
#define __SortFuncs_h__

char * BinarySearch(char * array, int nKey, int keyLen, const char * key);
char * BinarySearchEnhanced(char * array, int nKey, int arrLen, int cmpKeyLen, const char * key);

unsigned int RemoveSortedDuplicate(char * array, unsigned int nKey, int keyLen);

void RadixSort(unsigned char * array, unsigned int nKey, int keyLen);
void RadixSortEnhanced(unsigned char * array, unsigned int nKey, int keyLen,
					   unsigned char * appearKey, unsigned int * countKeyTable,
					   unsigned char ** posKeyTable, unsigned char ** ePosKeyTable);

void RadixSortV2(unsigned char * inputArray, unsigned char * sortedArray, 
				 unsigned int nKey, int keyLen);
void RadixSortV2Enhance(unsigned char * inputArray, unsigned char * sortedArray, 
						unsigned int nKey, int keyLen,
						unsigned char * appearKey, 
						unsigned char *** appearKeyPtr, unsigned char *** appearKeyPtrE,
						unsigned int * countKeyTable,
						unsigned char ** permutePtr, unsigned char ** permutePtrTemp);

void QuickSort(unsigned char * array, unsigned int nKey, int keyLen);
void QuickSortByte(unsigned char * array, int nKey);

void HeapSort(unsigned char * array, int nKey, int keyLen);
void HeapSortByte(unsigned char * array, int nKey);

int IsSortedArray(unsigned char * array, unsigned int nKey, int keyLen);

#endif