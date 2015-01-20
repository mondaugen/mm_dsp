#ifndef MM_WAVTAB_H
#define MM_WAVTAB_H 
#include "mm_array.h" 
#include "mm_sample.h" 
#include <stdint.h> 

/* Just the same as an MMArray, but must contain Sample type only! */
typedef struct __MMWavTab MMWavTab;
struct __MMWavTab {
    MMArray head;
    uint32_t samplerate; /* The sample rate that the sound in the wavetable was
                            "recorded" at*/
};

/* Table lookup, no interpolation. dest is a pointer to a Sample type. Wavtab is
 * instance of MMWavTab */
#define MMWavTab_get(wavtab,index) MMArray_get(wavtab,MMSample,(size_t)index)
/* Gets the oscillation frequency of the wavetable by dividing its samplerate by
 * its length */
#define MMWavTab_get_freq(w) ((MMSample)((MMWavTab*)w)->samplerate \
        / (MMSample)MMArray_get_length(w))

#endif /* MM_WAVTAB_H */
