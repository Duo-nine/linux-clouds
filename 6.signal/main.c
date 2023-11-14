#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

/* 信号SIGNAL处理函数 [void (*sighandler_t)(int)] */
void _SIGINT_handler(int signum){
	printf("Catch siganl : %d\n",signum);
}

/* 主函数 */
int main(){

	signal(SIGINT,_SIGINT_handler);		//试图去捕获 SIGINT 并执行 _SIGNAL_handler()
//	signal(SIGQUIT,_SIGINT_handler);	//试图去捕获 SIGQUIT 并执行 _SIGNAL_handler()

	signal(SIGINT,SIG_DFL);		//试图去捕获 SIGINT 并执行默认动作
	signal(SIGQUIT,SIG_IGN);	//试图去捕获 SIGQUIT 并忽略

	signal(SIGKILL,SIG_IGN);
	//signal(SIGTERM,SIG_IGN);	
	
	int sec = 0;

	while(++sec){
		sleep(1);
		printf("Process %d running %ds ... \n",getpid(),sec);
	}

	return 0;
}

/*
	pid - process id 进程号

	pid_t getpid(void);
	函数功能:获取当前进程的进程号
	函数参数:无参数
	返 回 值:进程号


	sighandler_t signal(int signum, sighandler_t handler);
	函数功能:注册信号处理函数 捕获信号
	函数参数:signum 		- 想要捕获的信号值
			 handler 	- 指向处理程序			SIG_IGN -> signal ignore 忽略该信号
			 								SIG_DFL -> signal default 执行默认动作
	typedef void (*sighandler_t)(int);

	SIGKILL - 为管理员提供了可以杀死任意进程的方法

	(9)SIGKILL 不可以被捕获 不可以被忽略 不可以被阻塞

*/
