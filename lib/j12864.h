
//12864Һ����ʾ��

#define WR_COM_AD_L		0x264				//д�����ָ���ַ
#define WR_COM_AD_R		0x260				//д�Ұ���ָ���ַ
#define WR_DATA_AD_L 	0x266				//д��������ݵ�ַ	
#define WR_DATA_AD_R	0x262				//д�Ұ������ݵ�ַ
#define RD_BUSY_AD 		0x261				//��æ��ַ
#define RD_DATA_AD 		0x263				//�����ݵ�ַ

#define X 			0xB8							//��ʼ��ʾ�л�ַ
#define Y			0x40							//��ʼ��ʾ�л�ַ
#define FirstLine	0xC0							//��ʼ��ʾ��

extern void outportb( unsigned int, char);
extern char inportb( unsigned int );

//***************************************
//��������		
//***************************************					
//д���������ָ��
void WRComL(unsigned char _data)
{
	outportb(WR_COM_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д�Ұ�������ָ��
void WRComR(unsigned char _data)
{
	outportb(WR_COM_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д���������
void WRDataL(unsigned char _data)
{
	outportb(WR_DATA_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д�Ұ�������
void WRDataR(unsigned char _data)
{
	outportb(WR_DATA_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//��ʾ��������ݣ�count-��ʾ���ݸ���
void DisplayL(unsigned char *pt, char count)
{
	while (count--)
	{
		WRDataL(*pt++);			//д���������
	}
}

//��ʾ�Ұ������ݣ�count-��ʾ���ݸ���
void DisplayR(unsigned char * pt, char count)
{
	while (count--)
	{
		WRDataR(*pt++);			//д�Ұ�������
	}
}

//�����������ʼ��ʾ���е�ַ,x-X��ʼ������(0-7)��y-Y��ʼ������(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x+X);				//�е�ַ=������+�л�ַ
	WRComL(y+Y);				//�е�ַ=������+�л�ַ
}

//�����Ұ�����ʼ��ʾ���е�ַ,x:X��ʼ������(0-7)��y:Y��ʼ������(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x+X);				//�е�ַ=������+�л�ַ
	WRComR(y+Y);				//�е�ַ=������+�л�ַ
}

//***************************************
//��ʾͼ��		
//***************************************
//��ʾ�����һ��ͼ��,A-X��ʼ������(0-7)��B-Y��ʼ�е�ַ����(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYL(x,y);				//������ʼ��ʾ����
	DisplayL(pt, 64);			//��ʾ����
}

//��ʾ�Ұ���һ��ͼ��,A-X��ʼ�е�ַ����(0-7)��B-Y��ʼ�е�ַ����(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYR(x,y);				//������ʼ��ʾ����
	DisplayR(pt, 64);			//��ʾ����
}

//***************************************
//��ʾ���壬��ʾһ������Ҫռ��X������λ��
//***************************************
//�Ұ�����ʾһ���ֽ�/�֣�x-��ʼ��ʾ������X(0-7)��y-��ʼ��ʾ������Y(0-63)��pt-��ʾ�������׵�ַ
void ByteDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt, 8);			//��ʾ�ϰ�������
	SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt+8, 8);		//��ʾ�°�������
}

void WordDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt, 16);		//��ʾ�ϰ�������
	SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt+16, 16);	//��ʾ�°�������
}

//�������ʾһ���ֽ�/�֣�x-��ʼ��ʾ������X(0-7)��y-��ʼ��ʾ������Y(0-63)��pt-��ʾ�������׵�ַ
void ByteDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt, 8);		//��ʾ�ϰ�������
	SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt+8, 8);		//��ʾ�°�������
}

void WordDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt, 16);		//��ʾ�ϰ�������
	SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt+16, 16);	//��ʾ�°�������
}
//����
void LCDClear()
{
//�������
	unsigned char x,y;
	char j;
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = 0; x < 8; x++)		//��8��
	{
		SETXYL(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataL(0);
	}
//���Ұ���
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = 0; x < 8; x++)		//��8��
	{
		SETXYR(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataR(0);
	}
}

//Һ����ʼ��	
void LCD_INIT()
{
	WRComL(0x3e);			//��ʼ�������������ʾ
	WRComL(FirstLine);		//������ʼ��ʾ�У���0��
	WRComR(0x3e);			//��ʼ���Ұ���������ʾ
	WRComR(FirstLine);		//������ʼ��ʾ�У���0��
	LCDClear();				//����
	WRComL(0x3f);			//����ʾ
	WRComR(0x3f);			//����ʾ
}

//��ʱ����
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
		//Һ����ʼ��
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

