#include "myfb.h"

/* 全局变量 */
int fb_fd;
struct fb_var_screeninfo vinfo;	/* 用来存储屏幕信息可变参数 */
size_t fbmem_size;
void *p_fbmem = NULL;

/* 在屏幕中显示一个像素点(RGB888) */
void fbDisplayPixel(int x, int y, int color){
	/* 在屏幕中找到目标像素点的起始地址 */
	unsigned char  *pen8  = p_fbmem + (y * vinfo.xres + x) * vinfo.bits_per_pixel / 8;
	unsigned short *pen16 = (unsigned short *)pen8;
	unsigned int   *pen32 = (unsigned int *)pen8;
	int red, green, blue;

	if( x >= vinfo.xres || x < 0 ){
		return;
	}
	if( y >= vinfo.yres || y < 0 ){
		return;
	}

	/* 检查位深 */
	switch( vinfo.bits_per_pixel ){
		case 8 : {	/* RGB332 */
			red   = (color >> 16) & 0xff;
			green = (color >>  8) & 0xff;
			blue  = color & 0xff;
			color = ((red >> 5) << 5) | ((green >> 5) << 2) | (blue >> 6);
			*pen8 = color;
			break;
		};
		case 16: {	/* RGB565 */
			red   = (color >> 16) & 0xff;
			green = (color >>  8) & 0xff;
			blue  = color & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen16 = color;
			break;
		};
		case 32: {	/* RGB888 */
			*pen32 = color;
		};
		default : printf("Can not suppport %u bits_per_pixel.\n", vinfo.bits_per_pixel);
	}
}

void fbClean(int color){
	int i, j;
	for(i = 0; i < vinfo.xres; i++){
		for(j = 0; j < vinfo.yres; j++){
			fbDisplayPixel(i, j, color);
		}
	}
}

/* 显示一行像素的数据 RGB888 */
void fbDisplayLineData(int x, int y, int width, const unsigned char *pline){
	int i;
	int ix = x;
	int color;
	for(i = 0; i < width * 3; i += 3, ix++){
		color = (pline[i] << 16) | (pline[i+1] << 8) | pline[i+2] ;	/* 组合RGB888 */
		fbDisplayPixel(ix, y, color);
	}
}

/* 将一张图片显示在LCD中 RGB888 */
void fbDisplayImage(int x, int y,const unsigned char *pImage, int width, int height){
	int i;
	unsigned char *p_image = (unsigned char *)pImage;
	for(i = 0; i < height; i++, y++){
		fbDisplayLineData(x, y, width, p_image);
		p_image += width * 3;
	}
}


/* 将一张图片显示在LCD中 RGB565 */
void fbShowImage(int x, int y,const unsigned char *pImage, int width, int height){
	/* 在屏幕中找到显示图片的起始地址 */
	void *pen = p_fbmem + (y * vinfo.xres + x) * vinfo.bits_per_pixel / 8;
	size_t fb_line    = vinfo.xres * vinfo.bits_per_pixel / 8;
	size_t image_line = width * 2;
	unsigned char *p_image = (unsigned char *)pImage;
	int i;
	for(i = 0; i < height; i++){
		memcpy(pen, p_image, image_line);
		p_image += image_line;
		pen += fb_line;
	}
}

/* fb设备初始化 */
int fbInit(){
	if( -1 == (fb_fd = open(DEVICE_FB_PATHNAME, O_RDWR)) ){	/* 获取fb设备文件描述符 */
		perror("open");
		return -1;
	}

	/* 获取fb设备 可变参数(动态参数) */
	if( -1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) ){
		perror("ioctl");
		return -1;
	}
	fbmem_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8 ;

	/* 内存映射 */
	p_fbmem = mmap(NULL, fbmem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if( p_fbmem == (void *)-1 ){
		perror("mmap");
		return -1;
	}

	printf("==== Device \"%s\" initialized succeed ====\n", basename(DEVICE_FB_PATHNAME));
	printf("screen width   : %u px\n", vinfo.xres);
	printf("screen height  : %u px\n", vinfo.yres);
	printf("bits per pixel : %u bits\n", vinfo.bits_per_pixel);
	printf("=========================================\n");
	return 0;
}

/* fb设备释放 */
int fbDestory(){
	if(munmap(p_fbmem, fbmem_size) == -1){
		perror("munmap");
		return -1;
	}
	close(fb_fd);
	return 0;
}

