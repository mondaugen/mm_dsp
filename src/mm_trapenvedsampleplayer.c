#include "mm_trapenvedsampleplayer.h" 
#include "mm_common_calcs.h" 

/* The env field of the init struct will be replaced with a pointer to the
 * TrapEnvedSamplePlayer's envelope (a trapezoidal envelope, duh).
 * The rest of the fields of the init struct, including the fields of it's base
 * classe(s) need to be provided. */
void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp,    
                                  MMTrapEnvedSamplePlayerInitStruct *init)
{
    ((MMEnvedSamplePlayerInitStruct*)init)->env = (MMEnvelope*)&tesp->teg.te;
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer*)tesp,
            (MMEnvedSamplePlayerInitStruct*)init);
    /* Trapezoid envelope needs to be initialized, but its parameters can later
     * be changed, we just put some arbitrary ones for now */
    MMTrapEnvGen_init(&tesp->teg,((MMEnvedSamplePlayer*)tesp)->envBus,
                        init->tickPeriod, 0, 1, 1, 1,-1);
    MMEnvedSamplePlayer_insertEnvGen(tesp,&tesp->teg);
}

void MMTrapEnvedSamplePlayer_noteOn(
        MMTrapEnvedSamplePlayer *tesp, 
        MMTrapEnvedSamplePlayer_noteOnStruct *init)
{
    MMSamplePlayerSigProc_setIndex_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->index);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note = init->note;
    MMSamplePlayerSigProc_setRate_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)), 
            MMCC_MIDItoRate(init->note) / MMWavTab_get_freq(init->samples));
    MMSamplePlayerSigProc_setRate_ptr_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            NULL);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples =
        init->samples;
    MMTrapezoidEnv_setEnvParams(&tesp->teg.te,
                                0,
                                init->amplitude,
                                init->attackTime,
                                init->releaseTime,
                                init->sustainTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

/* set the rate of playback rather than the MIDI note, but still index it by a
 * note to keep track of the voice */
void MMTrapEnvedSamplePlayer_noteOn_Rate(
        MMTrapEnvedSamplePlayer *tesp,
        MMTrapEnvedSamplePlayer_noteOnStruct *init)
{
    MMSamplePlayerSigProc_setIndex_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->index);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note =
        init->note;
    MMSamplePlayerSigProc_setRate_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->rate);
    MMSamplePlayerSigProc_setRate_ptr_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            NULL);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = 
        init->samples;
    MMTrapezoidEnv_setEnvParams(&tesp->teg.te,
                                0,
                                init->amplitude,
                                init->attackTime,
                                init->releaseTime,
                                init->sustainTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

/* set the rate of playback from a pointer to a rate so that it can update
 * dynamically */
void MMTrapEnvedSamplePlayer_noteOn_pRate(
        MMTrapEnvedSamplePlayer *tesp,
        MMTrapEnvedSamplePlayer_noteOnStruct *init)
{
    MMSamplePlayerSigProc_setIndex_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->index);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note =
        init->note;
    MMSamplePlayerSigProc_setRate_flt_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->rate);
    MMSamplePlayerSigProc_setRate_ptr_(
            &(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)),
            init->p_rate);
    mm_q8_24_t tmp;
    tmp = (mm_q8_24_t)(((int64_t)(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)).rate
            * *(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp)).p_rate) >> 24LL);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).last_rate = 
        tmp;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = 
        init->samples;
    MMTrapezoidEnv_setEnvParams(&tesp->teg.te,
                                0,
                                init->amplitude,
                                init->attackTime,
                                init->releaseTime,
                                init->sustainTime);
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

