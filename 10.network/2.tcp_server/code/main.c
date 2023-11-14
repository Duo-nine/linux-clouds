#include "server.h"
#include "client_handle.h"

int main(int argc, char **argv)
{
	unsigned short port = 60000;//执行程序时，不输入端口号，默认此端口号
	int server_socket;			//服务端套接字
	pthread_t pt;				//线程标识符
	char buffer[1024]  = {0};	//存放发送数据
	char *p_sktid = NULL;		//指向id编号的字符
	char *p_msg   = NULL;		//指向数据的内容
	
	//主函数检查输入参数的个数
	//[./执行程序][端口号]
	if( argc > 2 )
	{	
		printf("< usage > : %s [port] \n", argv[0]);
		printf("            Default port is 60000.\n");
		return -1;
	}

	//解析端口号
	//如果输入的第二个参数不为空
	//则输入的第二个参数中的端口号覆盖默认端口号
	if( argv[1] != NULL )
	{
		port = atoi(argv[1]);//atoi函数将输入的ascii码值转换为int类型值
	}
	if( -1 == (server_socket = serverSocketInit(port)) )
	{
		printf("#Error : Server socket initialize failed.\n"); 
		return -1;
	}
	if( pthread_create(&pt, NULL, threadAcceptClient, &server_socket) )
	{
		perror("pthread_create");
		return -1;
	}

	while(1)
	{
		//服务端向客户端发送消息
		//格式为：[/][用户端套接字编号][:][要输入的内容]
		fgets(buffer, sizeof(buffer), stdin);//stdin:标准输入
		
		if( *buffer != '/' )
		{
			printf("Invalid command.\n");
		}
		else
		{
			p_sktid = buffer + 1;	//接收id编号的字符
			p_msg   = buffer;		//接收数据格式
			while( *p_msg != ':' )
			{
				p_msg++;
			}

			//发送客户端向服务端发送的数据
			if( send(atoi(p_sktid), p_msg + 1, strlen(p_msg + 1), 0) < 0 )
			{
				perror("send");
			}
		}
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
