#include "server.h"

/* serverSocketInit - 创建并初始化服务端监听套接字
   @port - 服务端绑定的端口号
   成功返回初始化好的监听套接字 失败返回-1					*/
int serverSocketInit(unsigned short port)
{
	int server_socket;
	struct sockaddr_in server_addr;
	int optval = 1;
	
	if( -1 == (server_socket = socket(AF_INET, SOCK_STREAM, 0)) )
	{
		perror("socket");
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* IP 0.0.0.0 泛指本机所有可以使用的IP*/

	if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) )
	{
		perror("setsockopt");
		return -1;
	}
	if( bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) )
	{
		perror("bind");
		return -1;
	}
	if( listen(server_socket, 50) )
	{
		perror("listen");
		return -1;
	}
	return server_socket;
}

