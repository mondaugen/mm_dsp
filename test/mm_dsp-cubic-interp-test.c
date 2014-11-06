#include "mm_interp.h" 

#include <stdlib.h> 
#include <stdio.h> 

int main (int argc, char **argv)
{
    if (argc != 10) {
        fprintf(stderr,"arguments are: progname x0 y0 x1 y1 x2 y2 x3 y3 x\n");
        return(1);
    }
    float x0, x1, x2, x3, y0, y1, y2, y3, x;
    x0 = atof(argv[1]);
    x1 = atof(argv[3]);
    x2 = atof(argv[5]);
    x3 = atof(argv[7]);
    y0 = atof(argv[2]);
    y1 = atof(argv[4]);
    y2 = atof(argv[6]);
    y3 = atof(argv[8]);
    x  = atof(argv[9]);
//    printf("%f %f %f %f %f %f %f %f %f\n", x0, y0, x1, y1, x2, y2, x3, y3, x);
    printf("%f\n",MM_f_interp_cubic_(x0,y0,x1,y1,x2,y2,x3,y3,x));
    return(0);
}
