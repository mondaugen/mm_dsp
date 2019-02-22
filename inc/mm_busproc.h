#ifndef MM_BUSPROC_H
#define MM_BUSPROC_H 

typedef struct _MMBusProc MMBusProc;

MMBusProc *
MMBusProc_new(
MMBus *bus,
void (*fun)(MMBus *bus, void *aux),
void *aux);

#endif /* MM_BUSPROC_H */
