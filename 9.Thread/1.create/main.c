#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/* 全局变量 */
char buffer[1024];

/* 线程处理函数 */
void *threadHandler(void *arg){
	while(1){
		fgets(buffer, sizeof(buffer), stdin);
	}
}

/* 主线程 */
int main(int argc, char **argv){
	
	pthread_t pt;	//存放线程的标识符

	if( pthread_create(&pt, NULL, threadHandler, NULL) ){
		perror("pthread_create");
		return -1;
	}

	printf("pt = %lu\n",pt);
	strcpy(buffer, "Hello!");

	while(1){
		sleep(1);
		printf("%s\n",buffer);
	}	

	return 0;
}

/*

	int pthread_create( pthread_t *thread, const pthread_attr_t *attr,
						void *(*start_routine) (void *), void *arg);
	函数功能:创建一个线程
	函数参数:thread			 	指针,指向的区域用来存储创建成功的-线程标识符-
	
			 attr		 	指针,指向的区域用来读取创建线程时使用的属性
			 			 	写NULL,使用线程的默认属性
			 			 	
			 start_routine	函数指针,指向的函数将是线程工作的内容,即需要调用的函数
			 
			 arg			指针,指向创建线程时要传递给线程的参数
			 				写NULL,表示不给线程传递任何参数
	返 回 值:成功返回0,失败返回错误值(非零0)
	注     意:使用线程相关的函数时,需要链接Linux线程库(不是系统默认库) -lpthread





	编写程序 程序中有一段buffer
	每隔一秒 输出一次buffer中的数据 由键盘输入的字符串将赋值给buffer
*/
