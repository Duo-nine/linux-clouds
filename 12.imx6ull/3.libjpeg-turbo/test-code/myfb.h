#ifndef _MY_FB_H
#define _MY_FB_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <libgen.h>

#define DEVICE_FB_PATHNAME	"/dev/fb0"

/* RGB888 */
#define RED		0xFF0000
#define ORANGE	0xFF7F00
#define YELLOW	0xFFFF00
#define GREEN	0x00FF00
#define CYAN	0x00FFFF
#define BLUE	0x0000FF
#define PURPLE	0x7F00FF
#define WHITE	0xFFFFFF
#define BLACK	0x000000

/* 函数声明 */
int fbInit();						//fb设备初始化
int fbDestory();					//fb设备释放
void fbClean(int color);
void fbShowImage(int x, int y,const unsigned char *pImage, int width, int height);	//RGB565
void fbDisplayPixel(int x, int y, int color);
void fbDisplayLineData(int x, int y, int width, const unsigned char *pline);
void fbDisplayImage(int x, int y,const unsigned char *pImage, int width, int height);	//RGB888

#endif /* _MY_FB_H */
