#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

struct node {
    int data;
    struct node *next;
} *head;

void insert(struct node *newt) {
    struct node *node = head, *prev = NULL;
    while (node != NULL && node->data < newt->data) {
        prev = node;
        node = node->next;
    }
    newt->next = node;
    if (prev == NULL)
        head = newt;
    else
        prev->next = newt;
}

void insert_pp(struct node *newt)
{
    struct node **link = &head;
    while (*link && (*link)->data < newt->data)
        link = &(*link)->next;
    newt->next = *link;
    *link = newt;
}

#define LIST_SIZE 10000
#define USE_POINTER_TO_POINTER 0

int main()
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < LIST_SIZE; i++) {
        struct node *new_node = malloc(sizeof(struct node));

        new_node->data = i;
        new_node->next = NULL;

#if USE_POINTER_TO_POINTER
        insert_pp(new_node);
#else
        insert(new_node);
#endif
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    struct node *node = head, *prev;
    for (int i = 0; i < LIST_SIZE; i++) {
        //printf("%d data = %d\n", i, node->data);
        prev = node;
        node = node->next;
        free(prev);
    }
    
    long time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("time = %ld\n", time);
}
