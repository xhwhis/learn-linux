#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_val(char *path, char *key, char *ans) {
    FILE *fp = NULL;
    ssize_t read;
    size_t len;
    char *line = NULL;
    char *substr = NULL;
    if (key == NULL || ans == NULL) {
        printf ("Error in arguement\n");
        return -1;
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return -1;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) continue;
        if (substr[strlen(key)] == '=') {
            strncpy(ans, substr + strlen(key) + 1, read - strlen(key) - 1);
            *(ans + strlen(ans) - 1) = '\n';
            break;
        }
    }
    free(line);
    fclose(fp);
    if (substr == NULL) return -1;
    return 1;
}

int main() {
    char buff[32];
    memset(buff, 0, sizeof(buff));
    get_val("./test.conf", "ip", buff);
    printf ("ip=%s", buff);
    memset(buff, 0, sizeof(buff));
    get_val("./test.conf", "name", buff);
    printf ("name=%s", buff);
    return 0;
}
