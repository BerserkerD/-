#define PA_Addr		0x270
#define PB_Addr		0x271
#define PC_Addr		0x272
#define	CON_Addr	0x273

#define u8	unsigned char 
#define u16	unsigned int 

extern char inportb( unsigned int );								//��I/O
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
		if (AllKey() == 0)		//���������ޱպϼ�����
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
				if (j & 2)				//1���м��պ�
					keyResult += 8;
			}
			else						//û�м�����
			{
				keyResult++;			//�м�������1
				i = ((i << 1) | 1);
			}
		}while(bNoKey  && (i != 0xff));
	}
	if (!bNoKey)
	{
		while(AllKey())		//�ж��ͷŷ�
		{
			
		}
	}
	return keyResult;
}



