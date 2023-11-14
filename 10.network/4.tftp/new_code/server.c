#include "server.h"

static void getIpv4ByNetworkDeviceName(char (*ip)[16]);

/* serverSocketInit - 创建并初始化服务端监听套接字
   @port - 服务端绑定的端口号
   成功返回初始化好的监听套接字 失败返回-1					*/
int serverSocketInit(unsigned short port){
	int server_socket;
	struct sockaddr_in server_addr;
	int optval = 1;
	
	if( -1 == (server_socket = socket(AF_INET, SOCK_STREAM, 0)) ){
		perror("socket");
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* IP 0.0.0.0 泛指本机所有可以使用的IP*/

	if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ){
		perror("setsockopt");
		return -1;
	}
	if( bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) ){
		perror("bind");
		return -1;
	}
	if( listen(server_socket, 50) ){
		perror("listen");
		return -1;
	}

	char ip[16][16];
	getIpv4ByNetworkDeviceName(ip);

	/* 根据网络设备名称获取IP */
	printf("=============================\n");
	printf("Tcp server established succeed.\n");
	printf("IPv4 : %s | %s | %s\n", ip[0], ip[1], ip[2]);
	printf("Port : %d\n", port);
	printf("=============================\n");
	
	return server_socket;
}

/* 根据网络设备名称获取IP */
static void getIpv4ByNetworkDeviceName(char (*ip)[16]){

	int fd;								/* 套接字 */
	int intrface;
    struct ifreq buf[INET_ADDRSTRLEN];  //这个结构定义在/usr/include/net/if.h，用来配置和获取ip地址，掩码，MTU等接口信息的
    struct ifconf ifc;
	//static char ip[16][16] = {0};
 
	/* 1 建立socket链接，利用ioctl来自动扫描可用网卡*/
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0){
        ifc.ifc_len = sizeof(buf);
        // caddr_t,linux内核源码里定义的：typedef void *caddr_t；
        ifc.ifc_buf = (caddr_t)buf;
		/*2  这里获取到可用网卡清单，包括网卡ip地址，mac地址*/
		if( -1 == ioctl(fd, SIOCGIFCONF, (char *)&ifc) ){
			perror("ioctl");
			return ;
		}
        intrface = ifc.ifc_len/sizeof(struct ifreq);  //计算出有效网卡的数量
        printf("intrface = %d\n", intrface);
		int x = 0;
        while (intrface-- > 0){	/* 有剩余未处理的网络设备 */
			//CHECK_LINE;
			/*3  遍历并索引指定网卡的地址*/
			if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[intrface]))){
//				if(strcmp(buf[intrface].ifr_ifrn.ifrn_name,dev) == 0){
//					//CHECK_LINE;
//					strcpy(ip, (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr)));
//					//printf("IP:%s\n", ip);
//				}
				if( strcmp(buf[intrface].ifr_ifrn.ifrn_name,"") ){
					strcpy(ip[x++], (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr)));
				}
			}
    	}
    }
	close(fd);
	return ;
}

/*
struct ifconf{
	int ifc_len;             //Size of buffer.
	union{
		__caddr_t ifcu_buf;
		struct ifreq *ifcu_req;
	} ifc_ifcu;
};  //通常用来保存所有的接口信息

*/
