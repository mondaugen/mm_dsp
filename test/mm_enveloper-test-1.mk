BIN = mm_enveloper-test-1.elf
SRCS = mm_enveloper-test-1.c ../src/mm_trapezoid_env.c ../../mm_primitives/src/mm_rationalfunc.c \
	   ../src/mm_enveloper.c ../src/mm_sigproc.c ../src/mm_sigchain.c ../src/mm_sigconst.c ../src/mm_bus.c
INC = -I../inc -I../../mm_primitives/inc
LDFLAGS = -lm
CC = gcc
$(BIN) : $(SRCS)
	$(CC) $(INC) $^ -o $@ $(LDFLAGS)


