float servoQianhou()
{//前后轴的计算*************************
	unsigned float temp1,temp2;
	adTimeData[0]=iic_read_address(0x90,0x00,0x91);
	angley1=((adTimeData[1] - adInitData[1]) * angler);
	if (angley1 >= 0)
	{
		flag = 1;
		temp1 = distancex( 1 - cos(angley1)); //x- x*cos
		temp2 = diatancey + sin(angley1)*distancex ;//y + sin *x
		angley2 = atan(float (temp1 / temp2));
	}
	else 
	{
		flag = 0;
		angley1 = fabs(angley1);
		temp1 = distancex( 1 - cos(angley1));//x - x*cos
		temp2 = diatancey - sin(angley1)*distancex ;//y - sin*x 
		angley2 = atan(float (temp1 / temp2));
	}
	return angley2;
//*************************************
}
void servoAngleConvert(float angleTemp,unsigned char *ph,unsigned char *pl)
{
	unsigned int temp;
	unsigned char temp1;
	temp = (unsigned int)(angleTemp * servoUnit);
	if (flag == 1)//+++++
	{
		temp1 = *++ph;
		*ph =*ph + (temp >> 8);
		*++ph = temp1 + temp;
		
		temp1 = *++pl;
		*pl = *pl - (temp >> 8);
		*++pl = temp1 - temp;
	}
	else 
	{
		temp1 = *++ph;
		*ph =*ph -(temp >> 8);
		*++ph = temp1 - temp;
		
		temp1 = *++pl;
		*pl = *pl + (temp >> 8);
		*++pl = temp1 + temp;
	}		
}