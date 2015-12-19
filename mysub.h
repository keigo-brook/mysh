//
// Created by KeigoOgawa on 12/14/15.
//
#ifndef MYSH_MYSUB_H
#define MYSH_MYSUB_H

#include <stdio.h>
#include "my_structs.h"

static void discardline(FILE *fp);
void getstr(char *prompt, command *head, int slen);

#endif //MYSH_MYSUB_H
