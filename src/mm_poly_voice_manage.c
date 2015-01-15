#include "mm_poly_voice_manage.h"
#include <stdlib.h> 

/* Handle allocation of polyphonic notes. */

struct __MMPolyManager {
    MMPolyVoice **voices;
    size_t numVoices;
    MMPolyVoice playingVoices;
    size_t numPlayingVoices;
};

/* Turn a note on using params. If steal is set, then will steal the oldest
 * voice. params must have been alloced with malloc() and can be freed with
 * free(). when stealing is off, params are freed immediately after being used
 * to turn the voice on. when stealing is on, the "doOnDone" method of the
 * MMPolyVoice (it should have one!) should free the params. */
void MMPolyManager_noteOn(MMPolyManager *pm, void *params, MMPolyManagerSteal steal)
{
    /* Check if note is already playing */
    MMPolyVoice *pv;
    pv = (MMPolyVoice*)MMDLList_getNext(&pm->playingVoices);
    while (pv) {
        if (MMPolyVoice_compare(pv,params) == 0) {
            MMPolyVoice_turnOn(pv,params);
            free(params); /* free params because they will not be used again */
            return;
        }
        pv = (MMPolyVoice*)MMDLList_getNext(pv);
    }
    /* So the voice isn't already playing, try and play the next free voice */
    int i;
    for (i = 0; i < pm->numVoices; i++) {
        if (pm->voices[i]->used == MMPolyVoiceUsed_FALSE) {
            MMPolyVoice_turnOn(pm->voices[i],params);
            free(params); /* free params because they will not be used again */
            MMDLList_insertAfter((MMDLList*)&pm->playingVoices,(MMDLList*)pm->voices[i]);
            return;
        }
    }
    /* So there wasn't a free voice, if we're allowed to steal, do it */
    if (steal == MMPolyManagerSteal_TRUE) {
        pv = (MMPolyVoice*)MMDLList_getTail((MMDLList*)&pm->playingVoices);
        /* The function attached here should start a new note playing when the note
         * has finished */
        MMPolyVoice_attachOnTurnOff(pv,params);
        MMPolyVoice_turnOff(pv,params);
        /* don't free the params !! whatever happens when the note eventually
         * turns off still needs the params! */
    }
}

void MMPolyManager_noteOff(MMPolyManager *pm, void *params)
{
    /* find note */
    MMPolyVoice *pv;
    pv = (MMPolyVoice*)MMDLList_getNext(&pm->playingVoices);
    while (pv) {
        if (MMPolyVoice_compare(pv,params) == 0) {
            MMPolyVoice_attachOnTurnOff(pv,params);
            MMPolyVoice_turnOff(pv,params);
            return;
        }
        pv = (MMPolyVoice*)MMDLList_getNext(pv);
    }
    /* note wasn't there, do nothing */
}

MMPolyManager *MMPolyManager_new(size_t numVoices)
{
    MMPolyManager *result = (MMPolyManager*)malloc(sizeof(MMPolyManager));
    result->voices = (MMPolyVoice**)malloc(sizeof(MMPolyVoice*) * numVoices);
    result->numVoices = numVoices;
    result->numPlayingVoices = 0;
    return result;
}

void MMPolyManager_free(MMPolyManager *pm)
{
    free(pm->voices);
    free(pm);
}

/* Returns -1 if out of range and otherwise stores pointer to polyvoice and
 * returns 0 */
int MMPolyManager_addVoice(MMPolyManager *pm, size_t index, MMPolyVoice *voice)
{
    if (index >= pm->numVoices) {
        return -1; /* Index out of range */
    }
    pm->voices[index] = voice;
    return 0;
}

/* Returns NULL if out of range and otherwise returns pointer to polyvoice */
MMPolyVoice *MMPolyManager_getVoice(MMPolyManager *pm, size_t index)
{
    if (index >= pm->numVoices) {
        return NULL; /* Index out of range */
    }
    return pm->voices[index];
}

/* Returns NULL if out of range and otherwise returns pointer to polyvoice after
 * having written NULL at its index in the poly manager */
MMPolyVoice *MMPolyManager_removeVoice(MMPolyManager *pm, size_t index)
{
    if (index >= pm->numVoices) {
        return NULL; /* Index out of range */
    }
    MMPolyVoice *result = pm->voices[index];
    pm->voices[index] = NULL;
    return result;
}

