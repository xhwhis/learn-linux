#include <stdio.h>
#include <unistd.h>

int main() {
    char buffer[100];
    getcwd(buffer, 100);
    printf ("before dir is %s\n", buffer);
    if (chdir("/home/lws") < 0) printf ("failed!\n");
    else printf ("success!\n");
    getcwd(buffer, 100);
    printf ("after dir is %s\n", buffer);
    return 0;
}

/*
 * 每个程序的运行进程是独立的，shell里运行mycd不能改变位置,
 * 系统的cd应该内嵌在shell（或bash）里所以可以改变。
 * */
