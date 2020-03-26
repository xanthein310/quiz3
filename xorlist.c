#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

typedef struct __list list;
struct __list {
    int data;
    struct __list *addr;
};

#define XOR(a, b) ((list *) ((uintptr_t) (a) ^ (uintptr_t) (b)))

void insert_node(list **l, int d) {
    list *tmp = malloc(sizeof(list));
    tmp->data = d;

    if (!(*l)) {
        tmp->addr = NULL;
    } else {
        (*l)->addr = XOR(tmp, (*l)->addr);
        tmp->addr = *l;
    }
    *l = tmp;
}

void delete_list(list *l) {
    while (l) {
        list *next = l->addr;
        if (next)
            next->addr = XOR(next->addr, l);
        free(l);
        l = next;
    }
}

list *insert_sort(list *start)
{
    list *merge = NULL;
    list *l = start;

    while (l) {
        list *next = l->addr;
        if (next)
            next->addr = XOR(next->addr, l);

        //find place to insert
        if (!merge) {
            merge = l;
            merge->addr = NULL;
        } else {
            list *search = merge;
            list *prev = NULL;

            while (search && (l->data > search->data)) {
                list *next_search = XOR(search->addr, prev);
                prev = search;
                search = next_search;
            }

            if (search) {
                l->addr = XOR(prev, search);
                if (prev)
                    prev->addr = XOR(XOR(prev->addr, search), l);
                else
                    merge = l;
                search->addr = XOR(l, XOR(prev, search->addr));
            } else {
                l->addr = prev;
                prev->addr = XOR(prev->addr, l);
            }
        }

        l = next;
    }
    return merge;
}

list *opt_sort(list *start, int size, int S)
{
    if (!start || !start->addr)
        return start;

    if (size < S)
        return insert_sort(start);

    int half_size = size / 2;

    list *left = start, *left_end = start, *right;
    list *prev = NULL;
    for (int i = 0; i < half_size - 1; i++) {
        list *next = XOR(left_end->addr, prev);
        prev = left_end;
        left_end = next;
    }

    right = XOR(left_end->addr, prev);
    right->addr = XOR(right->addr, left_end);
    left_end->addr = prev;

    left = opt_sort(left, half_size, S);
    right = opt_sort(right, size - half_size, S);

    for (list *merge = NULL; left || right;) {
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;
            if (next)
                next->addr = XOR(left, next->addr);

            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            } else {
                merge->addr = XOR(merge->addr, left);
                left->addr = merge;
                merge = left;
            }
            left = next;
        } else {
            list *next = right->addr;
            if (next)
                next->addr = XOR(right, next->addr);

            if (!merge) {
                start = merge = right;
                merge->addr = NULL;
            } else {
                merge->addr = XOR(merge->addr, right);
                right->addr = merge;
                merge = right;
            }
            right = next;
        }
    }

    return start;
}

list *sort(list *start)
{
    if (!start || !start->addr)
        return start;

    list *left = start, *right = start->addr;
    left->addr = NULL;
    right->addr = XOR(right->addr, left);

    left = sort(left);
    right = sort(right);

    for (list *merge = NULL; left || right;) {
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;
            if (next)
                next->addr = XOR(left, next->addr);

            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            } else {
                merge->addr = XOR(merge->addr, left);
                left->addr = merge;
                merge = left;
            }
            left = next;
        } else {
            list *next = right->addr;
            if (next)
                next->addr = XOR(right, next->addr);

            if (!merge) {
                start = merge = right;
                merge->addr = NULL;
            } else {
                merge->addr = XOR(merge->addr, right);
                right->addr = merge;
                merge = right;
            }
            right = next;
        }
    }

    return start;
}

int main()
{
    list *list_ = NULL, *l;
    struct timespec start, end;
    FILE *file_time;
    char buffer[32];

    file_time = fopen("time.txt", "w+");

    for (int count=0; count<10000; count+=50) {
        for (int i=0; i<count; i++)
            insert_node(&list_, i);

            clock_gettime(CLOCK_MONOTONIC, &start);
            sort(list_);
            clock_gettime(CLOCK_MONOTONIC, &end);

        long time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

        int size = snprintf(buffer, sizeof(buffer), "%d %lu\n", count, time);
        fwrite(buffer, 1, size, file_time);

        delete_list(list_);
        list_ = NULL;
    }

    fclose(file_time);
}
