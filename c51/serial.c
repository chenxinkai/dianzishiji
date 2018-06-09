#include"reg52.h"
#include"common.h"
#include"serial.h"
void serial_init()  //串口程序初始化
{
	TMOD &= ~(3 << 4) ; //定时器1工作在方式2
	TMOD |= (2 << 4);
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1; //开启定时器1
	SM0 = 0;
	SM1 = 1; //串行口工作方
	REN = 1;//运行串行接口接受数据
	EA = 1; //开启中断
	ES = 1; //串行口允许中断
}
void serial_send_ch(unsigned char send_dat)//发送字符
{
		ES=0;
		SBUF=send_dat;
		while(!TI);
		TI=0;
		ES=1;
}
void serial_send_da(unsigned char send_dat[])//发送字符串
{
	unsigned char num=0;
		ES=0;
		while( send_dat[num]!='\0')
		{
			SBUF=send_dat[num];
			while(!TI);
			TI=0;
			num++;
		}
		ES=1;
}
unsigned char serial_recv_da() //接受字符一个字符
{
	RI=0;
	return SBUF;
}
