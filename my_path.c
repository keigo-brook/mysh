//
// Created by KeigoOgawa on 12/15/15.
//

#include "my_path.h"

int path_num = 0;


// グローバル変数pathに.pathに書いてあるパスを保存する
void init_path() {
    int i, fd;
    ssize_t read_size;
    char tmp[MAX_PATH_LEN];
    for (i = 0; i < MAX_MY_PATH; ++i) {
        *path[i] = 0;
    }

    fd = open(".path", O_RDONLY);
    if (fd < 0) {
        // TODO .path open error
    }

    // TODO when length > MAX_PATH_LEN, while loop
    read_size = read(fd, tmp, MAX_PATH_LEN);
    if (read_size < 0) {
        // TODO .path read error
    }

    close(fd);

    // pathは':'で終わるので、そこで区切る
    int tp = 0, pi = 0;
    while (tmp[tp] != '\0') {
        if (tmp[tp] == ':') {
            // path一つ分が終わり
            path[path_num][pi] ='/';
            path[path_num][pi + 1] = '\0';
//            printf("input path %s\n", path[path_num]);
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


// グローバル変数pathに順番にnameをくっつけて、その名前のファイルが存在するか確かめる
// 存在した場合、nameを上書きしてtrueを返す
Bool get_path(char *name) {
    int i, fd;
    char tmp[256];

    for (i = 0; i < path_num; ++i) {
        sprintf(tmp, "%s%s", path[i], name);
        fd = open(tmp, O_RDONLY);
        if (fd > 0) {
            close(fd);
            strcpy(name, tmp);
            return true;
        }
        close(fd);
    }
    return false;
}

