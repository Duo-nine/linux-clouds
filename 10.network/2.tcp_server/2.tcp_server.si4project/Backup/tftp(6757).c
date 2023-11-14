#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
#define ERR_MSG(msg)                              \
    do {                                          \
        fprintf(stderr, "line : %d\n", __LINE__); \
        perror(msg);                              \
    } while (0)
 

int port; 
 
void menu(void); 										 // 显示TFTP文件客户端功能菜单
int Download_TFTP(int cfd, struct sockaddr_in sin); // 下载文件
int Upload_TFTP(int cfd, struct sockaddr_in sin);    // 上传文件
//int upload(int sfd, struct sockaddr_in sin);


//执行程序格式：[./exe] [服务端IP] [上传/下载] [文件名] [端口号]


int main(int argc, char **argv)
{
    int choice; // 功能选择
    port = 69;
    
    if( argc != 5 )//判断输入参数个数
	{
		printf("< usage > : %s [ipv4] [upload/download] [filename] [port]\n", argv[0]);
		return -1;
	}
		
    int sockfd;
	struct sockaddr_in target_addr;		/* 通讯目标 */
//	struct sockaddr_in local_addr;		//IP地址
	char buffer[1024] = {0};			//存放数据

	if( -1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) )//udp使用数据报套接字SOCK_DGRAM
	{
		ERR_MSG("socket error");;
		return -1;
	}
 
	port = argv[4];
	
	// 填充服务器信息结构体
	memset(&local_addr, 0x00, sizeof(target_addr));
	target_addr.sin_family       = AF_INET;
	target_addr.sin_port         = htons(atoi(port));	//获取本地端口号
	target_addr.sin_addr.s_addr  = inet_addr(argv[1]);		//使用任意IP
 
    while (1) 
	{
		if(!strcpy("upload",argv[2]))
		{
			printf("upload");
		}
//        menu();
        scanf("%d", &choice);
        getchar();
        switch (choice) 
		{
        	case 1:
            	Download_TFTP(sockfd, target_addr);
            	break;
        	case 2:
            	Upload_TFTP(sockfd, target_addr);
            	break;
        	case 3:
            	return -1;
        	default:
            	printf("\n输入格式错误!!!\n请在功能1 2 3中选择!!!\n\n");
            	sleep(1);
            	break;
        }
    }
 
    return 0;
}
 
void menu(void)
{
    printf("\n**************************TFTP文件客户端**************************\n");
    printf("***************************功能菜单目录***************************\n");
    printf("*\t\t\t1.下载文件\t\t\t\t *\n");
    printf("*\t\t\t2.上传文件\t\t\t\t *\n");
    printf("*\t\t\t3.退出系统\t\t\t\t *\n");
    printf("******************************************************************\n");
    printf("请选择你要执行的操作：");
}
 
int Download_TFTP(int cfd, struct sockaddr_in sin)
{
    ssize_t res; 			   // 存储返回值
    ssize_t Data_Num; 		   // 接收数据包中实际的大小
    char ACK1, ACK2; 		   // 存服务器发来数据包中块的编号
    int fd; 				   // 下载的文件描述符
    unsigned short opcode;     // 存储操作码
    int size; 				   // 发送协议包的大小
    char buf[516] = { 0 };     // 存储TFTP的5种协议包
    char filename[20] = { 0 }; // 存储下载文件的名字
 
    // 发送读请求
    printf("请输入下载的文件名称：");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';
 
    // 组装下载请求数据包
    size = sprintf(buf, "%c%c%s%c%s%c", 0x00, 0x01, filename, 0, "octet", 0);
    if (sendto(cfd, buf, size, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) 
	{
        ERR_MSG("sendto error");
        return -1;
    }
 
    // 打开文件
    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) 
	{
        ERR_MSG("open error");
        return -1;
    }
 
    // 获取服务器的临时端口
    struct sockaddr_in tem_sin;
    socklen_t addrlen = sizeof(tem_sin);
 
    while (1) 
	{
        // 清空buf
        bzero(buf, sizeof(buf));
 
        // 接收服务器发来的包
        if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0)
		{
            ERR_MSG("recvfrom error");
            break;
        }
 
        // 取出操作码，判断服务器发来的包的类型
        opcode = ntohs(*(short*)buf);
        if (opcode == 3) // 数据包
        {
            // 写入数据
            Data_Num = res - 4; // 数据的实际大小 = 整个数据包的大小 - 4(操作码 + 块编号)
            if (write(fd, buf + 4, Data_Num) < 0) 
			{
                ERR_MSG("write error");
                return -1;
            }
 
            // 组ACK包并发送给服务器
            ACK1 = buf[2];
            ACK2 = buf[3];
            bzero(buf, sizeof(buf));
            size = sprintf(buf, "%c%c%c%c", 0x00, 0x04, ACK1, ACK2);
            res = sendto(cfd, buf, size, 0, (struct sockaddr*)&tem_sin, sizeof(tem_sin));
            if (res < 0) 
			{
                ERR_MSG("sendto error");
                return -1;
            }
 
            // 数据的大小 < 512,说明数据已经读完，下载成功
            if (Data_Num < 512) 
			{
                printf("下载成功！请继续……\n");
                break;
            }
        }
		else if (opcode == 5) // 错误包
        {
            printf("[%d : %s]\n", ntohs(*(short*)(buf + 2)), buf + 4);
            break;
        }
    }
    return 0;
}
 
