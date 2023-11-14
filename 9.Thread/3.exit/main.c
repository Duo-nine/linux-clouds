#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#define	gettid()	syscall(SYS_gettid)

/* 函数声明 */
void *threadHandler(void *arg);

/* 主函数 */
int main(){

	pthread_t pt;
	//int s = 100;

	if( pthread_create(&pt, NULL, threadHandler, NULL) ){
		perror("pthread_create");
		return -1;
	}

	while(1){
		getchar();
		pthread_cancel(pt);	//发起对子线程pt的取消请求
		printf("pthread_cancel\n");
	}

	//pthread_exit(NULL);	//主线程如果使用 pthread_exit 以线程的方式退出 - 如果进程中还有子线程在运行 进程不会结束
	return 0;			//主线程如果使用 return 结束 - 进程直接结束
}

/* 线程处理函数 */
void *threadHandler(void *arg){
	int num = 0;
	while(1){
		//sleep(1);
		//printf("Child thread : %ld running...\n",gettid());
		num++;
		num--;
		pthread_testcancel();	//设置测试点 - 检查进程中是否有其他线程对自己发起cancel请求
	}
	pthread_exit(NULL);
}

/*
==================================================================================

	void pthread_exit(void *retval);
	函数功能:退出线程
	函数参数:retval			线程退出时携带的返回值
	返 回 值:无返回值
	
==================================================================================

	int pthread_cancel(pthread_t thread);
	函数功能:对目标线程发起取消请求(请求目标线程结束运行)
	函数参数:thread			 目标线程的标识符
	返 回 值:成功返回0,失败返回非0值
	注     意:被pthread_cancel()函数当作目标的线程,如果进行了系统调用动作或使用
			 pthread_testcancel()设置了测试点,那么才能成功响应

==================================================================================

	void pthread_testcancel(void);
	函数功能:在线程中设置cancel测试点,检查进程中是否有其他线程对自己发起cancel请求
	函数参数:无参数
	返 回 值:无返回值

线程结束执行的方式有3种
	1、线程将指定函数体中的代码执行完后自行结束 - 线程处理函数运行完后正常退出
	2、线程执行过程中，被同一进程中的其它线程强制终止 - 发送一个取消请求
	3、线程执行过程中，遇到 pthread_exit() 函数结束执行 √

*/
