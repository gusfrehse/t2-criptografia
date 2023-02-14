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

// Compress a key of unsigned chars
// keylen^2 complexity
void compresskey(unsigned char *key, int keylen) {
    unsigned int *buffer;

    if (!(buffer = calloc(keylen, sizeof(unsigned int)))) {
        perror("compresskey buffer");
        exit(1);
    }

    for (int i = 0; i < keylen; i++) {
        buffer[i] = key[i];
    }
}

static unsigned char getkey(int i, unsigned char *key, int keylen) {
    return key[((i % keylen) + keylen) % keylen];
}

// collapse key values to ckey = [0..len-1]^len, keeping order.
int nextbigger(unsigned char *key, int keylen, unsigned char *used) {
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

int minkey(unsigned char *key, int keylen) {
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
void collapsekey(unsigned char *key, int keylen) {
    unsigned char *newkey = malloc(keylen);
    assert(newkey);

    unsigned char *used = malloc(keylen);
    assert(used);

    memset(used, 0, keylen);

    //int minind = minkey(key, keylen);

    for (int i = 0; i < keylen; i++) {
        int ind = nextbigger(key, keylen, used); 
        newkey[ind] = i;
        used[ind] = 1;
    }

    memcpy(key, newkey, keylen);

    free(newkey);
}

void decrypt(unsigned char *key, int keylen, long long buffersize) {
    unsigned char *backbuffer = malloc(buffersize);
    assert(backbuffer);

    unsigned char *presentbuffer = malloc(buffersize);
    assert(presentbuffer);

    unsigned char *expandedkey = malloc(buffersize);
    assert(expandedkey);

    int read;

    for (int i = 0; i < buffersize; i++) {
        expandedkey[i] = getkey(i, key, keylen);
    }

    collapsekey(expandedkey, buffersize);

    read = fread(presentbuffer, 1, buffersize, stdin);

    while ((read = fread(backbuffer, 1, buffersize, stdin)) > 0) {
        for (int i = 0; i < buffersize; i++) {
            unsigned char c = presentbuffer[expandedkey[i]];
            fputc(c, stdout); 
        }

        // swap backbuffer and presentbuffer
        unsigned char *tmp = presentbuffer;
        presentbuffer = backbuffer;
        backbuffer = tmp;
    }

    int end = 0;
    for (int i = 0; i < buffersize; i++) {
        unsigned char c = presentbuffer[expandedkey[i]];

        if (c == '-') {
            for (int j = i + 1; j < buffersize; j++) {
                if (presentbuffer[expandedkey[j]] == '=') {
                    end = 1;
                    break;
                }
            }
        }

        if (end)
            break;

        fputc(c, stdout); 
    }

    free(backbuffer);
    free(presentbuffer);
    free(expandedkey);
}

