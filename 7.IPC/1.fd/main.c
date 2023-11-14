#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){

//	int fd;
//	char str[128] = {0};
//		
//	if( -1 == (fd = open("./txt",O_RDONLY | O_APPEND) ) ){
//		perror("open");
//		return -1;
//	}
//
//	read(fd,str,sizeof(str));
//
//	printf("%s\n",str);
//
//	close(fd);

	write(1,"Hello world!\n",13);

	return 0;
}

/*
	int open(const char *pathname, int flags);
	函数功能:打开文件 返回文件描述符
	函数参数:pathname		文件路径
			 flags		打开模式 - O_RDONLY 只读
			 					 - O_WRONLY	只写
			 					 - O_RDWR   读写
			 			- O_CREAT   文件不存在 创建文件
			 			- O_APPEND  添加/追加
			 
	返 回 值:成功返回一个文件描述符,失败返回-1
	

	ssize_t read (int fd,       void *buf, size_t count);
	ssize_t write(int fd, const void *buf, size_t count);
		fd    -> 文件描述符
		buf   -> 缓冲区首地址
		count -> 传输的字节数

	IPC - Inter Process Communication 进程间通信

	fd - file descriptor (int)

	管道 - 有名管道 / 无名管道
	消息队列
	共享内存
	信号量(组)
	套接字(特殊的文件描述符)

*/
