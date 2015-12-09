#ifndef __PMSPRune_h__
#define __PMSPRune_h__

#include "InputDefs.h"

typedef char CHammingDistMat[CONST_MAX_NUM_STRINGS][CONST_MAX_INPUT_STRING_LENGTH];

int PMSPrune( int motifLen, int hammingDist, const CInputStringSet * inputStrs,
			  CCompactMotif * foundMotifs, int maxNumMotifsAllowed);

int PMSPrune4( int motifLen, int hammingDist, CInputStringSet * inputStrs,
			  CCompactMotif * foundMotifs, int maxNumMotifsAllowed);

#endif