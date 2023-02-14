#include "encrypt.h"

#include <stdio.h>
#include <stdlib.h>

#define PADDING_START_CHAR '-'
#define PADDING_END_CHAR '='


struct val {
    int weight;
    int chr;
};

static char getkey(int i, char *key, int keylen) {
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

    if (!(arr = calloc(buffersize, sizeof(struct val)))) {
        perror("encrypt arr");
        exit(1);
    }

    int padded = 0;
    while (1) {
        read = fread(buffer, 1, buffersize, stdin);

        if (read < buffersize) {
            padded = 1;
            // need to add padding
            // fill out with the size of buffersize - read
            buffer[read] = PADDING_START_CHAR;
            for (int i = read + 1; i < buffersize; i++) {
                buffer[i] = PADDING_END_CHAR;
            }
        }

        for (int i = 0; i < buffersize; i++) {
            char k = getkey(i, key, keylen);

            insert(arr, i, (struct val){ .weight = k, .chr = buffer[i] });
        }

        for (int i = 0; i < buffersize; i++) {
            fputc(arr[i].chr, stdout); 
        }

        if (padded)
            break;
    }

    free(buffer);
    free(arr);
}

