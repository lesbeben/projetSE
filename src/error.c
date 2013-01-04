#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

int check_error_noquit(int value, const char* str) {
    if (value > 0) {
        fprintf(stderr, "%s", str);
        return 1;
    }
    return 0;
}

void check_error(int value, const char* str) {
    if (value > 0) {
        fprintf(stderr, "%s", str);
        raise(SIGTERM);
    }
}

void check_error2(int value, const char* str) {
    if (value > 0) {
        perror(str);
        raise(SIGTERM);
    }
}
