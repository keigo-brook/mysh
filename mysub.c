//
// Created by KeigoOgawa on 12/14/15.
//


#include <stdlib.h>
#include "mysub.h"

static void discardline(FILE *fp){
    int c;

    while((c = getc(fp)) != EOF){
        if(c == '\n'){
            break;
        }
    }
}

void getstr(char *prompt, char *s[], int slen) {
    int cmd_count = 0;
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

    tmp = strtok(buf, " ");
    if (tmp == NULL) {
        strcpy(s[0], "\0");
    } else {
        while (tmp != NULL) {
            if(strlen(tmp) < slen){
                strcpy(s[cmd_count++], tmp);
            }else{
                // TODO PATH＿MAX以上の数が入力された時の処理
                //        tmp = strtok(buf, " ");
                //        strcpy(s[cmd_count++], tmp);
                //        s[cmd_count + 1] = '\0';
                //        strncpy(s, buf, slen-1);
                //        s[slen-1] = '\0';
            }
            tmp = strtok(NULL, " ");
        }
        s[cmd_count] = NULL;
    }

}