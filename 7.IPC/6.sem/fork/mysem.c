#include "mysem.h"

/* 根据一个组标识符,设置该组中下标为num的信号量的值 */
static int setSemArrValue(int semid, int num, int value){
	union semun s = {
		.val = value,
	};	//设定要赋的值
	if( -1 == semctl(semid, num, SETVAL, s) ){
		perror("semctl");
		return -1;
	}
	return 0;
}

/* 获取一个信号组标识符,并对其内部的每一个信号量进行初始化 */
int semArrInit(int nsems){

	key_t key;			//键值
	int   semid;		//信号组标识符

	if( -1 == (key = ftok("/home",6)) ){
		perror("ftok");
		return -1;
	}
	if( -1 == (semid = semget(key, nsems, IPC_CREAT | 0666)) ){	/* 获取到信号组标识符 */
		perror("semget");
		return -1;
	}
	for(int i = 0; i < nsems; i++ ){	/* 初始化信号组中每一个信号量的值 默认第一个信号量初值为1 如果有多个 那么后边的信号量初值为0 */
		setSemArrValue(semid, i, !i);
	}
	return semid;
}

/* 申请信号量资源 */
int semP(int semid, int num){
	struct sembuf sem_buf = {
		 .sem_num = num,	/* 要操作的组中信号量下标 */
		 .sem_op  = -1, 	/* 操作信号量的值时,系统会将信号量目前的值与该值做加法运算 */
		 .sem_flg = SEM_UNDO,
	};	/* sem_flg是操作的标志位 设定为SEM_UNDO让进程在异常退出时自动释放申请的信号量
	     *                       设定为IPC_NOWAIT让申请动作被阻塞时不进行等待 */
	return semop(semid, &sem_buf, 1);
}

/* 释放信号量资源 */
int semV(int semid, int num){
	struct sembuf sem_buf = {
		 .sem_num = num,
		 .sem_op  = +1,
		 .sem_flg = SEM_UNDO,
	};
	return semop(semid, &sem_buf, 1);
}


/*

====================================================================================
	int semget(key_t key, int nsems, int semflg);
	函数功能:获取信号组标识符
	函数参数:key		 键值
			 nsems	 指定该组拥有多少个信号量
			 semflg  标志为和权限参数
	返 回 值:成功返回有效的信号组标识符,失败返回-1
====================================================================================
	int semctl(int semid, int semnum, int cmd, ...);
	函数功能:根据semid和cmd 操作目标信号组
	函数参数:semid		 信号组标识符
			 semnum	 该组中信号量的下标
			 cmd     操作值(宏) - SETVAL - arg(4)应当传入一个类型为union semun值,功能为设置semnum下标的信号量的值
			                      IPC_RMID - arg(4)无需传参 功能为删除信号组
	返 回 值:成功根据传入的cmd不同,返回值不同
			 失败返回-1
====================================================================================
	int semop(int semid, struct sembuf *sops, size_t nsops);
	函数功能:根据semid具体操作组中一个或多个信号量的值
	函数参数:semid		  信号组标识符
			 sops	  结构指针,指向类型为struct sembuf的结构体
			 nsops	  被本次调用操作的信号量的数量
	返 回 值:成功返回0,失败返回-1
	 
*/
