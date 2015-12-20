//
// Created by KeigoOgawa on 12/19/15.
//

#ifndef MYSH_MY_STRUCTS_H
#define MYSH_MY_STRUCTS_H

#include <stdio.h> /* fprintf */
#include <sys/types.h> /* fork */
#include <unistd.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <string.h>
#include <sys/fcntl.h>

typedef enum {
    false,
    true
} Bool;

typedef struct Command {
    char name[10];
    char* args[10];
    struct Command *next;
    char filename[2][10];
} command;

#endif //MYSH_MY_STRUCTS_H
