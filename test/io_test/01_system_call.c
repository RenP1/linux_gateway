#include "fcntl.h"
#include <unistd.h>
#include "stdio.h"

int main(int argc, char const *argv[])
{
    // 打开读文件
    int fd1 = open("ig.txt",O_RDONLY);
    if (fd1 == -1)
    {
        printf("打开ig.txt失败");
    }
    
    // 打开写文件
    int fd2 = open("rng.txt",O_CREAT | O_WRONLY, 0644);
    if (fd2 == -1)
    {
        printf("打开rng.txt失败");
    }
    
    // 边读边写
    int read_len;
    char buf[100];
    while ((read_len = read(fd1,buf,sizeof(buf))) > 0)
    {
        // 输出控制台
        write(STDOUT_FILENO,buf,read_len);
        // 写入文件
        write(fd2,buf,read_len);
    }
    // 关闭文件
    close(fd1);
    close(fd2);
    return 0;
}
