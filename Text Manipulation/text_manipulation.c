#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

int remove_spaces(const char *source, char *result, int *num_spaces_removed) {
    int i = 0, j = 0, empty = 0;
    int* spaces;

    if (source == NULL || strlen(source) == 0) {
        return FAILURE;
    }

    if (num_spaces_removed == NULL) {
        spaces = &empty;
    } else {
        spaces = num_spaces_removed;
    }

    *spaces = 0;

    while (source[i] && source[i] == ' ') {
        (*spaces)++;
        i++;
    }

    while (source[i]) {
        result[j] = source[i];
        i++;
        j++;
    }

    i = strlen(result) - 1;
    while (result[i] && result[i] == ' ') {
        (*spaces)++;
        i--;
    }
    result[i + 1] = '\0';

    return SUCCESS;
}

int center(const char *source, int width, char *result) {
    int i, spaces, length = strlen(source);

    if (source == NULL || strlen(source) == 0 || width < length) {
        return FAILURE;
    }

    spaces = (width - length) / 2;

    for (i = 0; i < spaces; i++) {
        result[i] = ' ';
    }

    strcat(result, source);
    
    for (i = spaces + length; i < spaces + spaces + length; i++) {
        result[i] = ' ';
    }
    result[i + 1] = '\0';

    return SUCCESS;
}