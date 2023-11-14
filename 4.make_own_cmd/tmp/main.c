/* 编写程序 复制一篇文件 */
/* copy /home/xxxx/????.c  */

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "xxx.h"

int main(int argc,char **argv){

	if( argc != 3 ){
		printf("<usage> : %s [src_file] [dest_path]\n",argv[0]);
		return -1;
	}

	char *srcfile_path = argv[1];			//源文件完成路径
	char *srcfile_name = basename(argv[1]);	//源文件文件名
	char *destfile     = argv[2];			//参数2 目标路径(携带文件名)
	FILE *fp_1 = NULL;
	FILE *fp_2 = NULL;
	int buf = 0;
	struct stat file_stat;	//定义stat类型的结构体变量 用于存放文件信息

	if( (-1 == stat(destfile,&file_stat)) ){	// 读取destfile文件信息
		if( errno != ENOENT ){
			return -1;
		}
	}else{
		if( S_ISDIR(file_stat.st_mode) ){	//判断destfile是否是一个目录
			/* 是路径 拼接文件名 */
			strcat(destfile,"/");
			strcat(destfile,srcfile_name);
			printf("line(%d) : destfile = %s\n",__LINE__,destfile);
		}
	}

	fp_1 = fopen(srcfile_path,"rb");
	if( !fp_1 ){
		perror("fopen srcfile_path");
		return -1;
	}
	
	fp_2 = fopen(destfile,"wb");
	if( !fp_2 ){
		perror("fopen destfile");
		return -1;
	}

	while( (buf = fgetc(fp_1)) != EOF ){	/*  复制的过程 */
		fputc(buf,fp_2);
	}

	fclose(fp_1);
	fclose(fp_2);
	
	return 0;
}

/* 
	file I/O 文件操作 标准I/O

	1.打开文件	fopen()
	
	2.数据读写
		fgetc()/fputc()		文件单字符读写
		fgets()/fputs()		文件字符串读写
		fread()/fwrite()	文件数据块读写 √
		fprintf()/fscanf()	文件格式化字符读写
	
	3.关闭文件	fclose()

==================================================================================

	FILE *fopen(const char *path, const char *mode);
	函数功能:打开文件(打开文件的模式 打开文件的类型)
	函数参数:path		 字符串->文件的路径
			 mode    字符串->打开模式
			 				 读写方法: 		'r' - 以只读模式打开文件 - 如果目标文件不存在,打开失败
			 				 			'w' - 以覆盖写入(只写)模式打开文件 - 如果目标文件不存在,会创建文件
			 				 			'a' - 以追加写入(只写)模式打开文件 - 如果目标文件不存在,会创建文件
			 				 			'+' - 以读写模式打开文件
			 				 				  "r+" 光标在文件首部
			 				 				  "w+" 清空文件内容,光标在文件首部
			 				 				  "a+" 光标在文件末尾
			 				 操作类型:
			 				 			't' - 缺省值,文本模式(ascii)
			 				 			'b' - 二进制模式,(bin) 使用较多
	返 回 值:成功会返回一个指向已经被打开文件的文件指针
			 失败返回NULL,并且 errno 会根据错误类型立即设置


==================================================================================

	int fclose(FILE *stream);
	函数功能:关闭文件
	函数参数:stream			文件指针(必须指向一个已经被打开的文件)
	返 回 值:成功返回0,失败返回-1

==================================================================================

	void perror(const char *s);
	函数功能:输出程序中最近一次出错的函数的错误原因(依赖errno)
	函数参数:s	-> 字符串(通常放置用户自定义的调式信息)
	返 回 值:无返回值

==================================================================================

	int fgetc(FILE *stream);
	函数功能:从文件中读取一个字符
	函数参数:stream	 	 文件指针
	返 回 值:成功返回读取到的字符(unsigned char),当读取到文件末尾时返回EOF(-1)

	int fputc(int c, FILE *stream);
	函数功能:向文件中写入一个字符
	函数参数:c			要写入的字符
			 stream	文件指针
	返 回 值:成功返回具体写入的字符值,失败返回EOF(-1)

==================================================================================

	char *fgets(char *s, int size, FILE *stream);
	函数功能:从文件中获取一串(指定长度)字符
	函数参数:s		 	指针->存储读取字符串的位置
			 size	读取字符的长度
			 stream	文件指针(写stdin表示从程序标准输入缓冲区 读取一串字符)
	返 回 值:成功返回第一个参数的值
			 失败返回空,如果已经读取到文件末尾,将直接返回



	char *dirname(char *path);
	函数功能:根据路径获取文件名(获取一个路径中的最后一段文件名称)
	函数参数:path		 路径名称字符串首地址			 	
	返 回 值:文件名称字符串首地址

	int stat(const char *pathname, struct stat *buf);
	函数功能:根据pathname找到并读取文件信息，存入buf指向的区域中
	函数参数:pathname		 文件路径
			 buf		 stat结构体指针
	返 回 值:On success, zero is returned. On error, -1 is returned.
	

	char *strerror(int errnum);
	函数功能:根据errno值,获取错误信息
	函数参数:errnum			错误码
	返 回 值:错误信息字符串首地址
	

 */
