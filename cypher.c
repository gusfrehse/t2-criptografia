#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 32

#define HEAP_SIZE 1024

struct val {
    int weight;
    int chr;
};

struct decryptval {
    int weight;
    int chr;
    int visited;
};

void printhelp(FILE *f, char *exename) {
    fprintf(f, "Usage:\n\t%s [-d] <key>\n", exename);
}

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

void encrypt(char *key, int keylen) {
    char buffer[BUFFER_SIZE];

    struct val arr[BUFFER_SIZE];
    int read;

    while ((read = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
        //printf("read %d\n", read);
        for (int i = 0; i < read; i++) {
            //printarr(arr, i);
            char k = getkey(i, key, keylen);

            insert(arr, i, (struct val){ .weight = k, .chr = buffer[i] });
        }
        //printarr(arr, read);

        for (int i = 0; i < read; i++) {
            fputc(arr[i].chr, stdout); 
        }
    }
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

void decrypt(char *key, int keylen) {
    sortkey(key, keylen); 
    printf("ascii: %s\n", key);

    char buffer[BUFFER_SIZE];

    struct decryptval arr[BUFFER_SIZE];

    int read;
    while ((read = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
        for (int i = 0; i < read; i++) {
            char k = getkey(i, key, keylen);

            insertdecrypt(arr, i, (struct decryptval){ .weight = k, .chr = buffer[i], .visited = 0});
        }

        for (int i = 0; i < read; i++) {
            for (int j = 0; j < read; j++) {
                :w
            } 
        }

        for (int i = 0; i < read; i++) {
            fputc(arr[i].chr, stdout); 
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