int Upload_TFTP(int cfd, struct sockaddr_in sin)
{
    ssize_t res; 				// 返回值
    int fd; 					// 下载的文件描述符
    unsigned short opcode; 		// 操作码
    unsigned short block_num;   // 块编号
    char buf[516] = { 0 }; 		// 存储TFTP的5种协议包
    char filename[20] = { 0 };  // 存储下载文件的名字
 
    // 发送上传请求
    printf("请输入需要上传文件的名称：");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';
 
    // 组装下载请求数据包
    int size = sprintf(buf, "%c%c%s%c%s%c", 0x00, 0x02, filename, 0, "octet", 0);
    if (sendto(cfd, buf, size, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
        ERR_MSG("sendto error");
        return -1;
    }
 
    // 获取服务器的临时端口
    struct sockaddr_in tem_sin;
    socklen_t addrlen = sizeof(tem_sin);
 
    // 接收服务器发来的包
    bzero(buf, sizeof(buf));
    if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0) 
	{
        ERR_MSG("recvfrom error");
    }
    opcode = ntohs(*(short*)buf); // 取出操作码
    block_num = ntohs(*(unsigned short*)(buf + 2)); // 取出块编号
 
    // 判断服务发来的ACK包是否合法
    if (opcode != 4 || block_num != 0) 
	{
        return -1;
    }
    printf("服务器ACK首次应答成功,开始传输数据……\n");
 
    // 打开文件
    if ((fd = open(filename, O_RDONLY)) < 0)
	{
        ERR_MSG("open error");
        return -1;
    }
 
    while (1) 
	{
        // 清空buf
        bzero(buf, sizeof(buf));
 
        // 组装发送数据包
        block_num++;
        char* ptr = buf;
        unsigned short* p_make = (unsigned short*)ptr;
        *p_make = htons(3); // 操作码
        unsigned short* p_block = p_make + 1;
        *p_block = htons(block_num); // 块编号
        res = read(fd, buf + 4, 512);
        if (res == 0) 
		{
            printf("上传成功！请继续……\n");
            break;
        } 
		else if (res == -1) 
		{
            ERR_MSG("read error");
            return -1;
        }
 
        // 给服务器发送数据包
        res = sendto(cfd, buf, res + 4, 0, (struct sockaddr*)&tem_sin, sizeof(tem_sin));
        if (res < 0) 
		{
            ERR_MSG("sendto error");
            return -1;
        }
 
        // 接收并检验服务器发来的包
        bzero(buf, sizeof(buf));
        if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0)
		{
            ERR_MSG("recvfrom error");
            return -1;
        }
 
        // 取出服务器发来的包的操作码
        opcode = ntohs(*(short*)buf);
        if (opcode == 4) 
		{
            if (block_num != ntohs(*(unsigned short*)(buf + 2))) 
			{
                break;
            }
        } 
		else if (opcode == 5) // 错误包
        {
            printf("[%d : %s]\n", ntohs(*(short*)(buf + 2)), buf + 4);
            break;
        }
    }
    return 0;
}
