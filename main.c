//
// Created by KeigoOgawa on 12/15/15.
//

#include <signal.h>
#include "my_structs.h"
#include "mysub.h"
#include "my_path.h"

int stdfd[2];

void changeRedirection(command *list, int old_fd[]) {
    int new_fd[2];
    new_fd[0] = old_fd[0];
    if (strlen(list->filename[0]) != 0) {
        if((new_fd[0] = open(list->filename[0], O_RDONLY)) == -1) {
            perror(list->filename[0]);
            exit(1);
        }
    }
    dup2(new_fd[0], 0);
    close(new_fd[0]);

    new_fd[1] = old_fd[1];
    if (strlen(list->filename[1]) != 0) {
        if((new_fd[1] = open(list->filename[1], O_RDWR)) == -1) {
            new_fd[1] = creat(list->filename[1], S_IRWXU);
        }
    }
    dup2(new_fd[1], 1);
    close(new_fd[1]);
}

void createChild(command *child) {
    pid_t pid;
    int fd[2], status;
    int saved_fd[0];
    command *parent = child->next;
    pipe(fd);
    if ((pid = fork()) == -1) {
        perror("fork()");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    } else if (pid == 0) {
        // 親の標準入力を子の標準出力先として扱う
        dup2(fd[1], 1);
        saved_fd[1] = dup(1);
        saved_fd[0] = dup(0);
        // もしリダイレクションがある場合、現在の設定からそちらに子の標準入力、標準出力を変更
        changeRedirection(child, saved_fd);
        close(fd[0]); close(fd[1]);
        close(saved_fd[0]); close(saved_fd[1]);
        // 子供を実行
        execvp(child->name, child->args);
        perror(child->name);
    } else {
        if (wait(&status) == (pid_t) -1) {
            perror("wait");
            close(fd[0]); close(fd[1]);
            exit(1);
        }
        // 子が変更した出力を標準出力に戻す
        dup2(stdfd[1], 1);
        // 子の出力結果がファイルにあるときはそちらを標準入力にする。そうでないときはパイプを標準入力にする
        if (strlen(child->filename[1]) != 0) {
            close(fd[0]);
            if((fd[0] = open(child->filename[1], O_RDONLY)) == -1) {
                perror(child->filename[1]);
                exit(1);
            }
        }
        dup2(fd[0], 0);
        saved_fd[1] = dup(1);
        saved_fd[0] = dup(0);
        // もしリダイレクションがある場合、現在の設定からそちらに親の標準入力、標準出力を変更
        changeRedirection(parent, saved_fd);
        close(fd[0]); close(fd[1]);
        close(saved_fd[0]); close(saved_fd[1]);

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
    command *list, *head = malloc(sizeof(command));
    init_path();
    int saved_fd[0];
    stdfd[0] = dup(0);
    stdfd[1] = dup(1);
    // 親プロセスではCtrl+cを無視する
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    sigemptyset (&act.sa_mask);
    sigaction (SIGINT, &act, NULL);

    while (true) {
        list = head;
        getstr(">> ", list, 1024);
        list = head->next;

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
            // 子プロセスではSIG_INTをデフォルトの動作にする
            act.sa_handler = SIG_DFL;
            sigaction (SIGINT, &act, NULL);
            if (list->next == NULL) {
                // 複数の命令がパイプされていない場合
                changeRedirection(list, saved_fd);
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
        }

        // メモリ解放
        command *used = list;
        int argi;
        while (used != NULL) {
            argi = 0;
            list = list->next;
            while (used->args[argi] != NULL) {
                free(used->args[argi]);
                argi++;
            }
            free(used);
            used = list;
        }
    }
}