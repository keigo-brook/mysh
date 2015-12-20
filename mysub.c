//
// Created by KeigoOgawa on 12/14/15.
//


#include <stdlib.h>
#include "mysub.h"
#include "my_path.h"

static void discardline(FILE *fp){
    int c;

    while((c = getc(fp)) != EOF){
        if(c == '\n'){
            break;
        }
    }
}

Bool isReservedName(char *name) {
    if (strcmp(name , "exit") == 0) {
        return true;
    }
    return false;
}

void getCmd(command *cmd, char *tmp, int slen) {
    int arg_count = 0, fd[2];

    // 命令1 引数1 引数2 ... の形になっているので、空白で分割する
    char *tmp1 = strtok(tmp, " ");
    if (tmp1 == NULL) {
        strcpy(cmd->name, "\0");
    } else {
        strcpy(cmd->name, tmp1);
        // 命令の名前をパスから検索して、見つかった場合引数も保存していく
        if (get_path(cmd->name)) {
            // 引数の一つ目にはコマンド名を入れる
            cmd->args[0] = malloc(sizeof(char)*10);
            strcpy(cmd->args[0], cmd->name);
            arg_count++;

            tmp1 = strtok(NULL, " ");
            while (tmp1 != NULL) {
                if(strlen(tmp1) < slen){
                    if (strcmp(tmp1, ">") == 0) {
                        strcpy(cmd->filename[1], strtok(NULL, " "));
                    } else if (strcmp(tmp1, "<") == 0) {
                        strcpy(cmd->filename[0], strtok(NULL, " "));
                    } else {
                        cmd->args[arg_count] = malloc(sizeof(char)*10);
                        strcpy(cmd->args[arg_count], tmp1);
                        arg_count++;
                    }
                }else{
                    // TODO PATH＿MAX以上の数が入力された時の処理
                }
                tmp1 = strtok(NULL, " ");
            }
            cmd->args[arg_count] = NULL;
        } else if (isReservedName(cmd->name) == false) {
            printf("mysh: command not found: %s\n", cmd->name);
            strcpy(cmd->name, "\0");
        }
    }
}

char *strsplit(char *str, const char *divider) {
    if (!str) {
        return NULL;
    }
    char *div_p = strstr(str, divider);
    if (!div_p) {
        return NULL;
    }
    *div_p = '\0';
    return div_p + strlen(divider);
}

void getstr(char *prompt, command *head, int slen) {
    char *p, *tmp, buf[PATH_MAX];

    // 文字列取得
    if(slen > PATH_MAX){
        fprintf(stderr,
                "getstr: buffer length must be <= %d (%d specified);"
                        " proceeding anyway\n", PATH_MAX, slen);
    }
    fputs(prompt, stderr);
    fgets(buf, PATH_MAX, stdin);
    // 行末の改行読み捨て
    if((p = strchr(buf, '\n')) == NULL){
        discardline(stdin);
    }else{
        *p = '\0';
    }

    // 文字列が以下の形になっているのでパイプで前から分割していく
    // [命令1, 引数1, 引数2] | [命令2, 引数1, 引数2] | ...
    char *tmp1;
    tmp1 = buf;
    tmp = strsplit(buf, "|");

    command *cmd = head;
    if (tmp1 == NULL) {
        strcpy(cmd->name, "\0");
        cmd->next = NULL;
    } else {
        while (tmp1 != NULL) {
            command *new_command = malloc(sizeof(command));
            strcpy(new_command->filename[0], "");
            strcpy(new_command->filename[1], "");

            new_command->next = NULL;
            if(strlen(tmp1) < slen){
                getCmd(new_command, tmp1, slen);
            }else{
                // TODO PATH_MAX以上の数が入力された時の処理
            }
            tmp1 = tmp;
            tmp = strsplit(tmp1, "|");
            if (tmp1 == NULL) {
                tmp1 = tmp;
                tmp = NULL;
            }

            cmd->next = new_command;
            cmd = cmd->next;
        }
    }
}