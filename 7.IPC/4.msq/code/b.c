#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

/* 消息结构 msgbuf */
struct msgbuf {
	long mtype;				/* 消息类型(非负) */
	char data[1024];		/* 消息数据(用户自定义) */
};


int main(int argc, char **argv){

	key_t key_value;	//键值
	int   msqid;		//消息队列标识符
	struct msgbuf msg;

	/* 创建键值 */
	if( -1 == (key_value = ftok("/home/meson",123)) ){
		perror("ftok");
		return -1;
	}
	/* 获取一个消息队列标识符 */
	if( -1 == (msqid = msgget(key_value, IPC_CREAT | 0666)) ){
		perror("msgget");
		return -1;
	}

	if( -1 == msgrcv(msqid,&msg,sizeof(msg) - sizeof(long),1234,IPC_NOWAIT) ){
		perror("msgrcv");
		return -1;
	}

	printf("msg = %s\n",msg.data);

	return 0;
}
