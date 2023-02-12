#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "encrypt.h"

#define HEAP_SIZE 1024

void printhelp(FILE *f, char *exename) {
    fprintf(f, "Usage:\n\t%s [-d] <key>\n", exename);
}

void insertdecrypt(struct decryptval* arr, int arrsize, struct decryptval v) {
    int i;
    for (i = 0; arr[i].weight <= v.weight && i < arrsize; i++);

    for (int j = arrsize; j > i; j--)
        arr[j] = arr[j - 1];

    arr[i] = v;
}

void printarr(struct val *v, int size) {
    for (int i = 0; i < size; i++) {
        printf("%3d ", v[i].weight);
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("  %c ", v[i].chr);
    }
    printf("\n");
}

void sortkey(char *key, int keylen) {
    if (keylen <= 1)
        return;

    for (int i = 1; i < keylen; i++) {
        int j;
        for (j = i; key[j - 1] >= key[j] && j >= 0; j--) {
            char tmp = key[j];
            key[j] = key[j - 1];
            key[j - 1] = tmp;
        }
    }
}

int main(int argc, char **argv) {
    int decryptflag = 0;
    int c;

    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
            case 'd':
                decryptflag = 1;
                break;
            case '?':
                printhelp(stderr, argv[0]);
            default:
                exit(1);
        }
    }

    // check if number of arguments is correct
    if (optind + 1 != argc) {
        printhelp(stderr, argv[0]); 
        exit(1);
    }

    if (decryptflag)
        decrypt(argv[optind], strlen(argv[optind]));
    else
        encrypt(argv[optind], strlen(argv[optind])); 
}
