#include <stdio.h>
#include <stdlib.h>

/* argc 记录主函数参数的个数 */
/* argv 指向主函数参数具体的内容 */
int main(int argc, char **argv){

	/* 主函数的参数检查 */
	if( argc != 2 ){
		printf("<usage> : %s [string]\n",argv[0]);
		return -1;
	}

	printf("integer : %d\n",atoi(argv[1]));

	return 0;
}

/*
	int atoi(const char *nptr);		ascii to integer
	函数功能:将字符串(中的数字字符)转为整型数值
	函数参数:nptr		 字符串首地址
	返 回 值:转换后的整型数值

*/


/* 编写程序 由主函数的参数 传入一个整型数字字符串 将其转为整形数 */


/* Segmentation fault (core dumped) -> 段错误 */
