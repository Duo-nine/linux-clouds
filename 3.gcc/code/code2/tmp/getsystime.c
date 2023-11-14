#include "getsystime.h"

char *getSysTime()
{
	time_t time_stamp;//存放时间戳
	static char time_data[20]={0};

	time(&time_stamp);
	struct tm *p=localtime(&time_stamp);

	sprintf(time_data,"%04d-%02d-%02d %02d:%02d:%02d"\
					 ,p->tm_year+1900,p->tm_mon+1,p->tm_mday\
					 ,p->tm_hour,p->tm_min,p->tm_sec);
	return time_data;

}
