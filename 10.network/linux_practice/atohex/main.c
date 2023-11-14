#include <stdio.h>
#include <string.h>

//void atohex(const char* str)//ASCII码转16进制hex格式
//{
//    int i;
//    for(i = 0; str[i] != '\0'; i++)
//	{
//        printf("%#x ",(unsigned char)str[i]);
//    }
//    printf("\n");
//}
//int main()
//{
//	char s[1024]={0};
//	scanf("%s",s);
//    atohex(s);
//    return 0;
//}


//char *aToHex(char *s) 
//{
//    int i;
//    for(i = 0; s[i] != '\0'; i++)
//	{
//        printf("%#x ",(unsigned char)s[i]);
//    }
//    return s;
//}
//
//int main()
//{
//	char s[1024]={0};
//	scanf("%s",s);
//    printf("%s", aToHex(s));
//    return 0;
//}


//char  *stringToHex(const char* str, char* hexStr) {
//    while (*str) {
//        sprintf(hexStr, "%s%#x ", hexStr, (unsigned char)*str);
//        str++;
//        hexStr += 5; //下一个位置，留出空格的位置
//    }
//    return hexStr;
//}
//
//int main(int argc,char **argv) {
//    const char* str = argv[1];
//    char hexStr[100] = ""; // 初始化为空字符串
//    stringToHex(argv[1], hexStr);
//    printf("%s\n", stringToHex(argv[1], hexStr));
//    return 0;
//}
