#ifndef MM_POLY_VOICE_MANAGE_H
#define MM_POLY_VOICE_MANAGE_H 

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
    MMPolyVoiceUsed used;
};

#define MMPolyVoice_set_turnOn(pv,f)          ((MMPolyVoice*)pv)->turnOn = f
#define MMPolyVoice_set_turnOff(pv,f)         ((MMPolyVoice*)pv)->turnOff = f
#define MMPolyVoice_set_compare(pv,f)         ((MMPolyVoice*)pv)->compare  = f
#define MMPolyVoice_set_attachOnTurnOff(pv,f) ((MMPolyVoice*)pv)->attachOnTurnOff  = f
#define MMPolyVoice_turnOn(pv,params)         ((MMPolyVoice*)pv)->turnOn((MMPolyVoice*)pv,params) 
#define MMPolyVoice_turnOff(pv,params)        ((MMPolyVoice*)pv)->turnOff((MMPolyVoice*)pv,params) 
#define MMPolyVoice_compare(pv,params)        ((MMPolyVoice*)pv)->compare((MMPolyVoice*)pv,params) 
#define MMPolyVoice_attachOnTurnOff(pv,params) \
    ((MMPolyVoice*)pv)->attachOnTurnOff((MMPolyVoice*)pv,params) 

typedef struct __MMPolyManager MMPolyManager;

void MMPolyManager_noteOn(MMPolyManager *pm, void *params, MMPolyManagerSteal steal);

void MMPolyManager_noteOff(MMPolyManager *pm, void *params);

MMPolyManager *MMPolyManager_new(size_t numVoices);

int MMPolyManager_addVoice(MMPolyManager *pm, size_t index, MMPolyVoice *voice);

MMPolyVoice *MMPolyManager_getVoice(MMPolyManager *pm, size_t index);

MMPolyVoice *MMPolyManager_removeVoice(MMPolyManager *pm, size_t index);

#endif /* MM_POLY_VOICE_MANAGE_H */
