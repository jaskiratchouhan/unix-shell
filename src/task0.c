#include "../include/msgs.h"
#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void zombieProcess() {
  int wstatus = 0;
  while (waitpid(-1, &wstatus, WNOHANG) > 0) {
  };
}

void sighandler(int signum) {

  write(STDOUT_FILENO, FORMAT_MSG("help", HELP_HELP_MSG),
        strlen(FORMAT_MSG("help", HELP_HELP_MSG)));
  write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
        strlen(FORMAT_MSG("cd", CD_HELP_MSG)));
  write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
        strlen(FORMAT_MSG("pwd", PWD_HELP_MSG)));
  write(STDOUT_FILENO, FORMAT_MSG("exit", EXIT_HELP_MSG),
        strlen(FORMAT_MSG("exit", EXIT_HELP_MSG)));
  write(STDOUT_FILENO, FORMAT_MSG("history", HISTORY_HELP_MSG),
        strlen(FORMAT_MSG("history", HISTORY_HELP_MSG)));
  // char *currentD = getcwd(NULL, 0);
  // if (currentD) {
  // write(STDOUT_FILENO, currentD, strlen(currentD));
  // write(STDOUT_FILENO, "$ ", 2);
  // free(currentD);
  // }
}
static char prevDir[100] = "";
int prev = 0;

char *history[10];
int commandNum = 0;
int historyCount = 0;
void add_to_history(char *args[], int background) {
  char entry[100];
  int i = 0;
  entry[0] = '\0';
  while (args[i] != NULL) {
    strcat(entry, args[i]);
    strcat(entry, " ");
    i++;
  }
  //  char buffer[5];
  // int len = snprintf(buffer, sizeof(buffer), "%d\n", background);
  // write(STDOUT_FILENO, buffer, len);
  if (background == 1) {
    strcat(entry, "&");
    strcat(entry, "&");
    // write(STDOUT_FILENO, "try", 3);
  }
  // remove last space
  if (strlen(entry) > 0) {
    entry[strlen(entry) - 1] = '\0';
  }
  // if (historyCount >= 10) {
  // free(history[0]);
  // for (int i = 1; i < 10; i++) {
  //  history[i - 1] = history[i];
  // }
  // history[9] = NULL;

  // historyCount = 9;
  // write(STDOUT_FILENO, entry, strlen(entry));
  history[commandNum % 10] = strdup(entry);
  // history[historyCount] = strdup(input);
  commandNum++;
  // historyCount++;
}

void print_history() {
  // if theres 30 commands currently i want to
  // print 30, 29, 28 , etc so
  // but if theres 4 we want to print ONLY four so
  // doing if 4- 10 == < 0 then set it to 4
  // int index;
  // if (commandNum - 10 < 0) {
  // index = commandNum;
  //} else {
  // index = 10;
  // }
  // int start = commandNum - index;
  int maxCommand = commandNum;
  if (commandNum > 10) {
    maxCommand = 10;
  }
  for (int i = 0; i < maxCommand; i++) {
    // int num = start - i;
    // if command num = 12 you want it to map to 10 so subtract
    int num = (commandNum - 1 - i + 10) % 10;
    // if (num <= 0 || history[historyindex] == NULL) {

    // continue;
    // }

    char output[100];
    snprintf(output, sizeof(output), "%d\t%s\n", commandNum - 1 - i,
             history[num]);
    write(STDOUT_FILENO, output, strlen(output));
  }
}

