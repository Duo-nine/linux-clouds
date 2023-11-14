#ifndef _FDS_MANAGE_H
#define _FDS_MANAGE_H

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

/* 函数声明 */
void fdsInit();
void addUser(int sockfd);
void delUser(int sockfd);
void copyFds(fd_set *target);

#endif /* _FDS_MANAGE_H */ 