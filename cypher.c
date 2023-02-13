#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "encrypt.h"
#include "decrypt.h"

#define HEAP_SIZE 1024

#define BUFFER_SIZE 32

void printhelp(FILE *f, char *exename) {
    fprintf(f, "Usage:\n\t%s [-d] <key>\n", exename);
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
        decrypt(argv[optind], strlen(argv[optind]), BUFFER_SIZE);
    else
        encrypt(argv[optind], strlen(argv[optind]), BUFFER_SIZE); 
}
