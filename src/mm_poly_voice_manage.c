#include "mm_dllist.h"

/* Handle allocation of polyphonic notes. */

typedef enum {
    MMPolyManagerSteal_TRUE,
    MMPolyManagerSteal_FALSE,
} MMPolyManagerSteal;

typedef enum {
    MMPolyVoiceUsed_TRUE,
    MMPolyVoiceUsed_FALSE,
} MMPolyVoiceUsed;

typedef struct __MMPolyVoice MMPolyVoice;

struct __MMPolyVoice {
    MMDLList head;
    void (*turnOn)(MMPolyVoice *pv, void *params);
    void (*turnOff)(MMPolyVoice *pv, void *params);
    /* should return negative when first less than second, 0 if equal, 1 if
     * greater than. If no ordering, should return 1 */
    int (*compare)(MMPolyVoice *pv, void *params);
    /* Attaches the turn off function so it can be called when whatever voice
     * has actually finished. */
    void (*attachOnTurnOff)(MMPolyVoice *pv, void *params);
};

#define MMPolyVoice_turnOn(pv,params) ((MMPolyVoice*)pv)->turnOn((MMPolyVoice*)pv,params) 
#define MMPolyVoice_turnOff(pv,params) ((MMPolyVoice*)pv)->turnOff((MMPolyVoice*)pv,params) 
#define MMPolyVoice_compare(pv,params) ((MMPolyVoice*)pv)->compare((MMPolyVoice*)pv,params) 
#define MMPolyVoice_attachOnTurnOff(pv,params) \
    ((MMPolyVoice*)pv)->attachOnTurnOff((MMPolyVoice*)pv,params) 

typedef struct __MMPolyManager MMPolyManager;

struct __MMPolyManager {
    MMPolyVoice **voices;
    size_t numVoices;
    MMPolyVoice playingVoices;
    size_t numPlayingVoices;
};

void MMPolyManager_noteOn(MMPolyManager *pm, void *params, MMPolyManagerSteal steal)
{
    /* Check if note is already playing */
    MMPolyVoice *pv;
    pv = MMDLList_getNext(&pm.playingVoices);
    while (pv) {
        if (MMPolyVoice_compare(pv,params) == 0) {
            MMPolyVoice_turnOn(pv,params);
            return;
        }
        pv = pv->next;
    }
    /* So the voice isn't already playing, try and play the next free voice */
    int i;
    for (i = 0; i < pm->numVoices; i++) {
        if (pm->voices[i]->used == MMPolyVoiceUsed_FALSE) {
            MMPolyVoice_turnOn(pm->voices[i],params);
            MMDLList_addAfter(&pm.playingVoice,pm->voices[i]);
            return;
        }
    }
    /* So there wasn't a free voice, if we're allowed to steal, do it */
    if (steal == MMPolyManagerSteal_TRUE) {
        pv = MMDLList_getLast(&pm.playingVoice);
        /* The function attached here should start a new note playing when the note
         * has finished */
        MMPolyVoice_attachOnTurnOff(pv,params);
        MMPolyVoice_turnOff(pv,params);
    }
}

void MMPolyManager_noteOff(MMPolyManager *pm, void *params)
{
    /* find note */
    MMPolyVoice *pv;
    pv = MMDLList_getNext(&pm.playingVoices);
    while (pv) {
        if (MMPolyVoice_compare(pv,params) == 0) {
            MMPolyVoice_attachOnTurnOff(pv,params);
            MMPolyVoice_turnOff(pv,params);
            return;
        }
        pv = pv->next;
    }
    /* note wasn't there, do nothing */
}
