#include "mm_trapenvedsampleplayer.h" 
#include "mm_common_calcs.h" 

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer*)tesp, (MMEnvelope*)&tesp->teg.te,
            outBus, internalBusSize, tickPeriod);
    /* Trapezoid envelope needs to be initialized, but its parameters
     * time can later be changed, we just put some arbitrary ones for now */
    MMTrapEnvGen_init(&tesp->teg, ((MMEnvedSamplePlayer*)tesp)->envBus,
                        tickPeriod, 0, 1, 1, 1);
    MMEnvedSamplePlayer_insertEnvGen(tesp,&tesp->teg);
}

void MMTrapEnvedSamplePlayer_noteOn(
        MMTrapEnvedSamplePlayer *tesp,
        MMSample        note,
        MMSample        amplitude,
        MMInterpMethod  interpolation,
        MMSample        index,
        MMSample        attackTime,
        MMSample        releaseTime,
        MMWavTab        *samples,
        MMBool          loop)
{
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).interp = interpolation;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).index = index;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note = note;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).rate = 
        MMCC_MIDItoRate(note) / MMWavTab_get_freq(samples);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).loop = loop;
    MMSigProc_setState(&MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp),
                        MMSigProc_State_PLAYING);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = samples;
    MMTrapezoidEnv_setEnvParams(0, amplitude, attackTime, releaseTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}
