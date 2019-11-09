#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *buffer = malloc(32);
    free(buffer);
    buffer[0] = 'H';
    buffer[1] = 'i';
    buffer[2] = '\0';
    printf("It still works: %s\n", buffer);

    char *buffer2 = malloc(32);
    printf("This should be different: %s\n", buffer2);
    free(buffer2);
}
