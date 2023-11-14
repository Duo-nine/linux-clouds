/* 回收线程的资源 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadHandler(void *arg);

/* 主线程 */
int main(int argc, char **argv){

	pthread_t pt;
	void *retval = NULL;

	//创建线程
	if( pthread_create(&pt, NULL, threadHandler, NULL) ){
		perror("pthread_create");
		return -1;
	}

	//等待目标线程结束,如果目标线程没有结束,那么pthread_join会阻塞
	if( pthread_join(pt,&retval) ){		//retval存储了目标线程的返回值
		perror("pthread_join");
		return -1;
	}
	printf("return : %s\n", (char *)retval);

	return 0;
}

void *threadHandler(void *arg){
	//pthread_detach(pthread_self());		//线程将自己设置为分离状态

	int num = 3;
	while(num--){
		sleep(1);
		printf("Child thread will exit after %ds ...\n",num);
	}
	pthread_exit("I'm over.");
}

/*
	进程中可供线程使用的资源是有限的
	如果一个默认属性线程的被创建,而其又默认的退出,没有进行资源回收的动作
	那么最终,进程中可供线程使用的资源将被耗尽
	进程不能再创建新的线程

	释放一个线程的资源 有两种方法:
		1.手动释放 - pthread_join()
		2.自动释放 - pthread_detach()

	自动释放: 线程的默认属性中 - "分离状态"被设置为"可结合的" 
								 即(PTHREAD_CREATE_JOINABLE)
			  一个"结合"状态下的线程 退出后不会释放其资源
			  我们可以通过调整线程属性"分离状态" ,将一个线程
			  设置为"可分离的" (PTHREAD_CREATE_DETACHED)
			  一个"分离"状态下的线程 退出后会自动释放其资源
			  即(系统会自动回收"分离"状态线程的资源)

=====================================================================================

	int pthread_detach(pthread_t thread);
	函数功能:将线程设置为"分离"状态
	函数参数:thread			 目标线程的标识符
	返 回 值:成功返回0
			 失败返回非0
	
=====================================================================================

	pthread_t pthread_self(void);
	函数功能:在调用该函数的线程中返回该线程的标识符(pthread_t)
	函数参数:无参数
	返 回 值:返回该线程的标识符

=====================================================================================

	int pthread_join(pthread_t thread, void **retval);
	函数功能:阻塞的等待目标线程结束并回收其资源,可以获取其返回值
	函数参数:thread			 目标线程的标识符
			 retval		 指向目标线程的返回值
	返 回 值:成功返回0
			 失败返回非0

*/
