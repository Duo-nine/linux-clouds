#ifndef _CLIENT_HANDLE_H
#define _CLIENT_HANDLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

/* 函数声明 */
//线程处理函数 - 接收客户端连接
void *threadAcceptClient(void *);
//线程处理函数 - 接收已连接客户端发送的消息
void *threadHandleClient(void *);


#endif /* _CLIENT_HANDLE_H */
