#include "server.h"
#include "client_handle.h"

int main(int argc, char **argv)
{
	unsigned short port = 60000;
	int server_socket;
	//char buffer[1024]  = {0};
	pthread_t pt;
	
	if( argc > 2 )/* 主函数检查写入参数个数 */
	{	
		printf("< usage > : %s [port] \n", argv[0]);
		printf("            Default port is 60000.\n");
		return -1;
	}
	if( argv[1] != NULL )
	{
		port = atoi(argv[1]);
	}
	if( -1 == (server_socket = serverSocketInit(port)) )
	{
		printf("#Error : Server socket initialize failed.\n"); return -1;
	}
	if( pthread_create(&pt, NULL, threadAcceptClient, &server_socket) )
	{
		perror("pthread_create");
		return -1;
	}

	while(1)
	{
		sleep(1);
	}

	return 0;
}



/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	用来接收客户端的连接
		@sockfd  - 服务端套接字
		@addr    - 协议地址结构指针,如果需要存储连接的客户端协议地址信息
				   使用该指针指定存储区域首地址
		@addrlen - 指针,该指针指向的内存单元中存储了协议地址信息的大小
	成功返回一个套接字,该套接字用来和刚刚连接好的客户端进行通信
	失败返回-1

*/
