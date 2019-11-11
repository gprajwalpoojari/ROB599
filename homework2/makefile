bmp: bmp.c
			gcc -o $@ $^ -std=c11

image_server: image_server.c bmp.c
			gcc -o $@ $^ -std=c11 -lpthread
