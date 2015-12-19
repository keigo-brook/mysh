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
    int arg_count = 0;
    char *tmp1 = strtok(tmp, " ");
    if (tmp1 == NULL) {
        strcpy(cmd->name, "\0");
    } else {
        strcpy(cmd->name, tmp1);
        if (get_path(cmd->name)) {
            tmp1 = strtok(NULL, " ");
            cmd->args[arg_count] = malloc(sizeof(char)*10);
            strcpy(cmd->args[0], cmd->name);
            arg_count++;
            while (tmp1 != NULL) {
                if(strlen(tmp1) < slen){
                    cmd->args[arg_count] = malloc(sizeof(char)*10);
                    strcpy(cmd->args[arg_count], tmp1);
                    arg_count++;
                }else{
                    // TODO PATH＿MAX以上の数が入力された時の処理
                    //        tmp = strtok(buf, " ");
                    //        strcpy(s[cmd_count++], tmp);
                    //        s[cmd_count + 1] = '\0';
                    //        strncpy(s, buf, slen-1);
                    //        s[slen-1] = '\0';
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
            new_command->next = NULL;
            if(strlen(tmp1) < slen){
                getCmd(new_command, tmp1, slen);
            }else{
                // TODO PATH＿MAX以上の数が入力された時の処理
                //        tmp = strtok(buf, " ");
                //        strcpy(s[cmd_count++], tmp);
                //        s[cmd_count + 1] = '\0';
                //        strncpy(s, buf, slen-1);
                //        s[slen-1] = '\0';
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