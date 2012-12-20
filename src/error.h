#ifndef ERROR_H
#define ERROR_H

/**
 * Si 'value' > 0
 *     affiche 'str' sur la sortie d'erreur et quitte le programme
 */
void check_error(int value, const char* str) {
    if (value > 0) {
        fprintf(stderr, "%s", str);
        exit(EXIT_FAILURE);
    }
}

/**
 * Si 'value' > 0
 *     passe str à la fonction 'perror' et quitte le programme
 */
void check_error2(int value, const char* str) {
    if (value > 0) {
        perror(str);
        exit(EXIT_FAILURE);
    }
}

#endif