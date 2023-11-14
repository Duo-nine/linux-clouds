#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/* 函数声明 */
void *threadHandler(void *arg);
void autoclean_1(void *arg);				//线程清理函数
void autoclean_2(void *arg);


/* 全局变量 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* 主线程 */
int main(){

	pthread_t pt;

	pthread_create(&pt, NULL, threadHandler, NULL);

	usleep(50);

	pthread_mutex_lock(&mutex);
	pthread_join(pt, NULL);
	pthread_mutex_unlock(&mutex);

	return 0;
}

/* 线程处理函数 */
void *threadHandler(void *arg){
	int *p = malloc(4);
	FILE *fp = NULL;
	
	printf("Child thread start working.\n");
	pthread_mutex_lock(&mutex);

	pthread_cleanup_push(autoclean_1, NULL);	/* 注册线程清理函数 */
	pthread_cleanup_push(autoclean_2, p);		//后注册的清理函数先执行
	
		fp = fopen("xxxx","rb");
		if( fp == NULL ){
			perror("fopen");
			//pthread_exit((void *)-1);
			return (void *)-1;
		}

	pthread_cleanup_pop(0);					/* 注销线程清理函数 */
	pthread_cleanup_pop(0);					/* 注销线程清理函数 */
	
	free(p);
	fclose(fp);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

/* 线程清理函数 */
void autoclean_1(void *arg){	//在注册之后和注销之前 如果线程发生退出的动作 则会自动执行清理函数
	printf("%s %d\n",__func__,__LINE__);
	pthread_mutex_unlock(&mutex);
}
void autoclean_2(void *arg){	//在注册之后和注销之前 如果线程发生退出的动作 则会自动执行清理函数
	printf("%s %d\n",__func__,__LINE__);
	free(arg);
}

/*
	注册线程清理函数
	void pthread_cleanup_push(void (*routine)(void *), void *arg);
		@routine	指向线程清理函数的指针
		@arg		要传递给线程清理函数的参数

	注销线程清理函数
	void pthread_cleanup_pop(int execute);
		@execute	0 自动清理 / 1 强制清理(如果没有在注销前触发清理动作 注销时会强制执行一遍)

	需要测试 当前使用的Linux系统发行版 中return是否能够触发清理机制

	Ubuntu 16.04.1

	Ubuntu 22
	Debian 11

*/
