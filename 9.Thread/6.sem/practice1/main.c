#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semA,semB,semC;//创建信号量

//线程处理函数
void *threadHandler_1(void *arg)
{
	while(1)
	{
		sem_wait(&semA);//sem_wait函数:-1
		printf("A\n");
		sem_post(&semB);//sem_post函数:+1
	}
}

void *threadHandler_2(void *arg)
{
	while(1)
	{
		sem_wait(&semB);
		printf("B\n");
		sem_post(&semC);
	}
}

void *threadHandler_3(void *arg)
{
	while(1)
	{
		sem_wait(&semC);
		printf("C\n");
		sem_post(&semA);
	}
}


int main()
{
	pthread_t pt1,pt2,pt3;//线程标识符

	//线程初始化
	sem_init(&semA, 0, 1);
	sem_init(&semB, 0, 0);
	sem_init(&semC, 0, 0);

	//线程创建
	pthread_create(&pt1, NULL, threadHandler_1, NULL);
	pthread_create(&pt2, NULL, threadHandler_2, NULL);
	pthread_create(&pt3, NULL, threadHandler_3, NULL);
	while(1)
	{

	}

	return 0;
}


