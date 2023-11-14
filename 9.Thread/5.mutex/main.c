#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *threadHandler(void *arg);

/* 全局变量 */
int num;

//互斥锁变量静态初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* 主线程 */
int main(int argc, char **argv)
{
	if( argc != 2 )
	{
		printf("<usage> : %s [num]\n", argv[0]);
		return -1;
	}
	int target = atoi(argv[1]);
	pthread_t pt_1,pt_2;
	num = 0;

	//创建线程
	pthread_create(&pt_1, NULL, threadHandler, &target);
	pthread_create(&pt_2, NULL, threadHandler, &target);

	//手动释放线程  pthread_join
	pthread_join(pt_1,NULL);
	pthread_join(pt_2,NULL);

	printf("num = %d\n",num);

	return 0;
}

/* 线程处理函数 */
void *threadHandler(void *arg)
{
	int target = *(int *)arg ;
	for(int i = 0; i < target; i++)
	{
		pthread_mutex_lock(&mutex);
			num++;
		pthread_mutex_unlock(&mutex);//上锁之后必须解锁
	}
	pthread_exit(NULL);
}

/* 
	互斥锁 mutex - 用于对共享数据进行锁定，保证同一时刻
				   只能有一个线程去操作
				   用于封装一段动作
	
	互斥锁类型 pthread_mutex_t

	使用互斥锁之前 必须先对其进行初始化
	静态初始化
	动态初始化

	查询线程同步互斥相关函数手册 需要安装 manpages-posix-dev
	apt-get install manpages-posix-dev

	pthread_mutex_lock(pthread_mutex_t *mutex)		上锁
	pthread_mutex_unlock(pthread_mutex_t *mutex)	解锁

*/
