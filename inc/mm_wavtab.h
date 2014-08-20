#ifndef MM_WAVTAB_H
#define MM_WAVTAB_H 
#include "mm_array.h" 
#include "mm_sample.h" 

/* Just the same as an MMArray, but must contain Sample type only! */
typedef MMArray MMWavTab;

/* Table lookup, no interpolation. dest is a pointer to a Sample type. Wavtab is
 * instance of MMWavTab */
#define MMWavTab_get(wavtab,index,dest) do { \
    while (index > wavtab.length) { index -= wavtab.length; } \
    while (index < wavtab.length) { index += wavtab.length; } \
    *dest = MMArray_get(wavtab,Sample,index); \
} while (0)

#endif /* MM_WAVTAB_H */
