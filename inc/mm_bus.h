#ifndef MM_BUS_H
#define MM_BUS_H 

#include "mm_array.h" 
#include "mm_sample.h" 

/* A bus is just a pointer to a Sample */
typedef MMSample * MMBus;
/* A bus handle is a pointer to a Bus */
typedef MMBusHandle * MMBus;
/* A bus array is just a normal array, but this way you know what's in the
 * data */
typedef MMArray    MMBusArray;

#define MMBus_setBus(bus,addr) bus = addr
#define MMBus_getSample(bus) (*(bus))
#define MMBusHandle_getBus(bush) (*(bush))
#define MMBusHandle_getSample(bush) MMBus_getSample(MMBusHandle_getBus(bush)) 
#define MMBusHandle_setSample(bush,val) MMBusHandle_getSample(bush) = val

#define MMBusArray_get(array,index) MMArray_get(array,MMBus,index) 
#define MMBusArray_set(array,index,value) MMArray_set(array,MMBus,index,value) 

#endif /* MM_BUS_H */
