#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* 全局变量 */
extern char *optarg;
extern int   optind;
extern int   opterr;
extern int   optopt;
long         type;		//存储发送/接收消息类型

//消息队列结构类型 msgbuf
struct msgbuf
{
	long mtype;
	char data[1024];
};

//函数声明
static int sendMsg(int msqid);//发送数据
static int recvMsg(int msqid);//接收数据

//主函数
int main(int argc, char **argv)
{
	int   ret;				//接收返回值
	char *mode = NULL;		//指向模式字符串
	key_t keyv ;
	int   msqid;			//消息队列标识符
	type = 0 ;

	//检查主函数传入的参数是否处理完成
	while( -1 != (ret = getopt(argc,argv,"m:t:")) )
	{
		switch( ret )
		{
			case 'm' : mode = optarg;break;		 //mode 处理工作模式
			case 't' : type = atol(optarg);break;//type 消息类型   字符串转long型
			case '?': break;					 //应当携带参数但没有携带参数的 || 无效选项
			default : break;
		}
	}

	if( !mode )//检查模式指定是否有效
	{	
		printf("Must specify transferm mode.\n");return -1;
	}
	else if( strcmp(mode,"recv") && strcmp(mode,"send") )
	{
		printf("Invalid mode argument.\n");return -1;
	}
	if( !type )//检查消息类型是否合法
	{	
		printf("Invalid message type.\n");return -1;
	}

	if( -1 == (keyv = ftok("/home/meson",100)) )//获取键值
	{	
		perror("ftok");
		return -1;
	}
	if( -1 == (msqid = msgget(keyv, IPC_CREAT | 0666)) )//获取消息队列标识符
	{	
		perror("msgget");
		return -1;
	}
	return ( !strcmp(mode,"recv") ) ? recvMsg(msqid) : sendMsg(msqid);
}

//发送数据
static int sendMsg(int msqid)
{
	struct msgbuf msg;//消息队列结构类型 msgbuf,里面定义了mtype和data
	printf("Input message : ");
	fgets(msg.data,sizeof(msg.data),stdin);
	msg.mtype = type;
	return msgsnd(msqid,&msg,sizeof(msg) - sizeof(long),0);
}
//接收数据
static int recvMsg(int msqid)
{
	struct msgbuf msg;
	if( msgrcv(msqid,&msg,sizeof(msg) - sizeof(long),type,IPC_NOWAIT) < 0 )
	{
		perror("msgrcv");
		return -1;
	}
	printf("recv = %s\n",msg.data);
	return 0;
}

/*
	int getopt(int argc, char *const argv[], const char *optstring);
	函数功能:处理主函数传入的参数选项
	函数参数:argc		...
			 argv	...
			 optstring    一个包含正确的参数选项字符串，用于参数的解析
			 			  "abc:"
	返 回 值:如果成功处理了一个参数选项,那么函数返回该选项字符
			 如果处理完了所有应当处理的参数选项,那么函数返回-1
			 如果发现了不在optstring中定义的参数选项,那么函数返回'?'
			 如果遇到缺少参数的选项，则返回值取决于 optstring 中的第一个字符':'如果它是':' 则返回':' 否则返回'?'

	optarg : 如果某个选项有参数，这指向当前选项的参数字符串
	optind : argv 的当前索引值
	opterr : 正常运行状态下为 0 
			 非零时表示存在无效选项或者缺少选项参数，并输出其错误信息
	optopt : 当发现无效选项字符时，即 getopt() 方法返回 '?' 字符
	         optopt 中包含的就是发现的无效选项字符

*/