int shellCommand(char *input, int background, char *token, char *saveptr) {

  // int len = strlen(input);
  // printf("%d", len);

  // static char prevDir[100] = ""
  char *args[1000];
  int i = 0;
  while (token != NULL) {
    args[i++] = token;
    // get the next arg
    token = strtok_r(NULL, " ", &saveptr);
  }
  args[i] = NULL;
  add_to_history(args, background);
  if (strcmp(args[0], "history") == 0) {
    if (i == 1) {
      // add_to_history(input);
      print_history();
    }
  }
  if (args[0][0] == '!') {
    if (isdigit(args[0][1])) {
      int thenum = commandNum;
      int targetNum = atoi(args[0] + 1);
      int found = 0;
      for (int i = 0; i < 10; i++) {
        if (targetNum == thenum) {
          // run nth command
          char historyCommand[100];
          strncpy(historyCommand, history[thenum % 10],
                  sizeof(historyCommand) - 1);
          historyCommand[sizeof(historyCommand) - 1] = '\0';

          char *newtoken;
          char *historyArgs[100];
          int argCount = 0;
          char *savedPtr;
          newtoken = strtok_r(historyCommand, " ", &savedPtr);
          while (newtoken != NULL) {
            historyArgs[argCount++] = newtoken;
            newtoken = strtok_r(NULL, " ", &savedPtr);
          }
          historyArgs[argCount] = NULL;
          // write(STDERR_FILENO, "in", 2);
          execvp(historyArgs[0], historyArgs);
          found = 1;
          break;
        }
        thenum--;
      }

      if (!found) {
        write(STDERR_FILENO, FORMAT_MSG("history", HISTORY_INVALID_MSG),
              strlen(HISTORY_INVALID_MSG));
      }
      // check if valid number
      // run nth command
    } else {
      write(STDERR_FILENO, FORMAT_MSG("history", HISTORY_INVALID_MSG),
            strlen(FORMAT_MSG("history", HISTORY_INVALID_MSG)));
    }
    return 0;
  }

  if (strcmp(args[0], "exit") == 0) {
    if (i > 1) {
      write(STDERR_FILENO, FORMAT_MSG("exit", TMA_MSG),
            strlen(FORMAT_MSG("exit", TMA_MSG)));
    } else {

      exit(0);
    }
    return 0;
  }

  if (strcmp(args[0], "pwd") == 0) {
    if (i > 1) {
      write(STDERR_FILENO, FORMAT_MSG("pwd", TMA_MSG),
            strlen(FORMAT_MSG("pwd", TMA_MSG)));
    } else {
      char *currentD = getcwd(NULL, 0);
      if (currentD != NULL) {
        write(STDOUT_FILENO, currentD, strlen(currentD));
        write(STDOUT_FILENO, "\n", 1);
        free(currentD);
      } else {
        write(STDERR_FILENO, FORMAT_MSG("pwd", GETCWD_ERROR_MSG),
              strlen(FORMAT_MSG("pwd", GETCWD_ERROR_MSG)));
      }
    }
    return 0;
  }

  if (strcmp(args[0], "cd") == 0) {

    if (i > 2) {
      // printf("in i >2");
      write(STDERR_FILENO, FORMAT_MSG("cd", TMA_MSG),
            strlen(FORMAT_MSG("cd", TMA_MSG)));
    } else {
      if ((i == 1) || (strcmp(args[1], "~") == 0 && i == 2)) {
        struct passwd *path = getpwuid(getuid());
        char *homedir = path->pw_dir;
        int val = chdir(homedir);
        if (val == -1) {
          write(STDERR_FILENO, FORMAT_MSG("cd", CHDIR_ERROR_MSG),
                strlen(FORMAT_MSG("cd", CHDIR_ERROR_MSG)));
        }
        return 0;
      }

      char currentDir[100];
      getcwd(currentDir, sizeof(currentDir));
      // printf("in else");
      if (args[1][0] == '-') {
        chdir(prevDir);
        return 0;
      }

      if (i == 2) {
        // printf("in i==2");
        // prevDir = args[1];
        int val = chdir(args[1]);
        if (val == -1) {
          // write(STDOUT_FILENO, &args[1][0], 1);
          write(STDOUT_FILENO, "in t", 3);
          write(STDERR_FILENO, FORMAT_MSG("cd", CHDIR_ERROR_MSG),
                strlen(FORMAT_MSG("cd", CHDIR_ERROR_MSG)));
        }
      }
      // Set it to home directory
      if ((i == 1) || (strcmp(args[1], "~") == 0 && i == 2)) {
        // write(STDOUT_FILENO, "in i==1 str cmp", (15));
        struct passwd *path = getpwuid(getuid());
        char *homedir = path->pw_dir;
        int val = chdir(homedir);
        // prevDir = args[1];
        if (val == -1) {
          write(STDERR_FILENO, FORMAT_MSG("cd", CHDIR_ERROR_MSG),
                strlen(FORMAT_MSG("cd", CHDIR_ERROR_MSG)));
        }
      }
      // if cd ~tmp make it go to tmp in homedirectory
      if (args[1][0] == '~' && args[1][1] != '\0') {
        // write(STDOUT_FILENO, "IN", 2);
        struct passwd *path = getpwuid(getuid());
        char *homedir = path->pw_dir;
        char *fullpath = malloc(strlen(homedir) + strlen(args[1]) + 1);
        strcpy(fullpath, homedir);
        strcat(fullpath, "/");
        strcat(fullpath, args[1] + 1);
        // prevDir = fullpath;
        //  write(STDOUT_FILENO, fullpath, strlen(fullpath));
        int val = chdir(fullpath);
        if (val == -1) {

          write(STDERR_FILENO, FORMAT_MSG("cd", CHDIR_ERROR_MSG),
                strlen(FORMAT_MSG("cd", CHDIR_ERROR_MSG)));
        }
        free(fullpath);
      }
      // if (args[1][0] == '-') {
      //  write(STDOUT_FILENO, prevDir, strlen(prevDir));
      // chdir(prevDir);
      // prev = 1;
      //}
      strcpy(prevDir, currentDir);
    }
    return 0;
  }
  if (strcmp(args[0], "help") == 0) {

    if (i == 1) {
      write(STDOUT_FILENO, FORMAT_MSG("help", HELP_HELP_MSG),
            strlen(FORMAT_MSG("help", HELP_HELP_MSG)));
      write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
            strlen(FORMAT_MSG("cd", CD_HELP_MSG)));
      write(STDOUT_FILENO, FORMAT_MSG("exit", EXIT_HELP_MSG),
            strlen(FORMAT_MSG("exit", EXIT_HELP_MSG)));
      write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
            strlen(FORMAT_MSG("pwd", PWD_HELP_MSG)));
      write(STDOUT_FILENO, FORMAT_MSG("history", HISTORY_HELP_MSG),
            strlen(FORMAT_MSG("history", HISTORY_HELP_MSG)));
      return 0;
    }
    if (i > 2) {
      write(STDOUT_FILENO, FORMAT_MSG("help", TMA_MSG),
            strlen(FORMAT_MSG("help", TMA_MSG)));
    } else if (strcmp(args[1], "cd") == 0) {
      write(STDOUT_FILENO, FORMAT_MSG("cd", CD_HELP_MSG),
            strlen(FORMAT_MSG("cd", CD_HELP_MSG)));
    } else if (strcmp(args[1], "pwd") == 0) {
      write(STDOUT_FILENO, FORMAT_MSG("pwd", PWD_HELP_MSG),
            strlen(FORMAT_MSG("pwd", PWD_HELP_MSG)));
    } else if (strcmp(args[1], "exit") == 0) {
      write(STDOUT_FILENO, FORMAT_MSG("exit", EXIT_HELP_MSG),
            strlen(FORMAT_MSG("exit", EXIT_HELP_MSG)));
    } else {

      char try[100];
      sprintf(try, "%s: %s\n", args[1], EXTERN_HELP_MSG);
      write(STDOUT_FILENO, try, strlen(try));

      // write(STDOUT_FILENO, try, strlen(try));
      //  write(STDOUT_FILENO, FORMAT_MSG(args[1], EXTERN_HELP_MSG),
      //      strlen(FORMAT_MSG(args[1], EXTERN_HELP_MSG)));
    }

    return 0;
  }
  pid_t pid = fork();
  // Child process
  if (pid == 0) {

    execvp(args[0], args);
    // if execvp fails then we reach this line and print the fail marco line
    write(STDERR_FILENO, FORMAT_MSG("shell", EXEC_ERROR_MSG),
          strlen(FORMAT_MSG("shell", EXEC_ERROR_MSG)));
    exit(EXIT_FAILURE);
  }
  // Parent process
  if (pid < 0) {
    write(STDERR_FILENO, FORMAT_MSG("shell", FORK_ERROR_MSG),
          strlen(FORMAT_MSG("shell", FORK_ERROR_MSG)));
  }
  // if (background == 1) {
  //  char *currentD = getcwd(NULL, 0);
  // if (currentD != NULL) {
  // write(STDOUT_FILENO, currentD, strlen(currentD));
  // write(STDOUT_FILENO, "$ ", 2);
  // free(currentD);
  //}
  // return 0;

  // if a foreground process we wait for it to clean it up
  if (background == 0) {
    int wstatus = 0;
    pid_t wpid = waitpid(pid, &wstatus, 0);

    if (wpid == -1) {
      write(STDERR_FILENO, FORMAT_MSG("shell", WAIT_ERROR_MSG),
            strlen(FORMAT_MSG("shell", WAIT_ERROR_MSG)));
    }
  } else {
    usleep(45000);
  }
  return 0;
}

