#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char file_buffer[64*1024];

char *get_context(char *key, char **argv, char *file_buffer);

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }
    int bytes_read = fread(file_buffer, 1, sizeof(file_buffer) - 1, f);
    if (!feof(f)) {
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves
    file_buffer[bytes_read] = '\0';
    char *key = file_buffer;
    while (1){
      key = strstr(key, argv[2]);
      if (key != NULL){
        char *char_ptr = get_context(key, argv, file_buffer);
        printf("%s\n",char_ptr);
        ++key;
      }
      else{
        break;
      }
    }
    return 0;
}

char *get_context(char *key, char **argv, char *file_buffer){
  int key_len = strlen(argv[2]);
  int n_lines = atoi(argv[3]);
  char *char_ptr = key + key_len;
  int context_len = 0;
  char *temp;
  //locate start pointer of context and number of characters
  if (n_lines == 0){
      char_ptr = key;
      temp = malloc((key_len + 2) * sizeof(char));
      for (int i = 0; i < key_len; i++){
        temp[i] = char_ptr[i];
      }
    }
  else{
    for (int current_line = 0; current_line < n_lines; current_line++){
      if (*char_ptr == '\n'){
        if (char_ptr == file_buffer){
            break;
          }
        else{
            char_ptr--;
            context_len++;
          }
      }
      while (*char_ptr != '\n'){
        if (char_ptr == file_buffer){
          break;
        }
        else{
          char_ptr--;
          context_len++;
        }
      }
    }
    temp = malloc((context_len+2) * sizeof(char));
    for (int i = 0; i < context_len; i++){
        temp[i] = char_ptr[i];
      }
  }
  return temp;
  free(temp);
}
