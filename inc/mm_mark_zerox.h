#ifndef MM_MARK_ZEROX_H
#define MM_MARK_ZEROX_H 
#include "mm_sample.h" 
#include <stddef.h> 

#define MZX_GROUP_CROSSINGS (1 << 0)
#define MZX_NO_FINAL_NULL   (1 << 1) 
#define MZX_SEARCH_BACKWARD (1 << 2) 

void MM_mark_zeroxs(MMSample *x, size_t N, MMSample **uc, MMSample **dc, int side,
        MMSample *x_l, size_t maxnc, unsigned int opt);
#endif /* MM_MARK_ZEROX_H */
