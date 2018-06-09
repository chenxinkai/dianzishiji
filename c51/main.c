#include"common.h"
#include"iic.h"
#include"reg52.h"
#include"math.h"
//AD 配置****************************************
static unsigned char adData[2]={0,0};//存放校准时的ad值
static unsigned char adInitData[2]={0,0};//初始ad采集值
static unsigned char adTimeData[2]={0,0}; //存放实时采集数据
static unsigned char flag; //控制角度的正负
// **********************************************
//舵机配置**************************
sbit servoOne=P1^0;
sbit servoTwo=P1^1;
sbit servoChange=P0^0;
sbit servoQuite=P0^1;
#define ANCON 7.199
#define angler 1.875 //一个分度值对应的角度
#define distancex 60 // 激光到转轴的距离
#define distancey 100 // 激光到靶子的距离
#define servoUnit 10.239 //一个定时器单位等于的舵机角度 
static unsigned char servoHighOne[2]={0,0};//舵机1定时器数据 高电平时间
static unsigned char servoLowOne[2]={0,0};//舵机1定时器数据，低电平时间 = 20ms - 高电平时间
static unsigned char servoHighTwo[2]={0,0};//舵机2定时器数据 高电平时间
static unsigned char servoLowTwo[2]={0,0};//舵机2定时器数据，低电平时间 = 20ms - 高电平时间
//****************************************

//舵机函数*****************************
void servoTimeInit() //舵机定时器初始化
{
	TMOD &= ~(3 << 0);//定时器0清除
	TMOD |=  (1 << 0);//工作方式1
	TH0 = servoHighOne[0];
	TL0 = servoHighOne[1];
	TMOD &= ~(3 << 4);//定时器1清除
	TMOD |=  (1 << 4);//工作方式1
	TH1 = servoHighTwo[0];
	TL1 = servoHighTwo[1];
	ET0 =1;
	ET1 =1;
	TR1 =1;
	TR0 = 1;
	EA=1;
	servoOne = 1;
	servoTwo = 1;
}
void servoCalculation(unsigned char speed,unsigned char *ph,unsigned char *pl)//0~255 对应舵机1~180*
{
	unsigned int temph,templ,temp; 
	temp = (unsigned int)(speed * ANCON);
	temph = (65076 - temp);
	templ = (47563 + temp);
	*ph = (temph >> 8);
	*++ph = temph;
	*pl = (templ >> 8);
	*++pl = templ;
}
void servoTimerOne() interrupt 1//舵机1定时器中断程序，产生pwm
{
	servoOne=~servoOne;
	if (servoOne == 0)
	{
	TR0=0;
	TH0 = servoLowOne[0];
	TL0 = servoLowOne[1];
	TR0=1;
	}
	else 
	{
	TR0=0;
	TH0 = servoHighOne[0];
	TL0 = servoHighOne[1];
	TR0=1;
	}
}
void servoTimerTwo() interrupt 3 //舵机2定时器中断程序，产生pwm
{
	servoTwo=~servoTwo;
	if (servoTwo == 0)
	{
	TR1=0;
	TH1 = servoLowTwo[0];
	TL1 = servoLowTwo[1];
	TR1=1;
	}
	else 
	{
	TR1=0;
	TH1 = servoHighTwo[0];
	TL1 = servoHighTwo[1];
	TR1=1;
	}
}
void servoCalibration(unsigned char *p) //舵机校准；
{
	unsigned char ad;
	while(servoQuite)
	{
		while (servoChange)
		{
		ad = iic_read_address(0x90,0x03,0x91);
		*p = ad;
		servoCalculation(adData[0],servoHighOne,servoLowOne);
		}
		ad = iic_read_address(0x90,0x03,0x91);
		*(p+1) = ad;
		servoCalculation(adData[1],servoHighTwo,servoLowTwo);
	}
}
//***************************************
void saveAdDate(unsigned char *p)
{
	*p = iic_read_address(0x90,0x00,0x91);
	*++p = iic_read_address(0x90,0x01,0x91);
}
float servoQianhou()
{//前后轴的计算*************************
	float temp1,temp2;
	float angley1,angley2;
	adTimeData[1]=iic_read_address(0x90,0x01,0x91);
	angley1=(float)((adTimeData[1] - adInitData[1]) * angler);
	if (angley1 >= 0)
	{
		flag = 1 ;//加
		temp1 = distancex - distancex*cos(angley1); //x- x*cos
		temp2 = distancey + sin(angley1)*distancex ;//y + sin *x
		angley2 = atan(temp1 / temp2);
	}
	else 
	{
		flag = 0 ;//减
		angley1 = fabs(angley1);
		temp1 = distancex - distancex*cos(angley1);//x - x*cos
		temp2 = distancey - sin(angley1)*distancex ;//y - sin*x 
		angley2 = atan(temp1 / temp2);
	}
	return angley2;
//*************************************
}
void servoAngleConvert(float angleTemp,unsigned char *ph,unsigned char *pl)
{
	unsigned int temp,temp1,temp2;
	unsigned char temp3;
	temp = (unsigned int)(angleTemp * servoUnit * 28.6);
	if (flag == 1)
	{
	temp3 = *++ph;
	temp1 = *ph * 256 + temp3;
	temp2 = temp1 + temp;
	*ph =(temp2 >> 8);
	*++ph = temp2;
		
	temp3 = *++pl;
	temp1 = *pl * 256 + temp3;
	temp2 = temp1 - temp;
	*pl =(temp2 >> 8);
	*++pl = temp2;
	}
	else 
	{
	temp3 = *++ph;
	temp1 = *ph * 256 + temp3;
	temp2 = temp1 - temp;
	*ph =(temp2 >> 8);
	*++ph = temp2;
		
	temp3 = *++pl;
	temp1 = *pl * 256 + temp3;
	temp2 = temp1 + temp;
	*pl =(temp2 >> 8);
	*++pl = temp2;
	}
}
void main()
{
	float temp;
	iic_init();
	servoTimeInit();
	servoCalibration(adData);
	delay_ms(1000);
	saveAdDate(adInitData);
	while(1)
	{
		temp = servoQianhou();
		servoAngleConvert(temp,servoHighOne,servoLowOne);
	}
}
