//
// Created by KeigoOgawa on 12/19/15.
//

#ifndef MYSH_MY_PATH_H
#define MYSH_MY_PATH_H

#include "my_structs.h"


#define MAX_MY_PATH 20
#define MAX_PATH_LEN 256
char path[MAX_MY_PATH][50];
void init_path();
Bool get_path(char *command);


#endif //MYSH_MY_PATH_H
