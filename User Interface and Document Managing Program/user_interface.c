/*
Tanav Marupudi
118051659
tanav
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <sysexits.h>
#include "document.h"

#define MAX_LINE_LENGTH 1024

static int check_blank_line(const char *str);
static int check_digit(const char *str);
static int count_quotes(const char *str);
static void remove_quotes(char *str);

int main(int argc, char *argv[]) {
    FILE *file;
    Document doc;
    int fields, ast_index, quotes = 0;
    char line[MAX_LINE_LENGTH + 1], file_name[MAX_LINE_LENGTH + 1],
        command[MAX_LINE_LENGTH + 1], para_num[MAX_LINE_LENGTH + 1],
        line_num[MAX_LINE_LENGTH + 1], store[MAX_LINE_LENGTH + 1],
        target[MAX_LINE_LENGTH + 1], replacement[MAX_LINE_LENGTH + 1], *ptr;
    unsigned int i = 0;

    /* Chooses how code will execute depending on amount of arguments
    passed, runs using stdin if no file name passed, file passed runs using
    file, returns errors if more than 2 arguments or file cannot be found */
    if (argc == 1) {
        printf("> ");
        file = stdin;
    } else if (argc == 2) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            return EX_OSERR;
        }
    } else if (argc > 2) {
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        return EX_USAGE;
    }

    /* Intializes document using "main_document" and grabs entire input */
    init_document(&doc, "main_document");
    fgets(line, MAX_LINE_LENGTH + 1, file);

    while (!feof(file)) {
        /* Edits line to equal before newline character
        to help find the command name within the line */
        line[strlen(line) - 1] = '\0';
        fields = sscanf(line, "%s", command);

        /* Runs code if line is not a comment and not blank */
        if (*command != '#' && !check_blank_line(line) && fields != -1) {

            /* Decides how to edit document depending on command's value */
            if (strstr(command, "add_paragraph_after") != NULL) {
                /* Adds paragraph */
                fields = sscanf(line, "%s %s %s", command, para_num, store);
                
                /* Checks all error cases before accessing document */
                if (check_digit(para_num) && atoi(para_num) > -1
                                    && check_digit(para_num) && fields == 2) {
                    if (add_paragraph_after(&doc, atoi(para_num)) == FAILURE) {
                        printf("add_paragraph_after failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "add_line_after") != NULL) {
                /* Adds line */
                fields = sscanf(line, "%s %s %s %s", command, para_num,
                                                            line_num, store);
                
                /* Sets pointer to the index after asterisk in line */
                for (i = 0; i < strlen(line); i++) {
                    if (line[i] == '*') {
                        ast_index = i;
                        ptr = &line[ast_index + 1];
                    }
                }

                /* Checks all error cases before accessing document */
                if (check_digit(para_num) && atoi(para_num) > -1 &&
                    check_digit(line_num) && atoi(line_num) > -1 &&
                                            ast_index != -1 && fields == 4) {
                    if (add_line_after(&doc, atoi(para_num),
                                        atoi(line_num), ptr) == FAILURE) {
                        printf("add_line_after failed\n");
                    }
                } else {
                    printf("Invalid Comment\n");
                }
            } else if (strstr(command, "print_document") != NULL) {
                /* Prints document */
                fields = sscanf(line, "%s %s", command, store);
                
                /* Checks all error cases before accessing document */
                if (fields == 1) {
                    print_document(&doc);
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "quit") != NULL ||
                                        strstr(command, "exit") != NULL) {
                /* Terminates run */
                fields = sscanf(line, "%s %s", command, store);

                /* Checks all error cases before accessing document */
                if (fields == 1) {
                    return 0;
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "append_line") != NULL) {
                /* Appends line */
                fields = sscanf(line, "%s %s %s", command, para_num, store);
                
                /* Sets pointer to the index after asterisk in line */
                for (i = 0; i < strlen(line); i++) {
                    if (line[i] == '*') {
                        ast_index = i;
                        ptr = &line[ast_index + 1];
                    }
                }

                /* Checks all error cases before accessing document */
                if (check_digit(para_num) && atoi(para_num) > -1 && 
                                            ast_index != -1 && fields == 3) {
                    if (append_line(&doc, atoi(para_num), ptr) == FAILURE) {
                        printf("append_line failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "remove_line") != NULL) {
                /* Removes line */
                fields = sscanf(line, "%s %s %s %s", command, para_num,
                                                            line_num, store);

                /* Checks all error cases before accessing document */
                if (check_digit(para_num) && atoi(para_num) > -1 &&
                    check_digit(line_num) && atoi(line_num) > -1 &&
                                                                fields == 3) {
                    if (remove_line(&doc, atoi(para_num),
                                                atoi(line_num) == FAILURE)) {
                        printf("remove_line failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "load_file") != NULL) {
                /* Loads file */
                fields = sscanf(line, "%s %s %s", command, file_name, store);

                /* Checks all error cases before accessing document */
                if (fields == 2) {
                    if (load_file(&doc, file_name) == FAILURE) {
                        printf("load_file failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "replace_text") != NULL) {
                /* Replaces text */

                /* Runs only if proper amount of quotes found */
                if (count_quotes(line) == 4) {
                    /* Sets target without quotation marks */
                    strcpy(target, strchr(line, '"'));
                    remove_quotes(target);
                    
                    /* Sets pointer past first set of quotation marks */
                    quotes = 0;
                    for (i = 0; i < strlen(line); i++) {
                        if (line[i] == '"') {
                            quotes++;

                            if (quotes == 2) {
                                ptr = &line[++i];
                            }
                        }
                    }

                    /* Sets replacement without quotation marks */
                    strcpy(replacement, strchr(ptr, '"'));
                    remove_quotes(replacement);

                    if (replace_text(&doc, target, replacement) == FAILURE) {
                        printf("replace_text failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "highlight_text") != NULL) {
                /* Highlights text */

                /* Runs only if proper amount of quotes found */
                if (count_quotes(line) == 2) {
                    strcpy(target, strchr(line, '"'));
                    remove_quotes(target);
                    highlight_text(&doc, target);
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "remove_text") != NULL) {
                /* Removes text */

                /* Runs only if proper amount of quotes found */
                if (count_quotes(line) == 2) {
                    strcpy(target, strchr(line, '"'));
                    remove_quotes(target);
                    remove_text(&doc, target);      
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "save_document") != NULL) {
                /* Saves document */
                fields = sscanf(line, "%s %s %s", command, file_name, store);

                /* Checks all error cases before accessing document */
                if (fields == 2) {
                    if (save_document(&doc, file_name) == FAILURE) {
                        printf("save_document failed\n");
                    }
                } else {
                    printf("Invalid Command\n");
                }
            } else if (strstr(command, "reset_document") != NULL) {
                /* Resets document */
                fields = sscanf(line, "%s %s", file_name, store);

                /* Checks all error cases before accessing document */
                if (fields == 1) {
                    reset_document(&doc);
                } else {
                    printf("Invalid Command\n");
                }
            } else {
                /* If command is spelled incorrectly */
                printf("Invalid Command\n");
            }
        }

        if (argc == 1) {
            printf("> ");
        }

        /* Reset variables */
        strcpy(command, "");
        strcpy(store, "");
        ast_index = -1;
        ptr = NULL;

        fgets(line, MAX_LINE_LENGTH + 1, file);
    }

    fclose(file);

    return 0;
}

/* Passes string and returns 0 if it is a blank line, 1 if not */
static int check_blank_line(const char *str) {
    unsigned int i;

    if (*str == '\0' || *str == '\n') {
        return 0;
    }

    for (i = 0; i < strlen(str); i++) {
        if (isspace(str[i] == 0)) {
            return 1;
        }
    }

    return 0;
}

/* Passes string and returns 1 if it contains a digit, 0 if not */
static int check_digit(const char *str) {
    unsigned int i;
    
    for (i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }

    return 1;
}

/* Passes string and returns amount of quotation marks in it */
static int count_quotes(const char *str) {
    unsigned int i = 0;
    int count = 0;

    for (i = 0; i < strlen(str); i++) {
        if (str[i] == '"') {
            count++;
        }
    }

    return count;
}

/* Passes string and method removes quotation marks */
static void remove_quotes(char *str) {
    unsigned int i;

    for (i = 0; i < strlen(str); i++) {
        str[i] = str[i + 1];
    }

    for (i = 0; i < strlen(str); i++) {
        if (str[i] == '"') {
            str[i] = '\0';
            return;
        }
    }
}