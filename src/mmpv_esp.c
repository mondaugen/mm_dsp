/* An poly voice for an enveloped sample player */

typedef struct __MMPvesp MMPvesp;

struct __MMPvesp {
    MMPolyVoice head;
    MMEnvedSamplePlayer *esp;
}
