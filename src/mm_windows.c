/* Fill arrays of type MMSample with windows like Hann, Hamming, etc. */
#include "mm_windows.h"
#include <math.h> 

/* Fills the array w with N points of a Hann window. If N is even, the
 * value at w[0] is 0 and the value at w[N-1]=cos(-pi/2 + pi*(N-1)/N). If N is
 * odd, the value at w[0] is 0 and the value at w[N-1] is 0, which means that
 * the value at w[N-2]=cos(-pi/2 + pi*(N-2)/(N-1)). This is so that the centre
 * of the window, whose value is 1, is always represented. */
void MM_hann_fill(MMSample *w, size_t N)
{
    if (N % 2) {
        /* N odd. Set end point to 0 and fill the reset as if window were of
         * length N-1 */
        w[N-1] = 0;
        N -= 1;
    }
    size_t n;
    MMSample phase;
    phase = -M_PI/2.;
    w[0] = 0.;
    phase += M_PI/(MMSample)N;
    for (n = 1; n < N; n++) {
        w[n] = cos(phase);
        w[n] *= w[n];
        phase += M_PI/(MMSample)N;
    }
}
