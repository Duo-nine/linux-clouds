#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv){

	pid_t ret = fork();		//创建子进程

	if( ret == -1 ){
		perror("fork");
		return -1;
	} else if ( !ret ){
		/* 子进程 */
		printf("I'm child  - PID : %d | PPID : %d\n",getpid(),getppid());
	} else {
		/* 父进程 */
		printf("I'm parent - PID : %d | CPID : %d\n",getpid(),ret);
	}

	while(1){
		sleep(1);
		//printf("Process %d running ... \n",getpid());
	}
	return 0;
}

/*
	一个进程，包括代码、数据和分配给进程的资源。fork() 函数通过系统调用创建一
个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事，但如果初始参
数或者传入的变量不同，两个进程也可以做不同的事.

	pid_t fork(void);
	函数功能:创建一个子进程( 子进程中fork()不会再去创建子进程 )
	函数参数:无参数
	返 回 值:成功,父进程中的fork()返回成功创建出的子进程PID,子进程fork()返回0
			 失败,fork()(父进程)返回-1,子进程不会被创建	

*/
