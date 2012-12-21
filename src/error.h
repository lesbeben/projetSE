#ifndef ERROR_H
#define ERROR_H

/**
 * Si 'value' > 0
 *     affiche 'str' sur la sortie d'erreur et quitte le programme
 */
void check_error(int value, const char* str);

/**
 * Si 'value' > 0
 *     passe str � la fonction 'perror' et quitte le programme
 */
void check_error2(int value, const char* str);

#endif
