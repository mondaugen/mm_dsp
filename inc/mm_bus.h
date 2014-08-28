#ifndef MM_BUS_H
#define MM_BUS_H 

#include "mm_array.h" 
#include "mm_sample.h" 

/* A bus is just a pointer to a Sample */
typedef MMSample * MMBus;
/* A bus array is just a normal array, but this way you know what's in the
 * data */
typedef MMArray    MMBusArray;

#define MMBusArray_get(array,index) MMArray_get(array,MMBus,index) 
#define MMBusArray_set(array,index,value) MMArray_set(array,MMBus,index,value) 

#endif /* MM_BUS_H */
