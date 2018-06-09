#ifndef __MPU6050_H__
#define __MPU6050_H__
//
#define SMPLRT_DIV   0x19 //����Ƶ�ʷ�Ƶ��
#define CONFIG 		 0x1A //�����ⲿ֡Ƶ�ʣ��͵�ͨ�˲���
#define GYRO_CONFIG  0x1B //����������
#define ACCE_CONFIG  0x1C //���ٶȼ�����
#define ACCEL_XOUT_H 0x3B //���ٶȼƵ�x���8λ
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H  0x43  //�����ǵ�x���8λ
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1 	 0x6B	//��Դ����
#define WHO_AM_I 	 0x75   //iic �Ĵ�����ַ
void mpu6050_init();	//mpu��ʼ��
void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz);
void calibrate_mpu6050(int *bias_gyro,int *bias_acce);
void mpu6050_get_data(int *p);
void mpu6050_timer() ;//�������ʱ��

//
#endif