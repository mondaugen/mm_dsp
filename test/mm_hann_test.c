#include <stdio.h>
#include "mm_windows.h" 

int main (int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr,"Arguments are %s length\n",argv[0]);
        return(-1);
    }
    int N;
    N = atoi(argv[1]);
    MMSample w[N];
    MM_hann_fill(w,N);
    fwrite(w,sizeof(MMSample),N,stdout);
}
