#include "mm_bus.h" 
#include <stdlib.h> 
#ifdef MM_DSP_DEBUG
 #include <assert.h>
#endif  

MMBus *MMBus_new(size_t size, size_t channels)
{
    MMBus *result;
    result = (MMBus*)malloc(sizeof(MMBus) + sizeof(MMSample) * size * channels);
#ifdef MM_DSP_DEBUG
    assert(result);
#endif  
    result->size = size;
    result->channels = channels;
    return result;
}
