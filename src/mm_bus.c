#include "mm_bus.h" 
#include <stdlib.h> 

MMBus *MMBus_new(size_t size, size_t channels)
{
    MMBus *result;
    result = (MMBus*)malloc(sizeof(MMBus) + sizeof(MMSample) * size * channels);
    result->size = size;
    result->channels = channels;
    return result;
}
