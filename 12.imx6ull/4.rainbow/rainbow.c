#define RGB(A,B,C)		((A << 16) | (B << 8) | C )
#define CALIBRATE(A)	((A < 0) ? 0 :((A > 255) ? 255 : A))

void fb_show_rainbow(){
	int i,j;
	int area[6] = {0};
	int  rgb[3] = {255,0,0};
	int    t[6] = {1,0,2,1,0,2};
	int tlrc;
	int x = 0;
	
	for(i = 0;i < 6;i++){
		area[i] = fb_width / 6;
	}
	for(i = fb_width % 6;i > 0;i--){
		area[i] += 1;
	}
	
	tlrc = 0xff / area[i];

	for(i = 0;i < 6;i++){	//确认6部分每部分要变动的颜色权重 以及变动是数值增加还是数值减少
		for(j = 0;j < area[i];j++,x++){
//			printf("[%4d/%4d] ",x+1,fb_width);
//			printf("\"%s\" power %s --> ", ((t[i]==0)?"red":((t[i]==1)?"green":((t[i]==2)?"blue":"???")))
//				                      , (i%2)?"down":"up");
//			printf("0x %2x %2x %2x ",rgb[0],rgb[1],rgb[2]);
			if(i%2){
				*(rgb + t[i]) -= ((j%2) ? tlrc : (tlrc + 1)) ;
			}else{
				*(rgb + t[i]) += ((j%2) ? tlrc : (tlrc + 1)) ;
			}
			*(rgb + t[i]) = CALIBRATE(*(rgb + t[i]));			//校准颜色值
			fbDisplayColumn(x, RGB( *rgb , *(rgb + 1) ,*(rgb + 2)));	//根据颜色值显示一列
			//printf("(change value:%2d)\n",((j%2) ? tlrc : (tlrc + 1)));
		}
	}
}

/* 显示一列颜色 */
void fbDisplayColumn(int x,int rgb){
	int y;
	for(y = 0;y < fb_height;y++){
		fbDisplayPixel(x,y,rgb);
	}
}