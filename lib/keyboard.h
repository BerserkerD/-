#define PA_Addr		0x270
#define PB_Addr		0x271
#define PC_Addr		0x272
#define	CON_Addr	0x273

#define u8	unsigned char 
#define u16	unsigned int 

extern char inportb( unsigned int );								//读I/O
extern void outportb( unsigned int, char);

void delay(u16 ms)
{
	u16 i;
	while(ms--)
	{
		i = 100;
		do
		{;}while(--i);
	}
}

u8 AllKey()
{
	u8 i;
	outportb(PB_Addr, 0x0);
	i = (~inportb(PC_Addr) & 0x3);
	return i;
}

u8 key()
{
	u8 i, j, keyResult;
	u8 bNoKey = 1;
	while(bNoKey)
	{
		if (AllKey() == 0)		//调用判有无闭合键函数
		{
			continue;
		}
		i = 0xfe;
		keyResult = 0;
		do
		{
			outportb(PB_Addr, i);
			j = ~inportb(PC_Addr);
			if (j & 3)
			{
				bNoKey = 0;
				if (j & 2)				//1行有键闭合
					keyResult += 8;
			}
			else						//没有键按下
			{
				keyResult++;			//列计数器加1
				i = ((i << 1) | 1);
			}
		}while(bNoKey  && (i != 0xff));
	}
	if (!bNoKey)
	{
		while(AllKey())		//判断释放否
		{
			
		}
	}
	return keyResult;
}



