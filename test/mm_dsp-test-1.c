/* Testing audio by making a signal processing chain, adding sample players and
 * playing this to stdout (sox or aplay will be able to pipe this to the
 * soundcard). */

int main (void)
{
    /* a signal chain to put our signal processors in. */
    MMSigChain sigChain;
    MMSigChain_init(&sc);

    /* a sample player
