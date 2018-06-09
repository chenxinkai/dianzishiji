#include"reg52.h"
#include"common.h"
#include"iic.h"
#include"mpu6050.h"
//
void mpu6050_init()	//mpu初始化
{
	iic_init();
	iic_write_address(PWR_MGMT_1,0x00);
	iic_write_address(SMPLRT_DIV, 0x07);
	iic_write_address(CONFIG,0x06);
	iic_write_address(GYRO_CONFIG,0x18);
	iic_write_address(ACCE_CONFIG,0x01);
}
void mpu6050_timer() //计算积分时间
{
	TMOD = 0x01;
	TH0 = 0x00;
	TL0 = 0x00;
	ET0 =1;
	EA =1;
}
void mpu6050_get_data(int *p)
{
	unsigned char temp[12];
	unsigned char num,num1;
	for (num=1;num<21;num++)
	{
		for(num1=0;num1<6;num1+1)
		{
			temp[num1]=iic_read_address(ACCEL_XOUT_H + num1);
		}
		for(num1=0;num1<6;num1++)
		{
			temp[6+num1]=iic_read_address(GYRO_XOUT_H + num1);
		}
		*p     = (int)( (int) (temp[0]<<8) | temp[1]);
		*(++p) = (int)( (int) (temp[2]<<8) | temp[3]);
		*(++p) = (int)( (int) (temp[4]<<8) | temp[5]);
		
		*(++p) = (int)( (int) (temp[6]<<8) | temp[7]);
		*(++p) = (int)( (int) (temp[8]<<8) | temp[9]);
		*(++p) = (int)( (int) (temp[10]<<8) | temp[11]);
	}
}
void calibrate_mpu6050(int *bias_gyro,int *bias_acce)
{
	unsigned char temp[12];
	unsigned char num,num1;
	long  bias_temp[6]={'0','0','0','0','0','0'};
	int bias_a[3]={'0','0','0'};
	int bias_g[3]={'0','0','0'};
	for (num=1;num<21;num++)
	{
		for(num1=0;num1<6;num1+1)
		{
			temp[num1]=iic_read_address(ACCEL_XOUT_H + num1);
		}
		for(num1=0;num1<6;num1++)
		{
			temp[6+num1]=iic_read_address(GYRO_XOUT_H + num1);
		}
		bias_a[0] = (int)( (int) (temp[0]<<8) | temp[1]);
		bias_a[1] = (int)( (int) (temp[2]<<8) | temp[3]);
		bias_a[2] = (int)( (int) (temp[4]<<8) | temp[5]);
		
		bias_g[0] = (int)( (int) (temp[6]<<8) | temp[7]);
		bias_g[1] = (int)( (int) (temp[8]<<8) | temp[9]);
		bias_g[2] = (int)( (int) (temp[10]<<8) | temp[11]);
		
		bias_temp[0] += bias_a[0];
		bias_temp[1] += bias_a[1];
		bias_temp[2] += bias_a[2];
		bias_temp[4] += bias_g[0];
		bias_temp[5] += bias_g[1];
		bias_temp[6] += bias_g[2];
		
	}
	*(bias_acce)   = (int)(bias_temp[0] / 20);
	*(++bias_acce) = (int)(bias_temp[1] / 20);
	*(++bias_acce) = (int)(bias_temp[2] / 20);
	*(bias_gyro)   = (int)(bias_temp[3] / 20);
	*(++bias_gyro) = (int)(bias_temp[4] / 20);
	*(++bias_gyro) = (int)(bias_temp[5] / 20);
}
void MadgwickQuaternionUpdate(float q[],float ax, float ay, float az, float gx, float gy, float gz,float deltat,float zeta)
//四元数转换
{
            float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];         
            float norm;                                               
            float f1, f2, f3;                                         
            float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; 
            float qDot1, qDot2, qDot3, qDot4;
            float hatDot1, hatDot2, hatDot3, hatDot4;
            float gerrx, gerry, gerrz, gbiasx, gbiasy, gbiasz;  

            
            float _halfq1 = 0.5f * q1;
            float _halfq2 = 0.5f * q2;
            float _halfq3 = 0.5f * q3;
            float _halfq4 = 0.5f * q4;
            float _2q1 = 2.0f * q1;
            float _2q2 = 2.0f * q2;
            float _2q3 = 2.0f * q3;
            float _2q4 = 2.0f * q4;

			norm = sqrt(ax * ax + ay * ay + az * az);
            if (norm == 0.0f) return; 
            norm = 1.0f/norm;
            ax *= norm;
            ay *= norm;
            az *= norm;
            
             
            f1 = _2q2 * q4 - _2q1 * q3 - ax;
            f2 = _2q1 * q2 + _2q3 * q4 - ay;
            f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;
            J_11or24 = _2q3;
            J_12or23 = _2q4;
            J_13or22 = _2q1;
            J_14or21 = _2q2;
            J_32 = 2.0f * J_14or21;
            J_33 = 2.0f * J_11or24;
          
            
            hatDot1 = J_14or21 * f2 - J_11or24 * f1;
            hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
            hatDot3 = J_12or23 * f2 - J_33 *f3 - J_13or22 * f1;
            hatDot4 = J_14or21 * f1 + J_11or24 * f2;
            
           
            norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
            hatDot1 /= norm;
            hatDot2 /= norm;
            hatDot3 /= norm;
            hatDot4 /= norm;
            
            
            gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;
            gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;
            gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;
            
            
            gbiasx += gerrx * deltat * zeta;
            gbiasy += gerry * deltat * zeta;
            gbiasz += gerrz * deltat * zeta;
			
            
            qDot1 = -_halfq2 * gx - _halfq3 * gy - _halfq4 * gz;
            qDot2 =  _halfq1 * gx + _halfq3 * gz - _halfq4 * gy;
            qDot3 =  _halfq1 * gy - _halfq2 * gz + _halfq4 * gx;
            qDot4 =  _halfq1 * gz + _halfq2 * gy - _halfq3 * gx;

            
            q1 += (qDot1 -(beta * hatDot1)) * deltat;
            q2 += (qDot2 -(beta * hatDot2)) * deltat;
            q3 += (qDot3 -(beta * hatDot3)) * deltat;
            q4 += (qDot4 -(beta * hatDot4)) * deltat;

          
            norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    
            norm = 1.0f/norm;
            q[0] = q1 * norm;
            q[1] = q2 * norm;
            q[2] = q3 * norm;
            q[3] = q4 * norm;
}
 