//
// Created by KeigoOgawa on 12/15/15.
//

#include "main.h"

int main(void) {
    int pid, status;
    char cmd[ARGS_MAX][PATH_MAX];
    char *te[ARGS_MAX];
    init_path();

    while(true) {
        for (int i = 0; i < ARGS_MAX; ++i) {
            te[i] = &cmd[i][0];
        }
        getstr(">> ", te, 1024);

        // exitで終了
        if (strcmp(te[0], "exit") == 0) {
            printf("bye...\n");
            exit(0);
        }

        // 空行が入力されたら何もしない
        if (strlen(te[0]) == 0) {
            continue;
        }


        if ((pid = fork()) == -1) {
            perror("fork()");
            return 1;
        } else if (pid == 0) {
            // child
            if (get_path(te[0])) {
                execvp(te[0], te);
                perror(te[0]);
                exit(1);
            }
        } else if (pid > 0) {
            // parent
            if (wait(&status) == (pid_t) -1) {
                perror("wait");
                exit(1);
            }
        }
    }
}