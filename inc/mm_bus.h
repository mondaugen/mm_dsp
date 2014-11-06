#ifndef MM_BUS_H
#define MM_BUS_H 

#include <stddef.h> 
#include "mm_array.h" 
#include "mm_sample.h" 

typedef struct __MMBus MMBus;

struct __MMBus {
    size_t size; /* size of one channel in samples */
    size_t channels; /* how many channels */
    /* the sample data. the size of this array in bytes is sizeof(MMSample) * size * channels. The samples are stored in interleaved format so, given an array of MMSample, the samples are stored: channel0-sample0, channel1-sample0, ... channel(n-1)-sample0, channel0-sample1, ... channel(n-1)-sample1, ..., ... channel(n-1)-sample(m-1) where n and m are the number of channels and size respectively */
    MMSample data[];  
};

/* A bus array is just a normal array, but this way you know what's in the
 * data */
typedef MMArray    MMBusArray;

#define MMBusArray_get(array,index) MMArray_get(array,MMBus,index) 
#define MMBusArray_set(array,index,value) MMArray_set(array,MMBus,index,value) 

MMBus *MMBus_new(size_t size, size_t channels);

#endif /* MM_BUS_H */
