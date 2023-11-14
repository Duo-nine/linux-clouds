#include <stdio.h>
#include <unistd.h>

extern char *optarg;
extern int   optind;
extern int   opterr;
extern int   optopt;

int main(int argc, char **argv){

	int ret;

	/* 检查主函数传入的参数是否处理完成 */
	while( -1 != (ret = getopt(argc,argv,"a:b:h")) ){
		switch( ret ){
			case 'a' : {
				printf("option 'a' -> %s\n",optarg);
				break;
			};
			case 'b': {
				printf("option 'b' -> %s\n",optarg);
				break;
			};
			case 'h':{
				

				return 0;
			};
			case '?': {break;};	/* 应当携带参数但没有携带参数的 || 无效选项 */
			default : {
				break;
			};
		}
	}

	

	return 0;
}

/*
	int getopt(int argc, char *const argv[], const char *optstring);
	函数功能:处理主函数传入的参数选项
	函数参数:argc		...
			 argv	...
			 optstring    一个包含正确的参数选项字符串，用于参数的解析
			 			  "abc:"
	返 回 值:如果成功处理了一个参数选项,那么函数返回该选项字符
			 如果处理完了所有应当处理的参数选项,那么函数返回-1
			 如果发现了不在optstring中定义的参数选项,那么函数返回'?'
			 如果遇到缺少参数的选项，则返回值取决于 optstring 中的第一个字符':'如果它是':' 则返回':' 否则返回'?'

	optarg : 如果某个选项有参数，这指向当前选项的参数字符串
	optind : argv 的当前索引值
	opterr : 正常运行状态下为 0 
			 非零时表示存在无效选项或者缺少选项参数，并输出其错误信息
	optopt : 当发现无效选项字符时，即 getopt() 方法返回 '?' 字符
	         optopt 中包含的就是发现的无效选项字符

*/
