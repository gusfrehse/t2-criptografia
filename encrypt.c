#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 32

struct val {
    int weight;
    int chr;
};

char getkey(int i, char *key, int keylen) {
    return key[((i % keylen) + keylen) % keylen];
}

void insert(struct val* arr, int arrsize, struct val v) {
    int i;
    for (i = 0; arr[i].weight <= v.weight && i < arrsize; i++);

    for (int j = arrsize; j > i; j--)
        arr[j] = arr[j - 1];

    arr[i] = v;
}

void encrypt(char *key, int keylen, long long buffersize) {
    char *buffer;
    struct val *arr;
    int read;

    if (!(buffer = malloc(buffersize))) {
        perror("encrypt buffer");
        exit(1);
    }

    if (!(buffer = calloc(buffersize, sizeof(struct val)))) {
        perror("encrypt arr");
        exit(1);
    }

    while ((read = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
        for (int i = 0; i < read; i++) {
            char k = getkey(i, key, keylen);

            insert(arr, i, (struct val){ .weight = k, .chr = buffer[i] });
        }

        for (int i = 0; i < read; i++) {
            fputc(arr[i].chr, stdout); 
        }
    }

    free(buffer);
    free(arr);
}

