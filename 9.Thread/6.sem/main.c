#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

char  buffer[1024];
sem_t sem;			//信号量

void *threadHandler(void *arg){

	struct timespec t = {1695807900, 0};

	while(1){
		sleep(1);
		printf("wait return : %d\n", sem_timedwait(&sem, &t));
		printf("buffer = %s\n",buffer);
	}
}

int main(){

	pthread_t pt;

	if( sem_init(&sem, 0, 5) ){		/* 初始化信号量 */
		perror("sem_init");
		return -1;
	}
	
	pthread_create(&pt, NULL, threadHandler, NULL);

	while(1){
		fgets(buffer, sizeof(buffer), stdin);
	}

	sem_destroy(&sem);

	return 0;
}


/*
====================================================================================

	int sem_init(sem_t *sem, int pshared, unsigned int value);
	函数功能:初始化信号量
	函数参数:sem		 	 目标信号量
			 pshared 	 0 -> 仅在进程间使用 | 非0 -> 可以被进程共享
			 value       信号量的初值
	返 回 值:returns 0 on success, on error, -1 is returned.
	
====================================================================================
申请信号量资源( 值-1 )
	int sem_wait(sem_t *sem);		如果值已经为0, 阻塞的等待

	int sem_trywait(sem_t *sem);	如果值已经为0, 直接返回一个错误值,不进行等待

	int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
									如果值已经为0, 阻塞的等待, 将在abs_timeout设置的绝对时间点返回

====================================================================================
释放信号量资源( 值+1 )
	int sem_post(sem_t *sem);

====================================================================================

	int sem_destroy(sem_t *sem);

*/
