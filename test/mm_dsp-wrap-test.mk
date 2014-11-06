BIN = mm_dsp-wrap-test.elf 
SRCS = mm_dsp-wrap-test.c ../src/mm_wrap.c
INC = -I../inc
$(BIN): $(SRCS)
	gcc $^ -o $@ $(INC) -lm

preprocess :
	gcc $(SRCS) -I../inc -E

clean :
	rm $(BIN) 
