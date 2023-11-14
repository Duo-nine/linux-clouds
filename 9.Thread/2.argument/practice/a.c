#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>


/* 消息结构 msgbuf */
struct msgbuf 
{
	long mtype;				/* 消息类型(非负) 密钥*/
	char data[1024];		/* 消息数据(用户自定义) */
};

//全局变量
pthread_t pt;
key_t key_value;	//键值
int   msqid;		//消息队列标识符
struct msgbuf msg;	//消息结构数组msgbuf

//线程处理函数
void *threadHandler(void *arg)
{
	//接收数据
	while(1)
	{
		if( -1 == msgrcv(msqid,&msg,sizeof(msg) - sizeof(long),1235,0) )
		{
			perror("msgrcv");
			return NULL;
		}
		printf("recv : %s\n",msg.data);
	}
}

//主函数
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

	
	/* 发送数据 */
	while(1)
	{
		
		fgets(msg.data,sizeof(msg.data),stdin);
		msg.mtype = 1234 ;//密钥标识符
		if(-1 == msgsnd(msqid,&msg,sizeof(msg) - sizeof(long),0))
		{
			perror("msgsnd");
			return -1;
		}
	}
	
	return 0;
}

/*

	进程使用消息队列发送/接收消息
	1.进程获取一个消息队列标识符
	2.通过该标识符操作消息队列 进行数据收发

	系统中消息队列的状态可以通过指令"ipcs"去查看


	key_t ftok(const char *pathname, int proj_id);
	函数功能:创建键值(用于IPC中获取相应的标识符)
	函数参数:pathname			一个系统中存在且进程拥有访问权限的文件路径
			 proj_id		8位的非0值
	返 回 值:成功返回一个有效键值,失败返回-1

=======================================================================================

	int msgget(key_t key, int msgflg);
	函数功能:根据键值获取消息队列标识符
	函数参数:key		 键值
			 msgflg	 标志位		IPC_CREAT - 如果键值没有对应msq 则创建该msq ; 如果已经对应msq 直接返回该msq标识符
								IPC_EXCL 如果键值对应消息队列已经存在,则不会再创建
								标志位中还可以使用八进制数值描述消息队列的权限
	返 回 值:成功返回一个有效的消息队列标识符(非负整数),失败返回-1

=======================================================================================

	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
	函数功能:向目标消息队列中发送消息
	函数参数:msqid		   	目标消息队列标识符
			 msgp		指向固定的消息结构(发地址)
			 msgsz		实际传输的消息大小(bytes)
			 msgflg		标志位 一般写0
	返 回 值:成功返回0,失败返回-1


	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	函数功能:从目标消息队列中接收消息
	函数参数:msqid		   	目标消息队列标识符
			 msgp		指向固定的消息结构(存地址)
			 msgsz		实际传输的消息大小(bytes)
			 msgtyp     消息类型
			 msgflg		标志位 一般写0 -> 阻塞等待接收
			 					IPC_NOWAIT	如果队列中没有该消息类型
	返 回 值:成功返回从消息队列中获取到的数据大小(bytes),失败返回-1

*/
