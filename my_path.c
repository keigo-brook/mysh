//
// Created by KeigoOgawa on 12/15/15.
//

#include "main.h"

int path_num = 0;


void init_path() {
    int i, fd;
    ssize_t read_size;
    char tmp[MAX_PATH_LEN];
    for (i = 0; i < MAX_MY_PATH; ++i) {
        *path[i] = NULL;
    }

    fd = open(".path", O_RDONLY);
    if (fd < 0) {
        // TODO .path open error
    }
    //    printf("%d\n", fd);


    // TODO when length > MAX_PATH_LEN, while loop
    read_size = read(fd, tmp, MAX_PATH_LEN);
    if (read_size < 0) {
        // TODO .path read error
    }
    //    printf("%d\n", read_size);

    close(fd);

    // pathは':'で終わるので、そこで区切る
    int tp = 0, pi = 0;
    while (tmp[tp] != '\0') {
        if (tmp[tp] == ':') {
            // path一つ分が終わり
            path[path_num][pi] ='/';
            path[path_num][pi + 1] = '\0';
            printf("input path %s\n", path[path_num]);
            path_num++;
            pi = 0;
            // TODO pnum > MAX_PNUM
        } else {
            path[path_num][pi] = tmp[tp];
            pi++;
        }
        tp++;
    }
}


Bool get_path(char *command) {
    int i, fd;
    char tmp[256];

    for (i = 0; i < path_num; ++i) {
        sprintf(tmp, "%s%s", path[i], command);
        fd = open(tmp, O_RDONLY);
        if (fd > 0) {
            close(fd);
            strcpy(command, tmp);
            return true;
        }
        close(fd);
    }

    printf("mysh: command not found: %s\n", command);
    return false;
}

