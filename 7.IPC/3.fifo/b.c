/* fifo - 写入 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define	FIFO_PATH	"/home/meson/fifo"

int main(){

	int fd;
	char buffer[1024] = {0};

	/* 创建管道文件 */
	if( -1 == mkfifo(FIFO_PATH,0666) && errno != EEXIST ){
		perror("mkfifo");
		return -1;
	}

	/* 打开管道文件 */
	if( -1 == (fd = open(FIFO_PATH,O_WRONLY)) ){
		perror("open");
		return -1;
	}

	while(1){
		fgets(buffer,sizeof(buffer),stdin);      //输入发送的数据
		if( write(fd,buffer,strlen(buffer)) < 0 ){	//读取管道中的内容
		/* 管道破裂 :  向一个没有读端的管道写数据。默认动作为 - 终止进程。 */
			perror("write");
			break;
		}
	}
	
	close(fd);
	
	return 0;
}


