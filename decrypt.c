#include <stdio.h>
#include <stdlib.h>

struct decryptval {
    int weight;
    int chr;
    int visited;
};

// Compress a key of chars
// keylen^2 complexity
void compresskey(char *key, int keylen) {
    int current;
    unsigned int *buffer;

    if (!(buffer = calloc(keylen, sizeof(unsigned int)))) {
        perror("compresskey buffer");
        exit(1);
    }

    for (int i = 0; i < keylen; i++) {
        buffer[i] = key[i];
    }
}

// collapse key values to ckey = [0..len-1]^len, keeping order.
// then, for each c \in ckey get ct[c] and put in the j pos of pt such that ckey[j].val = ct[c] and ckey[j].used = 0 

int nextbigger(char *key, int keylen, int start) {
    int minind = start;
    int minval = key[start];
    for (int i = start; i < keylen; i++) {
        if (key[i] >= start && key[i] < minval) {
            minind = i;
            minval = key[i];
        }
    }

    return minind;
}

// toodo
void collapsekey(char *key, int keylen) {
    for (int i = 0; i < key; i++) {
        nextbigger(key, keylen, i); 
    }
}

void decrypt(char *key, int keylen, long long buffersize) {
    char *buffer;
    struct decryptval *arr;
    int read;

    if (!(buffer = malloc(buffersize))) {
        perror("encrypt buffer");
        exit(1);
    }

    if (!(buffer = calloc(buffersize, sizeof(struct decryptval)))) {
        perror("encrypt arr");
        exit(1);
    }

    while ((read = fread(buffer, 1, buffersize, stdin)) > 0) {
        for (int i = 0; i < read; i++) {
            char k = getkey(i, key, keylen);

            insertdecrypt(arr, i, (struct decryptval){ .weight = k, .chr = buffer[i], .visited = 0});
        }

        for (int i = 0; i < read; i++) {
            for (int j = 0; j < read; j++) {
            } 
        }

        for (int i = 0; i < read; i++) {
            fputc(arr[i].chr, stdout); 
        }
    }
}

