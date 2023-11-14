#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

void *threadHandler(void *arg);

/* 全局变量 */
int num;
sem_t sem;

/* 主线程 */
int main(int argc, char **argv){

	if( argc != 2 ){
		printf("<usage> : %s [num]\n", argv[0]);
		return -1;
	}
	int target = atoi(argv[1]);
	pthread_t pt_1,pt_2;
	num = 0;
	
	sem_init(&sem, 0, 1);

	pthread_create(&pt_1, NULL, threadHandler, &target);
	pthread_create(&pt_2, NULL, threadHandler, &target);

	pthread_join(pt_1,NULL);
	pthread_join(pt_2,NULL);

	printf("num = %d\n",num);

	return 0;
}

/* 线程处理函数 */
void *threadHandler(void *arg){
	int target = *(int *)arg ;
	for(int i = 0; i < target; i++){
		sem_wait(&sem);
			num++;
		sem_post(&sem);
	}
	pthread_exit(NULL);
}

