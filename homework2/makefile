bmp:	bmp.c
		gcc	-o	$@	$^	-std=c11

image_server:	image_server.c	bmp.c
		gcc	-o	$@	$^	-std=c11	-lpthread

CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow -Wfloat-conversion
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

rasterize:	rasterize.c	bmp.c	image_server.c	vector_xy_i32_t.c vector_xy_t.c
		gcc -o	$@	$^	$(CFLAGS)	-lpthread	-lm
