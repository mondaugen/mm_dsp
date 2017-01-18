#! /bin/bash
gcc interp_cubic_rinc_q_24_8_v_test.c \
../src/mm_interp.c \
-I../inc \
-o interp_cubic_rinc_q_24_8_v_test.bin \
-ggdb3 -lm
