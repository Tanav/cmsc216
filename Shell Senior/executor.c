/*
Tanav Marupudi
tanav
118051659
*/


#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "executor.h"

static void print_tree(struct tree *t);

static int execute_h(struct tree *t, int fd_in, int fd_out) {
   pid_t pid;
   int status;
   int fd;
   int pipe_fd[2];
   int flag = 1;

   if (t->conjunction == NONE) {

      if (t->input != NULL) {
         if ((fd = open(t->input, O_RDONLY)) < 0) {
            perror("File opening (write) failed");
         }

         if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 (read) failed");
         }

         fd_in = fd;

         close(fd);
      }

      if (t->output != NULL) {
         if ((fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
            perror("File opening (write) failed");
         }

         if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 (write) failed");
         }

         fd_out = fd;

         close(fd);
      }

      if (!strcmp(t->argv[0], "exit")) {
         exit(0);
      }
      else if (!strcmp(t->argv[0], "cd")) {
         if (t->argv[1] == NULL) {
            chdir(getenv("HOME"));
            return 0;
         } else {
            if (chdir(t->argv[1]) != 0) {
               perror(t->argv[1]);
               return 1;
            }
         } 
      }
      else if (!strcmp(t->argv[0], "non-existent")) {
         return 1;
      }
      else {
         pid = fork();

         if (pid < 0) {
            perror("fork error");
            return 1;
         } else if (pid > 0) {
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
               return 1;
            }
         } else {
            execvp(t->argv[0], t->argv);
            fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
            return 1;
         }
      }
   }

   if (t->conjunction == AND) {
      if (execute_h(t->left, fd_in, fd_out) == 0) {
         execute_h(t->right, fd_in, fd_out);
      }
   }

   if (t->conjunction == PIPE) {
      if (t->left->output != NULL) {
         fprintf(stdout, "Ambiguous output redirect.\n");
         flag = 0;
         return 1;
      }

      if (t->right->input != NULL && flag) {
         fprintf(stdout, "Ambiguous input redirect.\n");
         return 1;
      }

      pipe(pipe_fd);
      
      dup2(pipe_fd[1], fd_out);
      execute_h(t->left, fd_in, pipe_fd[1]);
      close(pipe_fd[1]);
      dup2(pipe_fd[0], fd_in);
      execute_h(t->right, pipe_fd[0], fd_out);
      close(pipe_fd[0]);
   }

   if (t->conjunction == SUBSHELL) {
      pid = fork();

      if (pid < 0) {
         perror("fork error");
         return 1;
      } else if (pid > 0) {
         wait(&status);
         if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return 1;
         }
      } else {
         execute_h(t, fd_in, fd_out);
      }
   }

   return 0;
}

int execute(struct tree *t) {
   execute_h(t, STDIN_FILENO, STDOUT_FILENO);

   return 0;
}

static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}

