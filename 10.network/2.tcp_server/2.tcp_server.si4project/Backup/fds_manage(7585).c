#include "fds_manage.h"

/* 全局变量 */
fd_set fds;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void fdsInit(){
	FD_ZERO(&fds);
}

void addUser(int sockfd){
	pthread_mutex_lock(&mutex);
	FD_SET(sockfd, &fds);
	pthread_mutex_unlock(&mutex);
}

void delUser(int sockfd){
	pthread_mutex_lock(&mutex);
	FD_CLR(sockfd, &fds);
	pthread_mutex_unlock(&mutex);
}

void copyFds(fd_set *target){
	pthread_mutex_lock(&mutex);
	*target = fds;
	pthread_mutex_unlock(&mutex);
}
