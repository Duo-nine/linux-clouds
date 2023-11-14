/* fifo - 读取 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define	FIFO_PATH	"/home/meson/fifo"   //宏定义程序需要的路径

int main(){

	int fd;
	char buffer[1024] = {0};
	ssize_t readlen;

	//创建管道文件，创建失败返回-1
	if( -1 == mkfifo(FIFO_PATH,0666) && errno != EEXIST )
	{
		perror("mkfifo");
		return -1;
	}

	//打开管道文件，如果失败返回-1
	if( -1 == (fd = open(FIFO_PATH,O_RDONLY)) )
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		memset(buffer,0x00,sizeof(buffer));         //在读取之前清除之前好的内容
		readlen = read(fd,buffer,sizeof(buffer));	//读取管道中的内容	read()在无数据读取时会阻塞
		/* 当一个管道的写端被关闭 读端返回0 */
		if( readlen < 0 )
		{
			perror("read");
			break;
		}else if( !readlen )
		{
			printf("Writer was closed.\n");
			break;
		}
		printf("read = %s\n",buffer);
	}
	
	close(fd);
	
	return 0;
}

/*
	
	int mkfifo(const char *pathname, mode_t mode);
	函数功能:创建一篇管道文件
	函数参数:pathname		 描述管道文件创建的位置
			 mode		 权限
			 			(权限写4位八进制)
			 			(umask 值则表明了需要从默认权限中去掉哪些权限
			 			 来成为最终的默认权限值)
	返 回 值:成功返回0,失败返回-1

*/
