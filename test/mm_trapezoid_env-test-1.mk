BIN = mm_trapezoid_env_test-1.elf
SRCS = mm_trapezoid_env-test-1.c ../src/mm_trapezoid_env.c ../../mm_primitives/src/mm_rationalfunc.c
INC = -I../inc -I../../mm_primitives/inc
LDFLAGS = -lm
CC = gcc
$(BIN) : $(SRCS)
	$(CC) $(INC) $^ -o $@ $(LDFLAGS)


