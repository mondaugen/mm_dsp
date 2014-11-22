BIN = mm_esp-test-1.elf
SRCS = mm_esp-test-1.c ../src/mm_trapezoid_env.c \
	../../mm_primitives/src/mm_rationalfunc.c ../src/mm_enveloper.c \
	../src/mm_sigproc.c ../src/mm_sigchain.c ../src/mm_sigconst.c ../src/mm_bus.c \
	../src/mm_envedsampleplayer.c ../src/mm_trapenvedsampleplayer.c \
	../src/mm_busmerger.c ../src/mm_sampleplayer.c ../src/mm_wrap.c
INC = -I../inc -I../../mm_primitives/inc
LDFLAGS = -lm
CC = gcc
CFLAGS = -ggdb3
$(BIN) : $(SRCS)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LDFLAGS)


