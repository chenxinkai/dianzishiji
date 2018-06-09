#include"iic.h"
#include"reg52.h"
#include"common.h"
sbit scl=P2^1;
sbit sda=P2^0;
//iid 驱动
void iic_init()
{
	scl=1;
	delay_ms(1);
	sda=1;
	delay_ms(1);
}
void iic_start() //start single
{
	sda=1;
	scl=1;
	delay_10us(1);
	sda=0;
	delay_10us(1);
	scl=0;
	delay_10us(1);
}
void iic_stop() //stop single
{
	sda=0;
	scl=1;
	delay_10us(1);
	sda=1;
	delay_10us(1);
	sda=0;
}
void iic_response() //从机应答信号
{
	unsigned char wt; //wait time
	scl=1;
	delay_10us(1);
	while(( sda==1 ) && ( wt<250 ))		
		wt++;
	scl=0;
	delay_10us(1);
}
void iic_write_byte(unsigned char dat)
{
	unsigned char num,temp;
	temp = dat;
	for( num=0 ; num<8 ; num++ )
	{
		temp=temp<<1;
		scl=0;
		delay_10us(1);
		sda=CY;
		delay_10us(1);
		scl=1;
		delay_10us(1);
	}
	scl=0;
	delay_ms(1);
	sda=1;
	delay_ms(1);
}
unsigned char iic_read_byte()
{
	unsigned char num,byte;
	scl=0;
	delay_ms(1);
	sda=1;
	delay_ms(1);
	for ( num=0 ; num<8 ; num++ )
	{
		scl=1;
		delay_ms(1);
		byte=(byte<<1)|sda;
		scl=0;
		delay_ms(1);
	}
	return byte;
}
void iic_write_address(unsigned char SlaveAddress,address,dat)
{
	iic_start();
	iic_write_byte(SlaveAddress);
	iic_response();
	iic_write_byte(address);
	iic_response();
	iic_write_byte(dat);
	iic_response();
	iic_stop();
}
unsigned char iic_read_address(unsigned char writeAddress,unsigned char controlByte,unsigned char readAddress)
{
	unsigned char dat;
	iic_start();
	iic_write_byte(writeAddress);
	iic_response();
	iic_write_byte(controlByte);
	iic_response();
	
	iic_start();
	iic_write_byte(readAddress);
	iic_response();
	dat=iic_read_byte();
	iic_stop();
	return dat;
}