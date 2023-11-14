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

#define RED		0xFF0000
#define ORANGE	0xFF7F00
#define YELLOW	0xFFFF00
#define GREEN	0x00FF00
#define CYAN	0x00FFFF
#define BLUE	0x0000FF
#define PURPLE	0x7F00FF

#define DEVICE_FB_PATHNAME	"/dev/fb0"

int main(){

	int fb_fd;
	struct fb_var_screeninfo vinfo;	/* 用来存储屏幕信息可变参数 */
	size_t fbmem_size;
	void *p_fbmem = NULL;

	if( -1 == (fb_fd = open(DEVICE_FB_PATHNAME, O_RDWR)) ){	/* 获取fb设备文件描述符 */
		perror("open");
		return -1;
	}

	/* 获取fb设备 可变参数(动态参数) */
	if( -1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) ){
		perror("ioctl");
		return -1;
	}

	printf("screen width   : %u px\n", vinfo.xres);
	printf("screen height  : %u px\n", vinfo.yres);
	printf("bits per pixel : %u bits\n", vinfo.bits_per_pixel);
	fbmem_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8 ;

	/* 内存映射 */
	p_fbmem = mmap(NULL, fbmem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if( p_fbmem == (void *)-1 ){
		perror("mmap");
		return -1;
	}

	memset(p_fbmem, 0xF8, fbmem_size);

	munmap(p_fbmem, fbmem_size);
	close(fb_fd);

	return 0;
}

/* 
	位深 : 一个像素点占用多少位来存储颜色数据

	
*/
