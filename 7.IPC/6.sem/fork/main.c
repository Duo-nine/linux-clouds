#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "mysem.h"

#define PAGE_SIZE	4096

int main(int argc, char **argv){

	key_t key_value;		//键值
	int   shmid;			//共享内存标识符
	void *p_shm = NULL;		//共享内存段的首地址
	int   semid;
	pid_t fork_retval;
	int   num = 0;

	if( argc != 2 ){
		printf("<usage> : %s [Non-negative integers]\n",argv[0]);
		return -1;
	}
	num = atoi(argv[1]);

	/* 获取并初始化信号量 */
	semid = semArrInit(1);
	if( (fork_retval = fork()) < 0 ){	//创建子进程
		perror("fork");
		return -1;
	}
	if( !fork_retval ){	//子进程
		printf("semid = %d\n",semid);
	}else{	//父进程
		printf("semid = %d\n",semid);
	}
	if( -1 == (key_value = ftok("/home",102)) ){	//创建键值
		perror("ftok");
		return -1;
	}
	if( -1 == (shmid = shmget(key_value, PAGE_SIZE, IPC_CREAT | 0666)) ){	//获取shmid
		perror("shmget");
		return -1;
	}
	if( (void *)-1 == (p_shm = shmat(shmid, NULL, 0)) ){	//接入shm
		perror("shmat");
		return -1;
	}

	/* 信号量内部的值不能小于0,如果一个程序对信号量进行资源申请,企图将值-1,但此时其值已经为0,那么程序会阻塞,直到其值>0 */

	/* 申请信号量资源(-1) */
	semP(semid, 0);
	for(int i = 0; i < num; i++){		
		( *(int *)p_shm )++;
	}
	/* 释放信号量资源(+1) */
	semV(semid, 0);

	/* 信号量使用完毕后,应当立即释放(删除信号量) */
	if( -1 == semctl(semid, 0, IPC_RMID) ){
		perror("semctl");
		return -1;
	}

	/* 与目标共享内存段断开连接 */
	shmdt(p_shm);

	return 0;
}


/* 
	临界资源:是指一段能够同时被多个进程或线程访问的资源
	临界区域:指执行数据更新的代码需要独占式地执行

信号量:
	为了防止出现因多个程序同时访问一个共享资源而引发的一系列问题，我们需要一种
方法，它可以通过生成并使用令牌来授权，在任一时刻只能有一个执行线程访问代码的临
界区域。临界区域是指执行数据更新的代码需要独占式地执行。而信号量就可以提供这样
的一种访问机制，让一个临界区同一时间只有一个线程在访问它，也就是说信号量是用来
调协进程对共享资源的访问的。

信号量本质是内部有一个计数器
程序对于信号量可以做两种操作
	1.申请资源 值-1 (P操作)
	2.释放资源 值+1	(V操作)
		Passeren（通过） Vrijgeven（释放）

1.获取信号组标识符
2.申请资源 / 释放资源

 */
