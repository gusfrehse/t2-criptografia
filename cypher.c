#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

void printhelp(FILE *f, char *exename) {
    fprintf(f, "Usage:\n\t%s [plaintext]\n\t%s -d [cyphertext] [key]\n", exename, exename);
}

void encrypt(char *pt) {
}

void decrypt(char *cp, char *key) {
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
    if (decryptflag && (optind + 2 != argc) ||
        !decryptflag && (optind + 1 != argc)) {
        printhelp(stderr, argv[0]); 
        exit(1);
    }

    if (decryptflag)
        encrypt(argv[optind]);
    else
        decrypt(argv[optind], argv[optind + 1]); 
}
