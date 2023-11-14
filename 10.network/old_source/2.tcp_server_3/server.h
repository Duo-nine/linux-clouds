#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* 函数声明 */
//创建并初始化服务端监听套接字
int serverSocketInit(unsigned short port);

#endif /* _SERVER_H */