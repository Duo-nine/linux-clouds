#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "mysem.h"
#include <signal.h>

#define PAGE_SIZE	4096

int semid;

void _SIGINT_handler(int signum){
	if( -1 == semctl(semid, 0, IPC_RMID) ){
		perror("shmget");
	}
	_exit(0);
}

int main(int argc, char **argv){

	key_t key_value;		//键值
	int   shmid;			//共享内存标识符
	void *p_shm = NULL;		//共享内存段的首地址

	signal(SIGINT,_SIGINT_handler);

	semid = semArrInit(2);

	if( -1 == (key_value = ftok("/home",222)) ){
		perror("ftok");
		return -1;
	}
	/* 获取共享内存标识符 */
	if( -1 == (shmid = shmget(key_value,PAGE_SIZE,IPC_CREAT | 0666)) ){
		perror("shmget");
		return -1;
	}

	/* 接入目标共享内存段 获取内存的首地址 */
	if( (void *)-1 == (p_shm = shmat(shmid, NULL, 0)) ){
		perror("shmat");
		return -1;
	}
	printf("=== Attached shared memory succeed ===\n\n");

	/* 数据读写 */
	while(1){
		semP(semid, 1);
		printf("memory : %s\n", (char *)p_shm );
	}

	/* 与目标共享内存段断开连接 */
	if( -1 == shmdt(p_shm) ){
		perror("shmdt");
		return -1;
	}
	printf("\n=== Detached shared memory succeed ===\n");

	return 0;
}

