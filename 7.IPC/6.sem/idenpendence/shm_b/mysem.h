#ifndef _MYSEM_H
#define _MYSEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int               val;    /* Value for SETVAL */
	struct semid_ds  *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short   *array;  /* Array for GETALL, SETALL */
	struct seminfo   *__buf;  /* Buffer for IPC_INFO
	                           (Linux-specific) */
};


/* 函数声明 */
//获取一个信号组标识符,并对其内部的每一个信号量进行初始化
int semArrInit(int nsems);
//申请信号量资源
int semP(int semid, int num);
//释放信号量资源
int semV(int semid, int num);

#endif /* _MYSEM_H */

