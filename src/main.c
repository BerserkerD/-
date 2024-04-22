#include "keyboard.h"
#include "j12864.h"
#include "zimo.h"

#define ENTER_STATION		2
#define EXIT_STATION		6

#define MOVE_FORWARD		1
#define STEP_BACK			5

#define SWITCH				0
#define AD					4

unsigned char** stations[] = {sta1, sta2, sta3, sta4, sta5, sta6, sta7, sta8, sta9, sta10, sta11};
unsigned char* welcome[] = {wel_1, wel_2, wel_3, wel_4, wel_5, wel_6, wel_7, wel_8, wel_9};
unsigned char* ad[] = {ad_1, ad_2, ad_3, ad_4, ad_5, ad_6, ad_7, ad_8, ad_9,ad_10,ad_11,ad_12};
int lengths[] = {8, 2, 3, 4, 3, 4, 4, 4, 4, 3, 3};


u8 key_val = 0;
int pos = 0;
int k = 7;
int isarrival = 0; //是否到站
int isexist = 0; //是否出站
int isad = 0;//是否播放广告
int isgoup = 1;//是否上行，否则下行
int jmpout = 0;//循环播放是否跳出
int iscircul = 0;//是否循环播放


void checkjmp(u16 ms) {
		
	u16 i;
	while(ms--)
	{
		if(AllKey()!=0) jmpout = 1;
		i = 100;
		do
		{;}while(--i);
	}
		
} 

void main()
{
	
	outportb(CON_Addr, 0x89);	//PA、PB output ，PC input
			
	LCD_INIT();
	Display_initPic(welcome);
	
	while(1)
	{
		
		key_val = key();
		
					
		LCD_INIT();
		delay(100);
		
		//更改广告
		if(key_val == SWITCH) {
			isgoup = isgoup == 1 ? -1 : 1;	
		} else if(key_val == AD) {
			isad = isad == 1 ? 0 : 1;	
		} else isad = 0;
		
		//更改播放方式
		if(key_val == EXIT_STATION) {
			iscircul = 1;	
		} else if(key_val != SWITCH && key_val != AD) {
			iscircul = 0;
		}
		
		//更改站点位置
		if(key_val == ENTER_STATION){
			isarrival = 1;
			isexist = 0;
		} else if(key_val == EXIT_STATION){
			pos = (pos + isgoup) >= 0 && (pos + isgoup) < 11 ? pos + isgoup : pos;
			isarrival = 0;
			isexist = 1;
		} else if(key_val == MOVE_FORWARD){
			pos = (pos + isgoup) >= 0 && (pos + isgoup) < 11 ? pos + isgoup : pos;
		} else if(key_val == STEP_BACK){	
			pos = (pos - isgoup) >= 0 && (pos - isgoup) < 11 ? pos - isgoup : pos;
		}
		
		
		if(isad) {//广告
			Display_ad(ad);
		} else if(!iscircul) {//不循环播放
			
			Display_arrow(isgoup == 1 ? arrow_right : arrow_left, isgoup);
			Display_sta(stations, lengths, pos);
			if(isarrival) Display_arrival(arrival);
			else if(isexist) Display_next(next);
			
		} else if(iscircul) {//循环播放
		
			while(AllKey()) {
			}
			
			while(1) {
				
				if(AllKey() != 0) break;
				circulating_sta(stations, lengths, pos, k);
				k = --k >= (0-lengths[pos]) ? k : 7;
				Display_arrow(isgoup == 1 ? arrow_right : arrow_left, isgoup);
				if(isarrival) Display_arrival(arrival);
				else if(isexist) Display_next(next);
				
				delay(300);
				//checkjmp(300);
				LCD_INIT();
				//if(jmpout == 1) break;
				
			}
			k = 7;
			jmpout = 0;
			
		}	
	}
}



