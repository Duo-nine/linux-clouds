#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	//互斥锁变量的初始化
pthread_cond_t  cond_A  = PTHREAD_COND_INITIALIZER;	//条件变量的初始化
pthread_cond_t  cond_B  = PTHREAD_COND_INITIALIZER;	//条件变量的初始化


/* 线程处理函数 */
void *threadHandler_A(void *arg){
	while(1){
		pthread_mutex_lock(&mutex); 		//上锁
		pthread_cond_wait(&cond_A, &mutex);	//先解锁 - 等待条件信号到来 | 信号到达 上锁并返回
			printf("thread - %ld\n", (long)arg);
		pthread_mutex_unlock(&mutex);		//解锁
	}
}

/* 线程处理函数 */
void *threadHandler_B(void *arg){
	while(1){
		pthread_mutex_lock(&mutex); 		//上锁
		pthread_cond_wait(&cond_B, &mutex);	//先解锁 - 等待条件信号到来 | 信号到达 上锁并返回
			printf("thread - %ld\n", (long)arg);
		pthread_mutex_unlock(&mutex);		//解锁
	}
}

/* 主线程 */
int main(int argc, char **argv){
	
	pthread_t pt;

	pthread_create(&pt, NULL, threadHandler_A, (void *)(long)0);
	pthread_create(&pt, NULL, threadHandler_A, (void *)(long)1);
	pthread_create(&pt, NULL, threadHandler_B, (void *)(long)2);

	while(1){
		getchar();
		//pthread_cond_signal(&cond);
		pthread_cond_broadcast(&cond_A);
	}

	return 0;
}

/*
================================================================================

	int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
	函数功能:将线程添加至等待队列(阻塞),然后将互斥锁解锁
			 	 	等待条件信号到来 - 信号未到达 -> 继续阻塞
			 	 					 - 信号到达       -> 先进行上锁动作,如果无误那么函数返回
	函数参数:cond		条件变量地址
			 mutex	互斥锁变量地址
	返 回 值:成功返回0,失败返回非0
	注     意:调用函数之前,必须先创建好一个互斥锁并完成加锁操作,然后才能作为实参传递

================================================================================	

	int pthread_cond_signal(pthread_cond_t *cond);
	函数功能:发出对应的条件信号, (具体唤醒哪一个,取决于线程调度程序)
	函数参数:cond		 目标条件
	返 回 值:成功返回0, 失败返回-1

================================================================================	

	pthread_cond_broadcast(pthread_cond_t *cond)
	函数功能:发出对应的条件信号,唤醒该队列中所有等待的线程
	函数参数:cond		 目标条件
	返 回 值:成功返回0, 失败返回-1

================================================================================

*/

