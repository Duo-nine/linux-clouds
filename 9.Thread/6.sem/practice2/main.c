#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem;

/* 模拟窗口 线程 */
void *threadHandler(void *arg)
{
	long id = (long)arg;
	int sem_value;

	while(1)
	{
		if( -1 == sem_trywait(&sem) )//尝试-1
		{
			break;
		}
		if( -1 == sem_getvalue(&sem, &sem_value) )
		{
			perror("sem_getvalue");
		}
		printf("=== windows %c --- client %-2d business begin ===\n", (char)('A' + id), 20 - sem_value);
		sleep(3);
		printf("### windows %c --- client %-2d business over  ###\n", (char)('A' + id), 20 - sem_value);
	}
	pthread_exit(NULL);//退出线程
}

/* 主线程 */
int main(int argc, char **argv)
{
	pthread_t pt[3];//线程标识符

	sem_init(&sem, 0, 20);//信号量初始化

	for(long i = 0; i < 3; i++)//三个线程模拟窗口
	{
		if( pthread_create(pt + i, NULL, threadHandler, (void *)i) )//线程创建
		{
			perror("pthread_create");
			return -1;
		}
	}

	for(int i = 0; i < 3; i++)//手动释放线程
	{
		pthread_join(pt[i],NULL);
	}

	return 0;
}

