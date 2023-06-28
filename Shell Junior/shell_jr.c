/*
Tanav Marupudi
tanav
118051659
*/

#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 1024

int main() {
    char line[MAX_LEN + 1], one[MAX_LEN + 1], two[MAX_LEN + 1], *argv[3];
    pid_t pid;
    int flag = 0;

    printf("shell_jr: ");
    fflush(stdout);

    if (fgets(line, MAX_LEN + 1, stdin) == NULL) {
        flag = 1;
    }
    sscanf(line, "%s %s", one, two);

    if (!strcmp(one, "exit") || !strcmp(one, "hastalavista")) {
        printf("See you\n");
        fflush(stdout);
        exit(0);
    }

    while (flag != 1) {
        if (!strcmp(one, "cd")) {
            if (chdir(two) != 0) {
                printf("Cannot change directory to %s", two);
                fflush(stdout);
            }
        } else {
            pid = fork();

            if (pid < 0) {
                err(EX_OSERR, "fork error");
            } else if (pid > 0) {
                wait(NULL);
            } else {
                argv[0] = one;
                argv[1] = two;
                argv[2] = NULL;

                execvp(argv[0], argv);

                printf("Failed to execute %s\n", one);
                fflush(stdout);
                exit(EX_OSERR);
            }
        }

        memset(line, 0, MAX_LEN);
        memset(one, 0, MAX_LEN);
        memset(two, 0, MAX_LEN);

        printf("shell_jr: ");
        fflush(stdout);

        if (fgets(line, MAX_LEN + 1, stdin) == NULL) {
            flag = 1;
        }
        sscanf(line, "%s %s", one, two);

        if (!strcmp(one, "exit") || !strcmp(one, "hastalavista")) {
            printf("See you\n");
            fflush(stdout);
            exit(0);
        }
    }

    return 0;
}