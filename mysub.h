//
// Created by KeigoOgawa on 12/14/15.
//

#ifndef MYSH_MYSUB_H
#define MYSH_MYSUB_H
#include <string.h>
#include <stdio.h>
#include <sys/syslimits.h>

static void discardline(FILE *fp);
void getstr(char *prompt, char *s[], int slen);

#endif //MYSH_MYSUB_H
