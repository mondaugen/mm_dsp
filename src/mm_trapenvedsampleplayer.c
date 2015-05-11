#include "mm_trapenvedsampleplayer.h" 
#include "mm_common_calcs.h" 

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer*)tesp, (MMEnvelope*)&tesp->teg.te,
            outBus, internalBusSize);
    /* Trapezoid envelope needs to be initialized, but its parameters can later
     * be changed, we just put some arbitrary ones for now */
    MMTrapEnvGen_init(&tesp->teg, ((MMEnvedSamplePlayer*)tesp)->envBus,
                        tickPeriod, 0, 1, 1, 1,-1);
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
        MMSample        sustainTime,
        MMWavTab        *samples,
        MMBool          loop)
{
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).interp = interpolation;
    MMSamplePlayerSigProc_setIndex_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),index);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note = note;
    MMSamplePlayerSigProc_setRate_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)), 
        MMCC_MIDItoRate(note) / MMWavTab_get_freq(samples));
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).loop = loop;
    MMSigProc_setState(&MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp),
                        MMSigProc_State_PLAYING);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = samples;
    MMTrapezoidEnv_setEnvParams(&tesp->teg.te,0, amplitude, attackTime, releaseTime,sustainTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

/* set the rate of playback rather than the MIDI note, but still index it by a
 * note to keep track of the voice */
void MMTrapEnvedSamplePlayer_noteOn_Rate(
        MMTrapEnvedSamplePlayer *tesp,
        MMSample        note,
        MMSample        amplitude,
        MMInterpMethod  interpolation,
        MMSample        index,
        MMSample        attackTime,
        MMSample        releaseTime,
        MMSample        sustainTime,
        MMWavTab        *samples,
        MMBool          loop,
        MMSample        rate)
{
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).interp = interpolation;
    MMSamplePlayerSigProc_setIndex_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),index);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note = note;
    MMSamplePlayerSigProc_setRate_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),rate);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).loop = loop;
    MMSigProc_setState(&MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp),
                        MMSigProc_State_PLAYING);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = samples;
    MMTrapezoidEnv_setEnvParams(&tesp->teg.te,0, amplitude, attackTime, releaseTime, sustainTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}
