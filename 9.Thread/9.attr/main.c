#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* 子线程 */
void *threadHandler(void *arg){

	unsigned char buffer[1024 * 1024 * 10];
	memset(buffer, 0x00, sizeof(buffer));
	buffer[ 1024 * 1024 * 10 - 1 ] = 0x0A;

	sleep(1);

	pthread_exit(NULL);
}

/* 设置(修改)线程属性 */
static void set_pthread_attr(pthread_attr_t *attr){

	/* 修改分离状态 */
	if( pthread_attr_setdetachstate(attr, PTHREAD_CREATE_JOINABLE) ){
		perror("pthread_attr_setdetachstate");
		return;
	}
	/* 修改栈大小 */
	if( pthread_attr_setstacksize(attr, 0xB00000) ){
		perror("pthread_attr_getstacksize");
		return;
	}
}

/* 显示线程属性 */
static void display_pthread_attr(pthread_attr_t *attr){

	int i;
	size_t stack_size;

	/* 获取分离状态 */
	if( pthread_attr_getdetachstate(attr, &i) ){
		perror("pthread_attr_getdetachstate");
		return;
	}
	printf("Detach state        = %s\n",
		(i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
		(i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
		"???");

	/* 获取栈大小 */
	if( pthread_attr_getstacksize(attr, &stack_size) ){
		perror("pthread_attr_getstacksize");
		return;
	}
	printf("Stack Size          = 0x%lx\n", stack_size);
	
}


/* 主线程 */
int main(int argc, char **argv){

	pthread_t pt;
	//int num = 0;

	pthread_attr_t my_attr;				// 定义线程属性变量

	if( pthread_attr_init(&my_attr) ){	//初始化线程属性(变量) 初始化完即默认属性
		perror("pthread_attr_init");
		return -1;
	}

	/* 修改线程属性 */
	set_pthread_attr(&my_attr);

	/* 展示线程属性 */
	display_pthread_attr(&my_attr);

	pthread_create(&pt, &my_attr, threadHandler, NULL);

	pthread_join(pt, NULL);
	
	pthread_attr_destroy(&my_attr);	//释放

	return 0;
}

/*

	成功返回0 失败返回非0
		线程属性初始化
		int pthread_attr_init(pthread_attr_t *attr);
		
		释放
		int pthread_attr_destroy(pthread_attr_t *attr);

	Detach state        	分离状态			当前线程结束时系统是否会自动回收其资源(可结合   的PTHREAD_CREATE_JOINABLE / 可分离的PTHREAD_CREATE_DETACHED)
	Scope           		作用域				当前线程竞争CPU资源的范围(系统中所有进程 / 当前进程中的其他线程)
	Inherit scheduler   	继承策略			当前线程的属性来源(父线程 / 自定义)
	Scheduling policy   	调度策略			分时调度√ / 实时调度 / 轮询调度
	Scheduling priority 	调度优先级	    	仅在"调度策略"不为"分时调度"的状态下生效
	Guard size          	警戒缓冲区(栈)  
	Stack address       	栈地址				在栈上使用的地址
	Stack size          	栈大小				线程在栈上能够使用的最大空间

*/
