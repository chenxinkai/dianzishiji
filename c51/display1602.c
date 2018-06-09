#include"display1602.h"
#include"common.h"
#include"reg52.h"

sbit lcden=P3^4;
sbit lcdrs=P3^5;
sbit lcdrw=P3^6;
sbit duanla=P2^6;
sbit weila=P2^7;
void write_com(unsigned char com) //write command 
{
	lcdrs=0;
	P0=com;
	delay_ms(1);
	lcden=1;
	delay_ms(1);
	lcden=0;
}
void write_data(unsigned char dat) //write data
{
	lcdrs=1;
	P0=dat;
	delay_ms(1);
	lcden=1;
	delay_ms(1);
	lcden=0;
}
void init_1602()
{
	duanla=0;
	weila=0;
	lcden=0;
	lcdrw=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
}
void display_1602_one(unsigned char x,y,cha) //显示一个字符
{
	if ( x == 1)
	{
		write_com(0x80+y);
		write_data(cha);
	}
	else 
	{
		write_com(0x80+0x40+y);
		write_data(cha);
	}
}
void display_1602_multi(unsigned char x,y,unsigned char temp_display[])//显示一个字符串
{
	unsigned char num,temp;
	while(1)
	{
		temp=temp_display[num];
		if ( temp =='\0')
			break;
		display_1602_one(x,y+num,temp);
		delay_ms(1);
		num++;
	}
}
void format_display_1602(unsigned char x,y,int dat)//格式化int，便于1602显示
{
	unsigned char temp[7];
	unsigned char code number[]="0123456789";
	if (dat<0)
	{	
		dat=~(dat-1);
		temp[0]='-';
	}
	else 
	{
		temp[0]='+';
	}
	temp[1]= number[dat/10000];
	temp[2]= number[dat%10000/1000];
	temp[3]= number[dat%1000/100];
	temp[4]= number[dat%100/10];
	temp[5]= number[dat%10];
	display_1602_multi(x,y,temp);
}

