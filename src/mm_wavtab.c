#include "mm_wavtab.h" 
/* Increment n_players */
void MMWavTab_inc_n_players(MMWavTab *w)
{
    w->n_players++;
}

/* Decrement n_players */
void MMWavTab_dec_n_players(MMWavTab *w)
{
    w->n_players--;
}