int main() {
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL);

  while (1) {
    int background = 0;
    char *currentD = getcwd(NULL, 0);
    if (currentD != NULL) {
      write(STDOUT_FILENO, currentD, strlen(currentD));
      write(STDOUT_FILENO, "$ ", 2);
      free(currentD);
    } else {

      write(STDERR_FILENO, FORMAT_MSG("shell", GETCWD_ERROR_MSG),
            strlen(FORMAT_MSG("shell", GETCWD_ERROR_MSG)));
    }
    char input[1000];
    char *saveptr;
    // if (currentD != NULL) {
    // }
    zombieProcess();
    // else {
    // write(STDOUT_FILENO, FORMAT_MSG("shell", GETCWD_ERROR_MSG),
    //        strlen(FORMAT_MSG("shell", GETCWD_ERROR_MSG)));
    // }
    // Avoid zombie processes incase a background process
    // zombieProcess();
    ssize_t command = read(STDIN_FILENO, input, sizeof(input) - 1);

    if (command <= 0) {
      if (errno == EINTR) {
        continue;
      }
      write(STDERR_FILENO, FORMAT_MSG("shell", READ_ERROR_MSG),
            strlen(FORMAT_MSG("shell", READ_ERROR_MSG)));
    }

    else {
      input[command - 1] = '\0';
      // if (input[strlen(input) -1] == '&' && input[strlen(input) -2] == ' '){
      // background =1;
      // input[
      //}
      // Is a background process
      if (input[strlen(input) - 1] == '&') {
        background = 1;
        input[strlen(input) - 1] = '\0';
      }
      char *token = strtok_r(input, " ", &saveptr);
      shellCommand(input, background, token, saveptr);

      // if (background == 1) {
      // printf("in");
      //}
    }
  }
}
