#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){

	int  pipefd[2] = {0,0};
	char buffer[1024] = {0};

	if( -1 == pipe(pipefd) ){	/* 初始化无名管道(发生在创建子进程之前) */
		perror("pipe");
		return -1;
	}

	pid_t ret = fork();

	if( ret < 0 ){
		perror("fork");
		return -1;
	}else if( !ret ){
		/* 子进程 */
		printf("Before read : %s\n",buffer);
		read(pipefd[0], buffer, sizeof(buffer));	// 如果管道中没有数据 read阻塞
		printf("After read  : %s\n",buffer);
		
	}else{
		/* 父进程 */
		strcpy(buffer, "Hello world!\n");
		sleep(3);
		write(pipefd[1], buffer, strlen(buffer));
	}

	while(1){
		sleep(1);
	}

	return 0;
}

/*
	int pipe(int pipefd[2]);
	函数功能:创建无名管道并且初始化读写端的描述符
	函数参数:pipefd			 指向长度为2的整型数组的首地址(传入一个int xx[2]数组的首地址,用来存放两个描述符)
						 下标为0的描述符用来读取,下标为1的描述符用来写入
	返 回 值:成功返回0,失败返回-1

*/
