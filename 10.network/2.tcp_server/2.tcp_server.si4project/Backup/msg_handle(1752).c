#include "msg_handle.h"
#include "fds_manage.h"

extern char buffer[1024];
extern char *p_sktid;
extern char *p_msg;
char   warn[1024]  = {"Prohibit Sending Spaces or Line Breaks..."};


void msgHandler(int srcfd, char *msg)
{
	if(*buffer == '/')//判断客户端要和哪个客户端之间进行私聊
	{
		p_sktid = buffer + 1;	//指向id编号的字符
		p_msg	= buffer;
		while( *p_msg != ':' )
		{
			p_msg++;
		}
		if( send(atoi(p_sktid), p_msg + 1, strlen(p_msg + 1), 0) < 0 )
		{
			perror("send");
		}
	}
	else if(*buffer == '\r' || *buffer == '\n' || *buffer == ' ')
	{
		sprintf(buffer+1,"client%03d : %s\n", srcfd, warn);
		send(srcfd, msg + 1, strlen(msg + 1), 0);
	}

	else
	{
		msgBroadcast(srcfd, msg);
	}

}

void msgBroadcast(int srcfd, char *msg)
{
	fd_set fds;
	int i;
	copyFds(&fds);
	for(i = 0; i < FD_SETSIZE; i++)
	{
		if( FD_ISSET(i, &fds) )
		{
			sendMsgToTarget(srcfd, i, msg);
		}
	}
}

ssize_t sendMsgToTarget(int srcfd, int destfd, char *msg)
{
	char buffer[1024] = {0};

	sprintf(buffer ,"client%03d : %s\n", srcfd, msg);
	if( destfd == STDOUT_FILENO )
	{
		return write(destfd, buffer, strlen(buffer));
	}
	else
	{
		return send(destfd, buffer, strlen(buffer), 0);
	}
}



