//
// Created by KeigoOgawa on 12/15/15.
//

#include "my_structs.h"
#include "mysub.h"
#include "my_path.h"

void createChild(command *child) {
    pid_t pid;
    int fd[2], status;
    command *parent = child->next;
    pipe(fd);
    if ((pid = fork()) == -1) {
        perror("fork()");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    } else if (pid == 0) {
        // 子供の標準出力を親の標準入力にする
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        // 子供を実行
        execvp(child->name, child->args);
        perror(child->name);
    } else {
        if (wait(&status) == (pid_t) -1) {
            perror("wait");
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        // 親の標準入力を子供の標準出力にする
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        if (parent->next == NULL) {
            // 親の親がいないときは親を実行
            execvp(parent->name, parent->args);
            perror(parent->name);
            exit(1);
        } else {
            // 親の親がいるときは親を子供とみて再帰的に実行
            createChild(parent);
            perror("createChild");
            exit(1);
        }
    }
}

int main(void) {
    int pid, status;
    command *list = malloc(sizeof(command));
    init_path();

    while (true) {
        getstr(">> ", list, 1024);
        list = list->next;

        // exitで終了
        if (strcmp(list->name, "exit") == 0) {
            printf("bye...\n");
            exit(0);
        }
        // 空行が入力されたら何もしない
        if (strlen((list->name)) == 0) {
            continue;
        }

        // 子プロセスを作成して命令を実行
        if ((pid = fork()) == -1) {
            perror("fork()");
            return 1;
        } else if (pid == 0) {
            if (list->next == NULL) {
                // 複数の命令がパイプされていない場合
                execvp(list->name, list->args);
                perror(list->name);
                exit(1);
            } else {
                // パイプされている場合、さらに子供を作り順に実行する
                createChild(list);
            }
        } else {
            if (wait(&status) == (pid_t) -1) {
                perror("wait");
                exit(1);
            }
            list->next = NULL;
        }
    }
}