#ifndef MM_COMMON_CALCS_H
#define MM_COMMON_CALCS_H 

#include "mm_sample.h"
#include "mm_tuning.h"

#define MMCC_ET12_TABLE_LENGTH 128 

/* common conversions that need to be made */
#define MMCC_MIDItoRate(midinote) (MMCC_et12_rate(midinote) * MMTuning_getA4Hz())

float MMCC_et12_rate(float pitch);

#endif /* MM_COMMON_CALCS_H */
