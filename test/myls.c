#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>

int cmp(const void *a, const void *b) {
    return strcmp(a, b);
}

int cmp_len(const void *a, const void *b) {
    return strlen(a) < strlen(b);
}

int main(){
    DIR *dp;
    dp = opendir(".");
    char files[10000][100], str[100];
    struct dirent *dir;
    int cnt = 0;
    while((dir=readdir(dp)) != NULL) {
        strcpy(str, dir -> d_name);
        if(str[0] == '.' || !str) continue;
        strcpy(files[cnt++], str);
    }
    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);
    qsort(files, cnt, sizeof(files[0]), cmp_len);
    int col_num = 0, len = -2;
    while (len <= win.ws_col && col_num < cnt) {
        len += strlen(files[col_num++]) + 2;
    }
    col_num--;
    qsort(files, cnt, sizeof(files[0]), cmp);
    int len_max[100];
    for (int i = 0; i < col_num; i++) {
        int t = i, max = 0;
        while (t <= cnt) {
            if (max < strlen(files[t])) max = strlen(files[t]);
            t += col_num;
        }
        len_max[i] = max;
    }
    for (int i = 0; i < cnt; i++) {
        if (i && i % col_num == 0) printf ("\n");
        printf ("%s ", files[i]);
        printf ("%*s", len_max[i % col_num] - strlen(files[i]) + 1, " ");
    }
    printf ("\n");
    return 0;
}
