BIN = mm_dsp-cubic-interp-test.elf 
$(BIN): mm_dsp-cubic-interp-test.c
	gcc $^ -o $@ -I../inc

preprocess :
	gcc mm_dsp-cubic-interp-test.c -I../inc -E

clean :
	rm $(BIN) 
