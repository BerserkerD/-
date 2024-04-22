
//12864液晶显示屏

#define WR_COM_AD_L		0x264				//写左半屏指令地址
#define WR_COM_AD_R		0x260				//写右半屏指令地址
#define WR_DATA_AD_L 	0x266				//写左半屏数据地址	
#define WR_DATA_AD_R	0x262				//写右半屏数据地址
#define RD_BUSY_AD 		0x261				//查忙地址
#define RD_DATA_AD 		0x263				//读数据地址

#define X 			0xB8							//起始显示行基址
#define Y			0x40							//起始显示列基址
#define FirstLine	0xC0							//起始显示行

extern void outportb( unsigned int, char);
extern char inportb( unsigned int );

//***************************************
//基本控制		
//***************************************					
//写左半屏控制指令
void WRComL(unsigned char _data)
{
	outportb(WR_COM_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏控制指令
void WRComR(unsigned char _data)
{
	outportb(WR_COM_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写左半屏数据
void WRDataL(unsigned char _data)
{
	outportb(WR_DATA_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏数据
void WRDataR(unsigned char _data)
{
	outportb(WR_DATA_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//显示左半屏数据，count-显示数据个数
void DisplayL(unsigned char *pt, char count)
{
	while (count--)
	{
		WRDataL(*pt++);			//写左半屏数据
	}
}

//显示右半屏数据，count-显示数据个数
void DisplayR(unsigned char * pt, char count)
{
	while (count--)
	{
		WRDataR(*pt++);			//写右半屏数据
	}
}

//设置左半屏起始显示行列地址,x-X起始行序数(0-7)，y-Y起始列序数(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x+X);				//行地址=行序数+行基址
	WRComL(y+Y);				//列地址=列序数+列基址
}

//设置右半屏起始显示行列地址,x:X起始行序数(0-7)，y:Y起始列序数(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x+X);				//行地址=行序数+行基址
	WRComR(y+Y);				//列地址=列序数+列基址
}

//***************************************
//显示图形		
//***************************************
//显示左半屏一行图形,A-X起始行序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYL(x,y);				//设置起始显示行列
	DisplayL(pt, 64);			//显示数据
}

//显示右半屏一行图形,A-X起始行地址序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYR(x,y);				//设置起始显示行列
	DisplayR(pt, 64);			//显示数据
}

//***************************************
//显示字体，显示一个数据要占用X行两行位置
//***************************************
//右半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 8);			//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+8, 8);		//显示下半行数据
}

void WordDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 16);		//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+16, 16);	//显示下半行数据
}

//左半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 8);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+8, 8);		//显示下半行数据
}

void WordDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 16);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+16, 16);	//显示下半行数据
}
//清屏
void LCDClear()
{
//清左半屏
	unsigned char x,y;
	char j;
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYL(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataL(0);
	}
//清右半屏
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYR(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataR(0);
	}
}

//液晶初始化	
void LCD_INIT()
{
	WRComL(0x3e);			//初始化左半屏，关显示
	WRComL(FirstLine);		//设置起始显示行，第0行
	WRComR(0x3e);			//初始化右半屏，关显示
	WRComR(FirstLine);		//设置起始显示行，第0行
	LCDClear();				//清屏
	WRComL(0x3f);			//开显示
	WRComR(0x3f);			//开显示
}

//延时程序
void DelayTime()
{
	unsigned char i;
	unsigned int j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 0xffff; j++)
		{;}
	}
}

void Display_sta(unsigned char*** pt, int* lengths, int pos) {
	
	unsigned char** zhanming = pt[pos];
	int length_sta = lengths[pos];
	int i;
	for(i=0; i<length_sta; i++) {
		//液晶初始化
		if(i < length_sta/2){
			unsigned char col = 64 - 16 * (length_sta/2 - i);
			WordDisL(2, col, zhanming[i]);
		} else {
			unsigned char col = 16 * (i - length_sta/2);
			WordDisR(2, col, zhanming[i]);
		}
		
	}
	
}


void circulating_sta(unsigned char*** pt, int* lengths, int pos, int k) {
	
	unsigned char** zhanming = pt[pos];
	int length = lengths[pos];
	int i;
	for(i=0; i<length; i++) {
		if(k+i > 7 || k+i < 0)continue;
		else {
			if(k+i>=4) WordDisR(2, (k+i-4)*16, zhanming[i]);
			else WordDisL(2, (k+i)*16, zhanming[i]);	
		}
	}
		
}

void Display_arrival(unsigned char **pt){

	WordDisL(4, 48, pt[0]);
	WordDisR(4, 0, pt[1]);
	
}


void Display_next(unsigned char **pt){

	WordDisL(4, 48, pt[0]);
	WordDisR(4, 0, pt[1]);
	WordDisR(4, 16, pt[2]);
	
}

void Display_initPic(unsigned char **pt) {

	WordDisL(2, 32, pt[0]);
	WordDisL(2, 48, pt[1]);
	WordDisL(4, 24, pt[4]);
	WordDisL(4, 40, pt[5]);
	ByteDisL(4, 56, pt[6]);
	
	WordDisR(2, 0, pt[2]);
	WordDisR(2, 16, pt[3]);
	WordDisR(4, 0, pt[7]);
	WordDisR(4, 16, pt[8]);
	
}

void Display_ad(unsigned char **pt) {

	int i;
	for(i = 0; i < 8;i++){
		ByteDisL(2, 8*i, pt[i]);
	}
	for(i = 8; i < 12;i++){
		WordDisR(2, 16*(i-8), pt[i]);
	}
}

void Display_arrow(unsigned char *arrow, int dirt){
	
	if(dirt == 1) WordDisR(0, 48, arrow);
	else if(dirt == -1) WordDisL(0, 0, arrow);
		
}

