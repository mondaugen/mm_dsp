#include "mm_trapenvedsampleplayer.h" 

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer*)tesp, (MMEnvelope*)&tesp->te,
            outBus, internalBusSize, tickPeriod);
}
