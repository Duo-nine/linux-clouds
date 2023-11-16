#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <gif_lib.h>

struct fb_var_screeninfo var;	//可变参数
struct fb_fix_screeninfo fix;	//固定参数
unsigned char *fb_mem=NULL; 	//LCD屏的首地址

/*
函数功能: 画点
*/
void LCD_WritePoint(int x,int y,int color)
{
	unsigned int *lcd=(unsigned int *)(fb_mem+y*var.xres*var.bits_per_pixel/8+x*var.bits_per_pixel/8);
	*lcd=color; //颜色赋值
}

//帧缓冲显示
void FrameBufferDraw(int x,int y,int image_w,int image_h,unsigned char *rgbBuf)
{
	int w,h;
	unsigned char r,g,b; 
	unsigned int c;
	/*将图像数据显示在LCD屏幕上*/
	unsigned char *rgb_p=rgbBuf;
	for(h=0;h<image_h;h++)
	{
		for(w=0;w<image_w;w++)
		{
			b=*rgb_p++;
			g=*rgb_p++;
			r=*rgb_p++;
			c=r<<16|g<<8|b<<0;
			LCD_WritePoint(w+x,h+y,c); /*绘制像素点到LCD屏*/
		}
	}
}

//颜色转换
void GifBufferToRgb888(ColorMapObject *ColorMap, unsigned char *inRgb,GifRowType *Buffer, int w, int h)
{
    GifColorType *ColorMapEntry = NULL;
    GifRowType GifRow = NULL;
    unsigned char *rgbBuf = inRgb;
    int idxH = 0;
    int idxW = 0;

    for (idxH = 0; idxH < h; idxH++)
    {
        GifRow = Buffer[idxH];
        rgbBuf = inRgb + idxH * w * 3;

        for(idxW = 0; idxW < w; idxW++)
        {
            ColorMapEntry = &ColorMap->Colors[GifRow[idxW]];
			*rgbBuf++ = ColorMapEntry->Blue;
			*rgbBuf++ = ColorMapEntry->Green;
            *rgbBuf++ = ColorMapEntry->Red;    
        }
    }
}

//显示GIF图像
int LCD_DisplayGIF(int x,int y,unsigned char *file)
{
	int error=0;
	int size;
	int i;
	GifRowType *Buffer;
	GifFileType *fp;
    /*1. 打开图片文件*/
	fp=DGifOpenFileName(file,&error);
	if(fp==NULL)return -1;
	printf("GIF图片尺寸:%dx%d\n",fp->SWidth,fp->SHeight);
	
	/*2. 内存空间申请、初始化*/
	Buffer=(GifRowType*)malloc(fp->SHeight*sizeof(GifRowType));
	
	/*一行字节大小*/
	size = fp->SWidth*sizeof(GifPixelType);
	Buffer[0]=(GifRowType)malloc(size);
	
	/*将其颜色设置为BackGround*/
    for(i=0;i<fp->SWidth;i++)
	{
		Buffer[0][i]=fp->SBackGroundColor;
	}
	
	/*分配其他行，并将它们的颜色也设置为背景 */
	for(i=1;i<fp->SHeight;i++)
	{
		Buffer[i]=(GifRowType)malloc(size);
		memcpy(Buffer[i],Buffer[0],size);
	}
	
	/*3. 显示图片*/
	ColorMapObject *colorMap=NULL;
	GifByteType *extension=NULL;
	GifRecordType gRecordType=UNDEFINED_RECORD_TYPE;

	int InterlacedOffset[]={0,4,2,1};  // The way Interlaced image should
	int InterlacedJumps[]={8,8,4,2};   // be read - offsets and jumps...
	unsigned char rgbBuf[800 * 480]={0};
	
	int extCode = 0;
	int row = 0;
	int col = 0;
	int width = 0;
	int height = 0;
	int iW = 0;
	int iH = 0;
	do
	{
		if(DGifGetRecordType(fp,&gRecordType)==GIF_ERROR)break;
		
		switch(gRecordType)
		{
			case IMAGE_DESC_RECORD_TYPE:
				if(DGifGetImageDesc(fp)==GIF_ERROR)break;

				row=fp->Image.Top;
				col=fp->Image.Left;
				width=fp->Image.Width;
				height=fp->Image.Height;

				if(fp->Image.Interlace)
				{
					for(iH=0;iH<4;iH++)
					{
						for(iW=row+InterlacedOffset[iH];iW<row+height;iW+=InterlacedJumps[iH])
						{
							DGifGetLine(fp,&Buffer[iW][col],width);
						}
					}
				}
				else
				{
					for(iH=0;iH<height;iH++)
					{
						DGifGetLine(fp,&Buffer[row++][col],width);
					}
				}

				colorMap=(fp->Image.ColorMap?fp->Image.ColorMap:fp->SColorMap);
				if(colorMap==NULL)
				{
					break;
				}

				GifBufferToRgb888(colorMap,rgbBuf,Buffer,fp->SWidth,fp->SHeight);
				//将图像显示在LCD屏上
				FrameBufferDraw(x,y,fp->SWidth,fp->SHeight,rgbBuf);
				//帧间隔时间
				usleep(1000*50);
				break;

			case EXTENSION_RECORD_TYPE:
				/* 跳过文件中的所有扩展块*/
				if(DGifGetExtension(fp,&extCode,&extension)==GIF_ERROR)break;

				while(extension!=NULL)
				{
					if(DGifGetExtensionNext(fp, &extension) == GIF_ERROR)break;
				}
				break;
				
			case TERMINATE_RECORD_TYPE:
				break;

			default:
				break;
		}
	}while(gRecordType!=TERMINATE_RECORD_TYPE);
	
	/*4. 释放空间*/
	for(i =0;i<fp->SHeight;i++)
    {
       free(Buffer[i]);
    }
    free(Buffer);
    DGifCloseFile(fp,&error);
    return 0;
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("./app <GIF图片文件>\n");
		return 0;
	}
	
	int fd=open("/dev/fb0",O_RDWR);
	if(fd<0)
	{
		perror("设备文件打开失败");
		return 0;
	}
	
	/*1. 获取LCD屏的可变形参*/
	ioctl(fd,FBIOGET_VSCREENINFO,&var);
	printf("分辨率:%d*%d\n",var.xres,var.yres);
	printf("像素点位数:%d\n",var.bits_per_pixel);
	
	/*2. 获取LCD屏的固定形参*/
	ioctl(fd,FBIOGET_FSCREENINFO,&fix);
	printf("映射的长度:%d\n",fix.smem_len);
	printf("一行的字节数:%d\n",fix.line_length);
	
	/*3. 映射LCD缓冲区地址到进程空间*/
	fb_mem=mmap(NULL,fix.smem_len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(fb_mem==NULL)
	{
		perror("空间映射失败!\n");
		return 0;
	}
	
	/*4. 控制显示屏*/
	memset(fb_mem,0x000000,fix.smem_len); //将屏幕背景清屏为白色
	
	while(1)
	{
		printf("GIF图片显示状态:%d\n",LCD_DisplayGIF(100,100,argv[1]));
	}
	
	munmap(fb_mem,fix.smem_len);
	close(fd);
	return 0;
}
