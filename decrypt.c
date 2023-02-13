#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

static char getkey(int i, char *key, int keylen) {
    return key[((i % keylen) + keylen) % keylen];
}

// collapse key values to ckey = [0..len-1]^len, keeping order.
// then, for each c \in ckey get ct[c] and put in the j pos of pt such that ckey[j].val = ct[c] and ckey[j].used = 0 

int nextbigger(char *key, int keylen, char *used) {
    int minval = CHAR_MAX;
    int minind = -1;

    for (int i = 0; i < keylen; i++) {
        if (!used[i] && key[i] < minval) {
            minind = i;
            minval = key[i];
        }
    }

    return minind;
}

int minkey(char *key, int keylen) {
    int min = key[0];
    int minind = 0;
    for (int i = 1; i < keylen; i++) {
        if (key[i] < min) {
            minind = i;
            min = key[i];
        }
    }

    return minind;
}

// toodo
void collapsekey(char *key, int keylen) {
    char *newkey = malloc(keylen);
    assert(newkey);

    char *used = malloc(keylen);
    assert(used);

    memset(used, 0, keylen);

    int minind = minkey(key, keylen);

    for (int i = 0; i < keylen; i++) {
        int ind = nextbigger(key, keylen, used); 
        newkey[ind] = i;
        used[ind] = 1;
    }

    memcpy(key, newkey, keylen);

    free(newkey);
}

void decrypt(char *key, int keylen, long long buffersize) {
    char *buffer = malloc(buffersize);
    assert(buffer);

    char *expandedkey = malloc(buffersize);
    assert(expandedkey);

    int read;

    for (int i = 0; i < buffersize; i++) {
        expandedkey[i] = getkey(i, key, keylen);
    }

    printf("expandedkey: ");
    for (int i = 0; i < buffersize; i++) {
        printf("%d ", expandedkey[i]);
    }
    printf("\n");

    collapsekey(expandedkey, buffersize);

    printf("expandedkey: ");
    for (int i = 0; i < buffersize; i++) {
        printf("%d ", expandedkey[i]);
    }
    printf("\n");

    while ((read = fread(buffer, 1, buffersize, stdin)) > 0) {
        for (int i = 0; i < read; i++) {
            char k = getkey(i, key, keylen);
            fputc(buffer[key[i]], stdout); 
            fprintf(stderr, "i: %d key[i]: %d buffer[key[i]]: %c\n", i, key[i], buffer[key[i]]);
        }
    }
}

