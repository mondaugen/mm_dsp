#include "mm_trapenvedsampleplayer.h" 
#include "mm_common_calcs.h" 

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer*)tesp, (MMEnvelope*)&tesp->te,
            outBus, internalBusSize, tickPeriod);
}

void MMTrapEnvedSamplePlayer_noteOn(
        MMTrapEnvedSamplePlayer *tesp,
        MMSample        note,
        MMSample        amplitude,
        MMInterpMethod  interpolation,
        MMSample        index,
        MMSample        attackTime,
        MMSample        releaseTime,
        WavTav          *samples)
{
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).interp = interpolation;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).index = index;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note = note;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).rate = MMCC_MIDItoRate(note);
    MMSigProc_setState(&MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp),
                        MMSigProc_State_PLAYING);
    MMTrapezoidEnv_init(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp),
        0, amplitude, attackTime, releaseTime);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = samples;
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}
