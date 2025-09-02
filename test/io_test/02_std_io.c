#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/**
标准输入输出流 （File *）
    FILE *stdin;   标准输入
    FILE *stdout;  标准输出
    FILE *stderr;  标准错误输出
标准输出输出流的文件描述符
    STDIN_FILENO    0  标准输入
    STDOUT_FILENO   1  标准输出
    STDERR_FILENO   2  标准错误输出
 */

int main(int argc, char const *argv[])
{
    // 关闭标准输入输出
    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    // 打开正常日志文件流
    open("std.log", O_CREAT | O_WRONLY | O_APPEND, 0644);

    // 打开错误日志文件流
    open("error.log", O_CREAT | O_WRONLY | O_APPEND, 0644);

    // 使用标准输入(stdin)打印,获取到的字符串自带换行符(\n),FILE *一般用在库函数输入输出
    char name1[8];
    fgets(name1, 8, stdin);
    printf("使用标准输入(stdin)打印: %s", name1);
    // 使用标准输出(stdout)打印
    fputs("使用标准输出(stdout)打印!!!\n", stdout);
    // 使用标准错误输出(stderr)打印
    fputs("使用标准错误输出(stderr)打印!!!\n", stderr);

    // 使用标准输入(STDIN_FILENO)打印
    char name2[8];
    ssize_t len = read(STDIN_FILENO, name2, 8);
    printf("使用标准输入(STDIN_FILENO)打印: %.*s", len, name2);
    // 使用标准输出(STDOUT_FILENO)打印
    write(STDOUT_FILENO, "使用标准输出(STDOUT_FILENO)打印!!!\n", 43);
    // 使用标准错误输出(STDERR_FILENO)打印
    write(STDERR_FILENO, "使用标准错误输出(STDERR_FILENO)打印!!!\n", 49);
    return 0;
}
