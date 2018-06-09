#ifndef __MPU6050_H__
#define __MPU6050_H__
//
#define SMPLRT_DIV   0x19 //采样频率分频器
#define CONFIG 		 0x1A //配置外部帧频率，和低通滤波器
#define GYRO_CONFIG  0x1B //陀螺仪配置
#define ACCE_CONFIG  0x1C //加速度计配置
#define ACCEL_XOUT_H 0x3B //加速度计的x轴高8位
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H  0x43  //陀螺仪的x轴高8位
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1 	 0x6B	//电源管理
#define WHO_AM_I 	 0x75   //iic 寄存器地址
void mpu6050_init();	//mpu初始化
void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz);
void calibrate_mpu6050(int *bias_gyro,int *bias_acce);
void mpu6050_get_data(int *p);
void mpu6050_timer() ;//计算积分时间

//
#endif