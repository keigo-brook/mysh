//
// Created by KeigoOgawa on 12/15/15.
//

#ifndef MYSH_MAIN_H
#define MYSH_MAIN_H

#include <stdio.h> /* fprintf */
#include <sys/types.h> /* fork */
#include <unistd.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <string.h>
#include <sys/fcntl.h>

#include "mysub.h"

#define ARGS_MAX 10

typedef enum {
    false,
    true
} Bool;

#endif //MYSH_MAIN_H

// my_path.c
#ifndef MYSH_MY_PATH_H
#define MYSH_MY_PATH_H

#define MAX_MY_PATH 20
#define MAX_PATH_LEN 256

char path[MAX_MY_PATH][50];
void init_path();
Bool get_path(char *command);

#endif //MYSH_MY_PATH_H
