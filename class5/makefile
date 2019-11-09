test: test.c
			gcc -o $@ $^ -std=c11


CFLAGS += -fsanitize=address -fsanitize=undefined
golomb: golomb.c
			gcc -o $@ $^ -std=c11
