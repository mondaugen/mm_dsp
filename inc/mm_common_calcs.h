#ifndef MM_COMMON_CALCS_H
#define MM_COMMON_CALCS_H 

#include <math.h> 
#include "mm_sample.h"
#include "mm_tuning.h"

/* common conversions that need to be made */
#define  MMCC_MIDItoRate(midinote) pow(2., (midinote - 69.) / 12.) * MMTuning_getA4Hz()

#endif /* MM_COMMON_CALCS_H */
