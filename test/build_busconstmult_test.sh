#! /bin/bash
gcc busconstmult_test.c \
../src/mm_busconstmult.c \
../src/mm_bus.c \
../src/mm_sigproc.c \
-I../inc \
-I../../mm_primitives/inc \
-o busconstmult_test.bin \
-ggdb3 -lm
