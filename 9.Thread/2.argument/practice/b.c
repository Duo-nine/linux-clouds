#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>

/* 消息结构 msgbuf */
struct msgbuf 
{
	long mtype;				/* 消息类型(非负) */
	char data[1024];		/* 消息数据(用户自定义) */
};

//全局变量
pthread_t pt;
key_t key_value;	//键值
int   msqid;		//消息队列标识符
struct msgbuf msg;

//线程处理函数,需要创建线程
void *threadHandler(void *arg)
{
	/* 发送数据 */
	while(1)
	{
		fgets(msg.data,sizeof(msg.data),stdin);
		msg.mtype = 1235 ;
		if( -1 == msgsnd(msqid,&msg,sizeof(msg) - sizeof(long),0) )
		{
			perror("msgsnd");
			return NULL;
		}
	}
}


int main(int argc, char **argv)
{
	/* 创建键值 */
	if( -1 == (key_value = ftok("/home/dlinux",123)) )
	{
		perror("ftok");
		return -1;
	}
	/* 获取一个消息队列标识符 */
	if( -1 == (msqid = msgget(key_value, IPC_CREAT | 0666)) )
	{
		perror("msgget");
		return -1;
	}
	//创建一个线程
	if( pthread_create(&pt, NULL, threadHandler, NULL) )
	{
		perror("pthread_create");
		return -1;
	}

	//接收数据
	while(1)
	{
		//&msg用来存放接收的数据
		if( -1 == msgrcv(msqid,&msg,sizeof(msg) - sizeof(long),1234,0) )
		{
			perror("msgrcv");
			return -1;
		}
		printf("recv : %s\n",msg.data);
	}

	return 0;
}
