#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define STRING_LENGTH	1024
#define gettid()		syscall(SYS_gettid)

/* 创建四个子线程 这四个线程执行同一个线程处理函数 但是要分别打印数值0 1 2 3 */

/* 线程处理函数 */
void *threadHandler(void *arg){

	printf("Thread %ld - %ld\n", gettid(), (long)arg);

	sleep(1000);
	return NULL;
}

/* 主线程 */
int main(int argc, char **argv){

	pthread_t pt[4];

	long i;
	for(i = 0; i < 4; i++){
		if( pthread_create(pt + i, NULL, threadHandler, &pt) ){
			perror("pthread_create");
			return -1;
		}
	}
	//printf("Main thread id : %ld\n", gettid() );

	while(1){
		sleep(1);
	}

	return 0;
}

