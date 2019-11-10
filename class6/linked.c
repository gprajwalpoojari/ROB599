#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *prev;
    struct node *next;
    int value;
} node_t;

typedef struct list {
    node_t *start;
    node_t *end;
} list_t;

list_t *list_create(void) {
  list_t *list_tmp = malloc(sizeof(list_tmp));
  list_tmp->start = malloc(sizeof(list_tmp->start));
  list_tmp->end = malloc(sizeof(list_tmp->end));
  list_tmp->start->prev = list_tmp->start;
  list_tmp->start->next = list_tmp->end;
  list_tmp->end->next = list_tmp->end;
  list_tmp->end->prev = list_tmp->start;
  return list_tmp;
}
void list_push_start(list_t *list, int value) {
  node_t *tmp = malloc(sizeof(tmp));
  if (list->start->next == list->end && list->end->prev == list->start){
    tmp->value = value;
    tmp->next = list->end;
    tmp->prev = list->start;
    list->start->next = tmp;
    list->end->prev = tmp;
  }
  else{
    tmp->value = value;
    tmp->next = list->start->next;
    tmp->prev = list->start;
    list->start->next->prev = tmp;
    list->start->next = tmp;
  }
}

void list_push_end(list_t *list, int value) {
  node_t *tmp = malloc(sizeof(tmp));
  if (list->start->next == list->end && list->end->prev == list->start){
    tmp->value = value;
    tmp->next = list->end;
    tmp->prev = list->start;
    list->start->next = tmp;
    list->end->prev = tmp;
  }
  else{
    tmp->value = value;
    tmp->next = list->end;
    tmp->prev = list->end->prev;
    list->end->prev->next = tmp;
    list->end = tmp;
  }
}

int list_pop_start(list_t *list) {
  if (list->start->next == list->end && list->end->prev == list->start){
    fprintf(stderr, "No elements present to pop");
    exit(0);
  }
  else{
    node_t *temp = list->start->next;
    int val = temp->value;
    list->start->next = list->start->next->next;
    list->start->next->prev = list->start;
    free(temp);
    return(val);
  }
}

int list_pop_end(list_t *list) {
  if (list->start->next == list->end && list->end->prev == list->start){
    fprintf(stderr, "No elements present to pop\n");
    exit(0);
  }
  else{
    node_t *temp = list->end->prev;
    int val = temp->value;
    list->end->prev = list->end->prev->prev;
    list->end->prev->next = list->end;
    free(temp);
    return(val);
  }
}

void list_destroy(list_t *list) {
  node_t *temp1 = list->start;
  node_t *temp2 = list->start->next;
  while (temp2 != list->end){
    free(temp1);
    temp1 = temp2;
    temp2 = temp2->next;
  }
  free(temp1);
  free(temp2);

}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "could not open %s: ", argv[1]);
        perror("");
        return 1;
    }

    list_t *list = list_create();

    while (!feof(f)) {
        char command[16];
        int res = fscanf(f, "%10s", command);
        if (res != 1) {
            if (!feof(f)) {
                fprintf(stderr, "trouble parsing command. stopping.\n");
            }
            break;
        }

        if (strcmp("push_start", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_start(list, value);
        } else if (strcmp("push_end", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_end(list, value);
        } else if (strcmp("pop_start", command) == 0) {
            int v = list_pop_start(list);
            printf("%d\n", v);
        } else if (strcmp("pop_end", command) == 0) {
            int v = list_pop_end(list);
            printf("%d\n", v);
        } else {
            fprintf(stderr, "did not recognize command '%s'. stopping.\n", command);
            break;
        }
    }

    list_destroy(list);

    fclose(f);

    return 0;
}
