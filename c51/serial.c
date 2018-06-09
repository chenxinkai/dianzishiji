#include"reg52.h"
#include"common.h"
#include"serial.h"
void serial_init()  //���ڳ����ʼ��
{
	TMOD &= ~(3 << 4) ; //��ʱ��1�����ڷ�ʽ2
	TMOD |= (2 << 4);
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1; //������ʱ��1
	SM0 = 0;
	SM1 = 1; //���пڹ�����
	REN = 1;//���д��нӿڽ�������
	EA = 1; //�����ж�
	ES = 1; //���п������ж�
}
void serial_send_ch(unsigned char send_dat)//�����ַ�
{
		ES=0;
		SBUF=send_dat;
		while(!TI);
		TI=0;
		ES=1;
}
void serial_send_da(unsigned char send_dat[])//�����ַ���
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
unsigned char serial_recv_da() //�����ַ�һ���ַ�
{
	RI=0;
	return SBUF;
}
