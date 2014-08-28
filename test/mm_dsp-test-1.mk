BASE = $(HOME)/Documents/development
SRCS = $(BASE)/mm_dsp/test/mm_dsp-test-1.c \
	   $(BASE)/mm_dsp/src/mm_sampleplayer.c \
	   $(BASE)/mm_dsp/src/mm_sigchain.c \
	   $(BASE)/mm_dsp/src/mm_sigproc.c
INC  = $(BASE)/mm_primitives/inc \
	   $(BASE)/mm_dsp/inc \
	   /usr/local/include

CC   = clang
BIN  = mm_dsp-test-1.elf
CFLAGS = $(addprefix -I,$(INC)) -g3
LDFLAGS = -L/usr/local/lib -lsndfile

$(BIN) : $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
