#include "mm_interp.h" 

/* Get len_y_ values by looking up in y which is a table of length len_y. The
 * results are put in y_. idx0 is a pointer to a starting index which will be
 * incremented at the rate. Both idx0 and rate are in Q_24_8 format, that means
 * that there are 23 integer bits and 8 fractional bits, plus a sign bit. That
 * means the maximum len_y can be is 2^23, which amounts to about 180 seconds at
 * a sampling rate of 44100. The final value put into idx0 may not lie within
 * the bounds of the table y. This is okay because on successive calls to this
 * function, the index is always kept within bounds.
 * If len_y is 0, only 0s are written to y_.*/
void MM_interp_cubic_q_24_8_v(float *y_,
                              uint32_t len_y_,
                              float *y,
                              uint32_t len_y,
                              int32_t *idx0,
                              mm_q8_24_t rate)
{
    uint32_t n, idx0_;
    int32_t _idx0 = *idx0, M, _rate = (int32_t)(rate >> 16);
    float y_1, y0, y1, y2,
              frac1, frac0, frac_1, frac_2;
    /* len is only the integer part so shift it up by 8 bits */
    M = len_y;
    M <<= 8l;
    if (M == 0) {
        for (n = 0; n < len_y_; n++) {
            y_[n]=0;
            /* increment index */
            _idx0 += _rate;
        }
    } else {
        for (n = 0; n < len_y_; n++) {
            /* Ensure the index is within bounds */
            _idx0 -= (_idx0 / M - (_idx0 < 0)) * M;
            /* Compute the integer part of the index */
            idx0_ = (uint32_t)(_idx0 >> 8l);
            /* Compute the fractional part of the index */
            frac0 = ((float)(_idx0 & 0x000000ff)) / 256.0f;
            /* There seem to be no speed gains by using the commented out code. */
            //        y1 = y[(idx_0 + 1) * (idx0_ < (len_y-1))];
            //        y2 = y[(idx_0 + 2) - len_y*(idx0_ == (len_y-2))];
            y0 = y[idx0_];
            y1 = y[(idx0_ + 1) % len_y];
            y2 = y[(idx0_ + 2) % len_y];
            /* wrap negative index to beginning */
            idx0_ += (len_y - 1) * (idx0_ == 0) - (idx0_ > 0);
            y_1 = y[idx0_];
            /* store terms that are used more than once */
            frac1 = frac0 + 1.;
            frac_1 = frac0 - 1.;
            frac_2 = frac0 - 2.;
            /* do computation */
            y_[n] = frac1*frac_1*frac_2 * 0.5f*y0
                - frac0*frac_2*frac_1 * 0.166666667f*y_1
                - frac1*frac0*frac_2 * 0.5f*y1
                + frac1*frac0*frac_1 * 0.166666667f*y2; 
#ifdef CHECK_SAMPLE_IS_FINITE
            MMSample_assert_is_finite(y_[n]);
#endif
            /* increment index */
            _idx0 += _rate;
        }
    }
    *idx0 = _idx0;
}

/* More accurate rate parameters.
 * The same as MM_interp_cubic_q_24_8_v but rate is incremented by rate_inc each
 * sample so that the rate varies linearly over the the addressed points.
 * NOTE: rate and rint are in q8_24 format. */
void MM_interp_cubic_rinc_q_8_24_idx_q_24_8_v(float *y_,
                                              uint32_t len_y_,
                                              float *y,
                                              uint32_t len_y,
                                              int32_t *idx0,
                                              mm_q8_24_t *rate,
                                              mm_q8_24_t rinc,
                                              int32_t *index_err)
{
    uint32_t n, idx0_;
    int32_t _idx0 = *idx0, M, oldidx, adv, err = *index_err;
    mm_q8_24_t _rate = *rate, adj_rate;
    float y_1, y0, y1, y2,
              frac1, frac0, frac_1, frac_2;
    /* len is only the integer part so shift it up by 8 bits */
    M = len_y;
    M <<= 8l;
    if (M == 0) {
        for (n = 0; n < len_y_; n++) {
            y_[n]=0;
            /* increment index */
            oldidx = _idx0;
            adj_rate = _rate + err;
            _idx0 += (adj_rate >> 16);
            adv = _idx0 - oldidx;
            err = adj_rate - (adv << 16);
            _rate += rinc;
        }
    } else {
        for (n = 0; n < len_y_; n++) {
            /* Ensure the index is within bounds */
            _idx0 -= (_idx0 / M - (_idx0 < 0)) * M;
            /* Compute the integer part of the index */
            idx0_ = (uint32_t)(_idx0 >> 8l);
            /* Compute the fractional part of the index */
            frac0 = ((float)(_idx0 & 0x000000ff)) / 256.0f;
            /* There seem to be no speed gains by using the commented out code. */
            //        y1 = y[(idx_0 + 1) * (idx0_ < (len_y-1))];
            //        y2 = y[(idx_0 + 2) - len_y*(idx0_ == (len_y-2))];
            y0 = y[idx0_];
            y1 = y[(idx0_ + 1) % len_y];
            y2 = y[(idx0_ + 2) % len_y];
            /* wrap negative index to beginning */
            idx0_ += (len_y - 1) * (idx0_ == 0) - (idx0_ > 0);
            y_1 = y[idx0_];
            /* store terms that are used more than once */
            frac1 = frac0 + 1.;
            frac_1 = frac0 - 1.;
            frac_2 = frac0 - 2.;
            /* do computation */
            y_[n] = frac1*frac_1*frac_2 * 0.5f*y0
                - frac0*frac_2*frac_1 * 0.166666667f*y_1
                - frac1*frac0*frac_2 * 0.5f*y1
                + frac1*frac0*frac_1 * 0.166666667f*y2; 
#ifdef CHECK_SAMPLE_IS_FINITE
            MMSample_assert_is_finite(y_[n]);
#endif
            /* increment index */
            oldidx = _idx0;
            adj_rate = _rate + err;
            _idx0 += (adj_rate >> 16);
            adv = _idx0 - oldidx;
            err = adj_rate - (adv << 16);
            _rate += rinc;
        }
    }
    *idx0 = _idx0;
    *rate = _rate;
    *index_err = err;
}


/* This the same as MM_interp_cubic* except that linear interpolation
 * is used and so is lower quality. */
void MM_interp_linear_q24_8_v(float *y_,
                              float *y,
                              uint32_t len,
                              int32_t *idx0,
                              int32_t rate)
{
    uint32_t n, idx0_;
    int32_t _idx0 = *idx0, M;
    float y0, y1, frac;
    /* len is only the integer part so shift it up by 8 bits */
    M = len;
    M <<= 8l;
    for (n = 0; n < len; n++) {
        /* Ensure the index is within bounds */
//        _idx0 -= (_idx0 / M - ((0x80000000 & _idx0) >> 31l)) * M;
        _idx0 -= (_idx0 / M - (_idx0 < 0)) * M;
        /* Compute the integer part of the index */
        idx0_ = (uint32_t)(_idx0 >> 8l);
        /* Compute the fractional part of the index */
        frac = ((float)(_idx0 & 0x000000ff)) / 256.0f;
        y0 = y[idx0_];
        y1 = y[(idx0_ + 1) % len];
        y_[n] = y0 + frac*(y1 - y0);
        _idx0 += rate;
    }
    *idx0 = _idx0;
}
