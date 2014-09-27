BASE = $(HOME)/Documents/development
SRCS = $(BASE)/mm_dsp/test/mm_adsr-test-1.c \
	   $(BASE)/mm_dsp/src/mm_sampleplayer.c \
	   $(BASE)/mm_dsp/src/mm_sigchain.c \
	   $(BASE)/mm_dsp/src/mm_sigproc.c \
	   $(BASE)/mm_dsp/src/mm_adsr.c \
	   $(BASE)/mm_dsp/src/mm_sigconst.c
INC  =  $(BASE)/mm_primitives/inc \
		$(BASE)/mm_dsp/inc \
	   /usr/local/include

CC   = gcc 
BIN  = mm_adsr-test-1.elf
CFLAGS = $(addprefix -I,$(INC)) -g3
LDFLAGS = -L/usr/local/lib 

$(BIN) : $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean :
	rm $(BIN)
