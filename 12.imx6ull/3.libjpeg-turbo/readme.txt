以libjpeg-turbo为例 学习如何使用第三方源码库

编写代码
1、获取源码：进入libjpeg-turbo官网
2、解压缩-建立项目 阅读相关手册和示例代码，了解基本的编程框架

配置环境
1、配置
	System types: --host= [cross-compile to build programs to run on HOST]
	--host参数 指定程序构建以及运行的平台 填写交叉编译工具链名称即可
	Installation directories: --prefix= [install architecture-independent files in PREFIX]
	--prefix参数 指定最终编译好的库文件\二进制可执行文件\文档手册\头文件 生成的位置(路径)
	CC参数指定编译C语言源文件是使用的指令（交叉编译）

./configure --host=arm-linux-gnueabihf --prefix=$PWD/_install CC=arm-linux-gnueabihf-gcc 

2、编译
	make -j8 (核数越高，速度越快)
3、编译安装
	make install
	安装好的文件会存放在prefix参数指定的目录中

arm-linux-gnueabihf-gcc *.c -o test -I /opt/libjpeg-turbo-1.5.2/_install/include/ -ljpeg -L /opt/libjpeg-turbo-1.5.2/_install/lib/

将编译安装后生成的头文件以及库文件拷贝至交叉编译工具中
头文件：cp /opt/libjpeg-turbo-1.5.2/_install/include/* /opt/arm-linux-4.9.4/arm-linux-gnueabihf/include/ -arf
库文件：cp /opt/libjpeg-turbo-1.5.2/_install/lib/* /opt/arm-linux-4.9.4/arm-linux-gnueabihf/lib/ -arf

开发板中运行jpeg测试文件时 由于编译过程链接了库文件(动态库)
因此实际运行时 需要将相关的库文件部署在设备中(/lib)


