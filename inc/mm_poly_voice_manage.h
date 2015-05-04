#ifndef MM_POLY_VOICE_MANAGE_H
#define MM_POLY_VOICE_MANAGE_H 

#include <stdlib.h> 
#include "mm_dllist.h"

/* Handle allocation of polyphonic notes. */

typedef enum {
    MMPolyManagerSteal_FALSE,
    MMPolyManagerSteal_TRUE,
} MMPolyManagerSteal;

typedef enum {
    MMPolyVoiceUsed_TRUE,
    MMPolyVoiceUsed_FALSE,
} MMPolyVoiceUsed;

/* The params that are passed to the function need to subclass this struct so
 * that they can be passed a pointer to the PolyVoice that needs to be freed
 * when their sig proc calls onDone */
typedef struct __MMPolyVoiceParams MMPolyVoiceParams;

typedef struct __MMPolyVoice MMPolyVoice;

struct __MMPolyVoice {
    MMDLList head;
    void (*turnOn)(MMPolyVoice *pv, MMPolyVoiceParams *params);
    void (*turnOff)(MMPolyVoice *pv, MMPolyVoiceParams *params);
    /* should return negative when first less than second, 0 if equal, 1 if
     * greater than. If no ordering, should return 1 */
    int (*compare)(MMPolyVoice *pv, MMPolyVoiceParams *params);
    /* Attaches the turn off function so it can be called when whatever voice
     * has actually finished. This function should free params when it has
     * finished with them. ALSO it should free any previous params that may have
     * been attached and that haven't already been freed to avoid a memory leak! */
    void (*attachOnTurnOff)(MMPolyVoice *pv, MMPolyVoiceParams *params);
    MMPolyVoiceUsed used;
};

struct __MMPolyVoiceParams {
    MMPolyManagerSteal steal; /* Whether or not we steal a note */
    MMPolyVoice *parent;
    /* Something that can give unique voice numbers, and that
       needs to be let know when a voice is freed. */
    void *allocator; 
    /* The way that the allocator is let know when a voice is freed. */
    void (*yield_params_to_allocator)(void *allocator, void *params);
};

#define MMPolyVoiceParams_init(pvp)           ((MMPolyVoiceParams*)pvp)->steal = MMPolyManagerSteal_FALSE;\
                                              ((MMPolyVoiceParams*)pvp)->parent = NULL;\
                                              ((MMPolyVoiceParams*)pvp)->allocator = NULL;\
                                              ((MMPolyVoiceParams*)pvp)->yield_params_to_allocator = NULL
                                                 
#define MMPolyVoice_set_used(pv,u)            ((MMPolyVoice*)pv)->used = u
#define MMPolyVoice_set_turnOn(pv,f)          ((MMPolyVoice*)pv)->turnOn = f
#define MMPolyVoice_set_turnOff(pv,f)         ((MMPolyVoice*)pv)->turnOff = f
#define MMPolyVoice_set_compare(pv,f)         ((MMPolyVoice*)pv)->compare  = f
#define MMPolyVoice_set_attachOnTurnOff(pv,f) ((MMPolyVoice*)pv)->attachOnTurnOff  = f
#define MMPolyVoice_turnOn(pv,params)         ((MMPolyVoice*)pv)->turnOn((MMPolyVoice*)pv,params) 
#define MMPolyVoice_turnOff(pv,params)        ((MMPolyVoice*)pv)->turnOff((MMPolyVoice*)pv,params) 
#define MMPolyVoice_compare(pv,params)        ((MMPolyVoice*)pv)->compare((MMPolyVoice*)pv,params) 
#define MMPolyVoice_attachOnTurnOff(pv,params) \
    ((MMPolyVoice*)pv)->attachOnTurnOff((MMPolyVoice*)pv,params) 
#define MMPolyVoice_init(pv) MMDLList_init(pv) /* Just initializes list head */

typedef struct __MMPolyManager MMPolyManager;

void MMPolyManager_noteOn(MMPolyManager *pm,
                          MMPolyVoiceParams *params);

void MMPolyManager_noteOff(MMPolyManager *pm, MMPolyVoiceParams *params);

MMPolyManager *MMPolyManager_new(size_t numVoices);

int MMPolyManager_addVoice(MMPolyManager *pm, size_t index, MMPolyVoice *voice);

MMPolyVoice *MMPolyManager_getVoice(MMPolyManager *pm, size_t index);

MMPolyVoice *MMPolyManager_removeVoice(MMPolyManager *pm, size_t index);

#endif /* MM_POLY_VOICE_MANAGE_H */
